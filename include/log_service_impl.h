#ifndef GRPC_LOG_AGGREGATOR_LOG_SERVICE_IMPL_H
#define GRPC_LOG_AGGREGATOR_LOG_SERVICE_IMPL_H

#include <grpcpp/grpcpp.h>

#include "log.grpc.pb.h"

class LogServiceImpl : public logging::LogService::Service {
public:
    enum class LogMode {
        ASYNC = 0,
        SYNC = 1,
    };

    LogServiceImpl(LogMode mode);

    grpc::Status QueryLog(grpc::ServerContext* context, const logging::QueryRequest* request, logging::QueryResponse* response);
    grpc::Status SendLog(grpc::ServerContext* context, const logging::LogEntry* request, logging::LogResponse* response) override;
    grpc::Status StreamLog(grpc::ServerContext* context, grpc::ServerReader<logging::LogEntry>* reader, logging::LogResponse* response) override;
    grpc::Status SubscribeLog(grpc::ServerContext* context, const logging::QueryRequest* request, grpc::ServerWriter<logging::LogEntry>* writer) override;

private:
    LogMode mode_;
};

#endif //GRPC_LOG_AGGREGATOR_LOG_SERVICE_IMPL_H
