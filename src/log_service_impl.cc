#include <filesystem>
#include <fstream>

#include <grpcpp/grpcpp.h>

#include "log.grpc.pb.h"
#include "log_service_impl.h"
#include "utils/log_utils.h"
#include "utils/time_utils.h"

grpc::Status LogServiceImpl::QueryLog(grpc::ServerContext* context, const log::QueryRequest* request, log::QueryResponse* response) {
    std::filesystem::path file_path = std::filesystem::path("log") / request->source() / (request->date() + ".log");

    std::vector<log::LogEntry> entries;
    grpc::Status status = log_utils::ReadLogFileToEntries(file_path, request->keyword(), entries);

    if (!status.ok()) {
        return status;
    }

    for (const auto& entry : entries) {
        *response->add_entries() = entry;
    }

    return grpc::Status::OK;
}

grpc::Status LogServiceImpl::SendLog(grpc::ServerContext* context, const log::LogEntry* request, log::LogResponse* response) {
    grpc::Status status = log_utils::WriteLogEntryToFile(*request);

    if (!status.ok()) {
        response->set_success(false);
        return status;
    }

    response->set_success(true);
    return grpc::Status::OK;
}

grpc::Status LogServiceImpl::StreamLog(grpc::ServerContext* context, grpc::ServerReader<log::LogEntry>* reader, log::LogResponse* response) {
    log::LogEntry entry;

    while (reader->Read(&entry)) {
        grpc::Status status = log_utils::WriteLogEntryToFile(entry);

        if (!status.ok()) {
            response->set_success(false);
            return status;
        }
    }

    response->set_success(true);
    return grpc::Status::OK;
}

