#include <filesystem>
#include <fstream>

#include "log_service_impl.h"
#include "utils/time_utils.h"

grpc::Status LogServiceImpl::SendLog(grpc::ServerContext* context, const log::LogEntry* request, log::LogResponse* response) {
    std::filesystem::path directory = std::filesystem::path("log") / request->source();
    std::filesystem::create_directories(directory);

    std::string day = time_utils::FormatDay(request->timestamp());
    std::string time = time_utils::FormatTime(request->timestamp());

    std::filesystem::path file_path = directory / (day + ".log");

    std::ofstream out(file_path, std::ios::app);
    if (out.is_open()) {
        out << "[" << time << "] "
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