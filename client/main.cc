#include <iostream>
#include <string>

#include <grpcpp/grpcpp.h>
#include <google/protobuf/timestamp.pb.h>

#include "log.grpc.pb.h"
#include "log.pb.h"

void ExampleQueryLog(std::shared_ptr<log::LogService::Stub> stub, std::unique_ptr<grpc::ClientContext> context) {
    log::QueryRequest request;

    request.set_source("client-test");
    request.set_date("2025-04-29");
    request.set_keyword("");

    log::QueryResponse response;

    grpc::Status status = stub->QueryLog(context.get(), request, &response);

    if (status.ok()) {
        std::cout << "QueryLog succeeded! Found " << response.entries_size() << " entries:" << std::endl;
        for (const auto& entry : response.entries()) {
            std::cout << "[" << entry.level() << "] "
                      << entry.message() << " @ "
                      << entry.timestamp().seconds() << " (host: "
                      << entry.hostname() << ")" << std::endl;
        }
    } else {
        std::cout << "QueryLog failed with error code: " << status.error_code()
                  << ", message: " << status.error_message() << std::endl;
    }
}

void ExampleSendLog(std::shared_ptr<log::LogService::Stub> stub, std::unique_ptr<grpc::ClientContext> context) {
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

    grpc::Status status = stub->SendLog(context.get(), request, &response);

    if (status.ok()) {
        std::cout << "SendLog succeeded!" << std::endl;
    } else {
        std::cout << "SendLog failed with error code: " << status.error_code()
                  << ", message: " << status.error_message() << std::endl;
    }
}

void ExampleStreamLog(std::shared_ptr<log::LogService::Stub> stub, std::unique_ptr<grpc::ClientContext> context) {
    google::protobuf::Timestamp ts;

    log::LogEntry request;
    request.set_level("INFO");
    request.set_source("client-test");
    request.set_hostname("127.0.0.1");

    log::LogResponse response;

    std::unique_ptr<grpc::ClientWriter<log::LogEntry>> writer = stub->StreamLog(context.get(), &response);

    for (int i = 0; i < 5; i++) {
        auto now = std::chrono::system_clock::now();
        ts.set_seconds(std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count());

        *request.mutable_timestamp() = ts;
        request.set_message("Stream test " + std::to_string(i));

        writer->Write(request);
    }
    writer->WritesDone();

    grpc::Status status = writer->Finish();

    if (status.ok()) {
        std::cout << "StreamLog succeeded!" << std::endl;
    } else {
        std::cout << "StreamLog failed with error code: " << status.error_code()
                  << ", message: " << status.error_message() << std::endl;
    }
}

void ExampleSubscribeLog(std::shared_ptr<log::LogService::Stub> stub, std::unique_ptr<grpc::ClientContext> context) {
    log::QueryRequest request;

    request.set_source("client-test");

    std::unique_ptr<grpc::ClientReader<log::LogEntry>> reader = stub->SubscribeLog(context.get(), request);

    log::LogEntry entry;
    std::cout << "Subscribed. Waiting for log entries..." << std::endl;

    while (reader->Read(&entry)) {
        std::cout << "[" << entry.level() << "] "
                  << entry.message() << " @ "
                  << entry.timestamp().seconds() << " (host: "
                  << entry.hostname() << ")" << std::endl;
    }

    grpc::Status status = reader->Finish();
    if (!status.ok()) {
        std::cerr << "SubscribeLog failed with error code: " << status.error_code()
                  << ", message: " << status.error_message() << std::endl;
    } else {
        std::cout << "SubscribeLog stream ended." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::string server_address = "127.0.0.1:50051";
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());

    std::shared_ptr<log::LogService::Stub> stub = log::LogService::NewStub(channel);

    std::unique_ptr<grpc::ClientContext> send_log_context = std::make_unique<grpc::ClientContext>();
    send_log_context->AddMetadata("authorization", std::getenv("LOG_SERVICE_API_KEY"));
    ExampleSendLog(stub, std::move(send_log_context));

    std::unique_ptr<grpc::ClientContext> stream_log_context = std::make_unique<grpc::ClientContext>();
    stream_log_context->AddMetadata("authorization", std::getenv("LOG_SERVICE_API_KEY"));
    ExampleStreamLog(stub, std::move(stream_log_context));

    std::unique_ptr<grpc::ClientContext> query_log_context = std::make_unique<grpc::ClientContext>();
    query_log_context->AddMetadata("authorization", std::getenv("LOG_SERVICE_API_KEY"));
    ExampleQueryLog(stub, std::move(query_log_context));

    std::unique_ptr<grpc::ClientContext> subscribe_log_context = std::make_unique<grpc::ClientContext>();
    subscribe_log_context->AddMetadata("authorization", std::getenv("LOG_SERVICE_API_KEY"));
    ExampleSubscribeLog(stub, std::move(subscribe_log_context));
}
