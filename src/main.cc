#include <grpcpp/grpcpp.h>
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "log_service_impl.h"

int main(int argc, char* argv[]) {
    grpc::ServerBuilder builder;

    const char* portEnvironmentVariable = std::getenv("PORT");
    std::string port = portEnvironmentVariable ? portEnvironmentVariable : "50051";
    std::string address_url = "0.0.0.0:" + port;
    LogServiceImpl::LogMode mode = LogServiceImpl::LogMode::ASYNC;

    if (argc == 3) {
        address_url = argv[1];
        std::string mode_string = argv[2];

        if (mode_string == "async") {
            mode = LogServiceImpl::LogMode::ASYNC;
        } else if (mode_string == "sync") {
            mode = LogServiceImpl::LogMode::SYNC;
        } else {
            std::cout << "usage: " << argv[0] << " <address> <sync/async>" << std::endl;
            return 1;
        }
    }

    LogServiceImpl log_service(mode);
    builder.RegisterService(&log_service);

    builder.AddListeningPort(address_url, grpc::InsecureServerCredentials());

    std::unique_ptr<grpc::Server> server = builder.BuildAndStart();
    std::cout << "server listening on " << address_url << std::endl;

    server->Wait();
}
