#include <iostream>
#include <string>

#include <grpcpp/grpcpp.h>

#include "log.grpc.pb.h"
#include "log.pb.h"

int main(int argc, char* argv[]) {
    std::string server_address = "127.0.0.1:50051";
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());

    std::unique_ptr<log::LogService::Stub> stub = log::LogService::NewStub(channel);

    grpc::ClientContext context;

    log::LogEntry request;
    request.set_timestamp("2025-04-27T23:00:00Z");
    request.set_message("Test log message");
    request.set_level("INFO");
    request.set_source("client-test");
    request.set_hostname("127.0.0.1");

    log::LogResponse response;

    grpc::Status status = stub->SendLog(&context, request, &response);

    if (status.ok()) {
        std::cout << "RPC succeeded!" << std::endl;
    } else {
        std::cout << "RPC failed with error code: " << status.error_code()
                  << ", message: " << status.error_message() << std::endl;
    }
}
