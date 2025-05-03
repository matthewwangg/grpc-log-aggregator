#ifndef GRPC_LOG_AGGREGATOR_AUTH_UTILS_H
#define GRPC_LOG_AGGREGATOR_AUTH_UTILS_H

#include <grpcpp/grpcpp.h>

namespace auth_utils {

grpc::Status CheckAuthorization(const grpc::ServerContext* context);

} // namespace auth_utils

#endif //GRPC_LOG_AGGREGATOR_AUTH_UTILS_H
