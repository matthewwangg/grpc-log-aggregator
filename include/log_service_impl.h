#ifndef GRPC_LOG_AGGREGATOR_LOG_SERVICE_IMPL_H
#define GRPC_LOG_AGGREGATOR_LOG_SERVICE_IMPL_H

#include <grpcpp/grpcpp.h>

#include "log.grpc.pb.h"

class LogServiceImpl : public log::LogService::Service {
public:
    grpc::Status QueryLog(grpc::ServerContext* context, const log::QueryRequest* request, log::QueryResponse* response);
    grpc::Status SendLog(grpc::ServerContext* context, const log::LogEntry* request, log::LogResponse* response) override;
    grpc::Status StreamLog(grpc::ServerContext* context, grpc::ServerReader<log::LogEntry>* reader, log::LogResponse* response) override;
    grpc::Status SubscribeLog(grpc::ServerContext* context, const log::QueryRequest* request, grpc::ServerWriter<log::LogEntry>* writer) override;
};

#endif //GRPC_LOG_AGGREGATOR_LOG_SERVICE_IMPL_H
