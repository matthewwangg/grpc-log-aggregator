#include "utils/log_utils.h"

#include <chrono>
#include <filesystem>
#include <fstream>

#include <grpcpp/grpcpp.h>

#include "log.pb.h"
#include "utils/time_utils.h"

namespace log_utils {

grpc::Status WriteLogEntryToFile(const log::LogEntry& request) {
    std::filesystem::path directory = std::filesystem::path("log") / request.source();
    std::filesystem::create_directories(directory);

    std::string day = time_utils::FormatDay(request.timestamp());
    std::string time = time_utils::FormatTime(request.timestamp());

    std::filesystem::path file_path = directory / (day + ".log");

    std::ofstream out(file_path, std::ios::app);
    if (out.is_open()) {
        out << "[" << time << "] "
            << "[" << request.level() << "] "
            << "[" << request.source() << "] "
            << "[" << request.hostname() << "] "
            << request.message() << std::endl;
        out.close();
        return grpc::Status::OK;
    } else {
        std::cerr << "Failed to open log file!" << std::endl;
        return grpc::Status(grpc::StatusCode::INTERNAL, "Failed to open log file");
    }
}

} // namespace log_utils
