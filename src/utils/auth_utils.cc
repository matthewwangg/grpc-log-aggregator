#include "utils/auth_utils.h"

#include <grpcpp/grpcpp.h>

namespace auth_utils {

grpc::Status CheckAuthorization(const grpc::ServerContext* context) {
    const char* expected_key = std::getenv("LOG_SERVICE_API_KEY");

    if (!expected_key) {
        return grpc::Status(grpc::StatusCode::INTERNAL, "API key not configured on server");
    }

    auto it = context->client_metadata().find("authorization");
    if (it == context->client_metadata().end() || it->second != expected_key) {
        return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "Invalid or missing API key");
    }

    return grpc::Status::OK;
}

} // namespace auth_utils
