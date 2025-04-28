#include <filesystem>
#include <fstream>

#include "log_service_impl.h"

grpc::Status LogServiceImpl::SendLog(grpc::ServerContext* context, const log::LogEntry* request, log::LogResponse* response) {
    std::filesystem::path file_path = std::filesystem::path("log") / "server.log";
    std::filesystem::create_directories(file_path.parent_path());

    std::ofstream out(file_path, std::ios::app);
    if (out.is_open()) {
        out << "[" << request->timestamp() << "] "
            << "[" << request->level() << "] "
            << "[" << request->source() << "] "
            << "[" << request->hostname() << "] "
            << request->message() << std::endl;
        out.close();
    } else {
        std::cerr << "Failed to open log file!" << std::endl;
        response->set_success(false);
        return grpc::Status(grpc::StatusCode::INTERNAL, "Failed to open log file");
    }

    response->set_success(true);
    return grpc::Status::OK;
}