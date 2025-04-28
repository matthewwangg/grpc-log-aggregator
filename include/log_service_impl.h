#ifndef GRPC_LOG_AGGREGATOR_LOG_SERVICE_IMPL_H
#define GRPC_LOG_AGGREGATOR_LOG_SERVICE_IMPL_H

#include <grpcpp/grpcpp.h>

#include "log.grpc.pb.h"

class LogServiceImpl : public log::LogService::Service {
public:
    grpc::Status SendLog(grpc::ServerContext* context, const log::LogEntry* request, log::LogResponse* response) override;
};


#endif //GRPC_LOG_AGGREGATOR_LOG_SERVICE_IMPL_H
