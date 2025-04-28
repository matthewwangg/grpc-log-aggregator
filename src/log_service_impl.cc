#include "log_service_impl.h"

grpc::Status LogServiceImpl::SendLog(grpc::ServerContext* context, const log::LogEntry* request, log::LogResponse* response) {
    return grpc::Status::OK;
}