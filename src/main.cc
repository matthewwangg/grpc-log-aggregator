#include <grpcpp/grpcpp.h>
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "log_service_impl.h"

int main(int argc, char* argv[]) {
    grpc::ServerBuilder builder;

    LogServiceImpl log_service;
    builder.RegisterService(&log_service);

    std::string address_url = "0.0.0.0:50051";
    builder.AddListeningPort(address_url, grpc::InsecureServerCredentials());

    std::unique_ptr<grpc::Server> server = builder.BuildAndStart();
    std::cout << "Server listening on " << address_url << std::endl;

    server->Wait();
}
