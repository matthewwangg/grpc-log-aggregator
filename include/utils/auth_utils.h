#ifndef GRPC_LOG_AGGREGATOR_AUTH_UTILS_H
#define GRPC_LOG_AGGREGATOR_AUTH_UTILS_H

namespace auth_utils {

grpc::Status CheckAuthentication(const grpc::ServerContext* context);

} // namespace auth_utils

#endif //GRPC_LOG_AGGREGATOR_AUTH_UTILS_H
