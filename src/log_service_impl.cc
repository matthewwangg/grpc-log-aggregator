#include <filesystem>
#include <fstream>

#include <grpcpp/grpcpp.h>

#include "log.grpc.pb.h"
#include "log_service_impl.h"
#include "utils/log_utils.h"
#include "utils/time_utils.h"

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

