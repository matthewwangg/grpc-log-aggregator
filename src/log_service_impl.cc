#include <filesystem>
#include <fstream>

#include <grpcpp/grpcpp.h>

#include "log.grpc.pb.h"
#include "log_service_impl.h"
#include "utils/auth_utils.h"
#include "utils/log_utils.h"
#include "utils/time_utils.h"

grpc::Status LogServiceImpl::QueryLog(grpc::ServerContext* context, const log::QueryRequest* request, log::QueryResponse* response) {
    std::cout << "[QueryLog] Received from: " << context->peer() << std::endl;

    grpc::Status auth_status = auth_utils::CheckAuthorization(context);
    if (!auth_status.ok()) {
        std::cout << "[QueryLog] Failed to authenticate." << std::endl;
        return auth_status;
    }

    std::filesystem::path file_path = std::filesystem::path("log") / request->source() / (request->date() + ".log");

    std::vector<log::LogEntry> entries;
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

grpc::Status LogServiceImpl::SendLog(grpc::ServerContext* context, const log::LogEntry* request, log::LogResponse* response) {
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

    std::cout << "[SendLog] Successfully wrote log." << std::endl;
    response->set_success(true);
    return grpc::Status::OK;
}

grpc::Status LogServiceImpl::StreamLog(grpc::ServerContext* context, grpc::ServerReader<log::LogEntry>* reader, log::LogResponse* response) {
    std::cout << "[StreamLog] Connected from: " << context->peer() << std::endl;

    grpc::Status auth_status = auth_utils::CheckAuthorization(context);
    if (!auth_status.ok()) {
        std::cout << "[StreamLog] Failed to authenticate." << std::endl;
        return auth_status;
    }

    log::LogEntry entry;
    int count = 0;

    while (reader->Read(&entry)) {
        std::cout << "[StreamLog] Entry " << ++count << " written!" << std::endl;
        grpc::Status status = log_utils::WriteLogEntryToFile(entry);

        if (!status.ok()) {
            std::cout << "[StreamLog] Failed to write log." << std::endl;
            response->set_success(false);
            return status;
        }
    }

    std::cout << "[StreamLog] Finished receiving " << count << " entries." << std::endl;
    response->set_success(true);
    return grpc::Status::OK;
}

