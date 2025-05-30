#include <filesystem>
#include <fstream>
#include <memory>
#include <queue>
#include <string>

#include <grpcpp/grpcpp.h>

#include "log.grpc.pb.h"
#include "log_service_impl.h"
#include "utils/auth_utils.h"
#include "utils/log_utils.h"
#include "utils/pubsub_utils.h"
#include "utils/time_utils.h"

grpc::Status LogServiceImpl::QueryLog(grpc::ServerContext* context, const logging::QueryRequest* request, logging::QueryResponse* response) {
    std::cout << "[QueryLog] Received from: " << context->peer() << std::endl;

    grpc::Status auth_status = auth_utils::CheckAuthorization(context);
    if (!auth_status.ok()) {
        std::cout << "[QueryLog] Failed to authenticate." << std::endl;
        return auth_status;
    }

    std::filesystem::path file_path = std::filesystem::path("log") / request->source() / (request->date() + ".log");

    std::vector<logging::LogEntry> entries;
    grpc::Status status = log_utils::ReadLogFileToEntries(file_path, request->keyword(), entries);

    if (!status.ok()) {
        std::cout << "[QueryLog] Failed to read log." << std::endl;
        return status;
    }

    for (const auto& entry : entries) {
        *response->add_entries() = entry;
    }

    std::cout << "[QueryLog] Returned " << entries.size() << " matching entries." << std::endl;
    return grpc::Status::OK;
}

grpc::Status LogServiceImpl::SendLog(grpc::ServerContext* context, const logging::LogEntry* request, logging::LogResponse* response) {
    std::cout << "[SendLog] Received from: " << context->peer() << std::endl;

    grpc::Status auth_status = auth_utils::CheckAuthorization(context);
    if (!auth_status.ok()) {
        std::cout << "[SendLog] Failed to authenticate." << std::endl;
        return auth_status;
    }

    grpc::Status status = log_utils::WriteLogEntryToFile(*request);

    if (!status.ok()) {
        std::cout << "[SendLog] Failed to write log." << std::endl;
        response->set_success(false);
        return status;
    }

    pubsub_utils::LogPubSub::Instance().Publish(*request);

    std::cout << "[SendLog] Successfully wrote log." << std::endl;
    response->set_success(true);
    return grpc::Status::OK;
}

grpc::Status LogServiceImpl::StreamLog(grpc::ServerContext* context, grpc::ServerReader<logging::LogEntry>* reader, logging::LogResponse* response) {
    std::cout << "[StreamLog] Connected from: " << context->peer() << std::endl;

    grpc::Status auth_status = auth_utils::CheckAuthorization(context);
    if (!auth_status.ok()) {
        std::cout << "[StreamLog] Failed to authenticate." << std::endl;
        return auth_status;
    }

    logging::LogEntry entry;
    int count = 0;

    while (reader->Read(&entry)) {
        std::cout << "[StreamLog] Entry " << ++count << " written!" << std::endl;
        grpc::Status status = log_utils::WriteLogEntryToFile(entry);

        if (!status.ok()) {
            std::cout << "[StreamLog] Failed to write log." << std::endl;
            response->set_success(false);
            return status;
        }

        pubsub_utils::LogPubSub::Instance().Publish(entry);
    }

    std::cout << "[StreamLog] Finished receiving " << count << " entries." << std::endl;
    response->set_success(true);
    return grpc::Status::OK;
}

grpc::Status LogServiceImpl::SubscribeLog(grpc::ServerContext* context, const logging::QueryRequest* request, grpc::ServerWriter<logging::LogEntry>* writer) {
    std::cout << "[SubscribeLog] Connected from: " << context->peer() << std::endl;

    grpc::Status auth_status = auth_utils::CheckAuthorization(context);
    if (!auth_status.ok()) {
        std::cout << "[SubscribeLog] Failed to authenticate." << std::endl;
        return auth_status;
    }

    std::shared_ptr<pubsub_utils::LogPubSub::SubscriberQueue> queue = pubsub_utils::LogPubSub::Instance().Subscribe(request->source());
    std::cout << "[SubscribeLog] Subscription successfully created." << std::endl;

    while (!context->IsCancelled()) {
        std::unique_lock<std::mutex> lock(queue->mu_);

        queue->cv_.wait(lock, [&] {
            return !queue->entries_.empty() || context->IsCancelled();
        });

        while (!queue->entries_.empty()) {
            logging::LogEntry entry = queue->entries_.front();
            queue->entries_.pop();

            lock.unlock();

            bool successful = writer->Write(entry);
            if (!successful) {
                break;
            }

            lock.lock();
        }
    }

    return grpc::Status::OK;
}
