#include <iostream>
#include <string>

#include <grpcpp/grpcpp.h>
#include <google/protobuf/timestamp.pb.h>

#include "log.grpc.pb.h"
#include "log.pb.h"

int main(int argc, char* argv[]) {
    std::string server_address = "127.0.0.1:50051";
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());

    std::unique_ptr<log::LogService::Stub> stub = log::LogService::NewStub(channel);

    grpc::ClientContext context;

    google::protobuf::Timestamp ts;
    auto now = std::chrono::system_clock::now();
    ts.set_seconds(std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count());

    log::LogEntry request;
    *request.mutable_timestamp() = ts;
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
