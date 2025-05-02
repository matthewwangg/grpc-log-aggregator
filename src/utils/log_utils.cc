#include "utils/log_utils.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <vector>

#include <google/protobuf/util/time_util.h>
#include <grpcpp/grpcpp.h>

#include "log.pb.h"
#include "utils/time_utils.h"

namespace log_utils {

bool ParseLogLine(const std::string& line, log::LogEntry& entry, const std::string& date) {
    size_t p1 = line.find(']');
    size_t p2 = line.find(']', p1 + 1);
    size_t p3 = line.find(']', p2 + 1);
    size_t p4 = line.find(']', p3 + 1);

    if (p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos || p4 == std::string::npos) {
        return false;
    }

    std::string time   = line.substr(1, p1 - 1);
    std::string level  = line.substr(p1 + 3, p2 - p1 - 3);
    std::string source = line.substr(p2 + 3, p3 - p2 - 3);
    std::string host   = line.substr(p3 + 3, p4 - p3 - 3);
    std::string message = line.substr(p4 + 2);

    std::string datetime = date + "T" + time + "Z";
    google::protobuf::Timestamp ts;
    if (!google::protobuf::util::TimeUtil::FromString(datetime, &ts)) {
        return false;
    }

    *entry.mutable_timestamp() = ts;
    entry.set_level(level);
    entry.set_source(source);
    entry.set_hostname(host);
    entry.set_message(message);

    return true;
}

grpc::Status ReadLogFileToEntries(const std::filesystem::path& file_path, const std::string& keyword, std::vector<log::LogEntry>& entries) {
    std::ifstream in(file_path);
    std::string date = file_path.stem().string();

    if (!in.is_open()) {
        std::cerr << "Failed to open log file!" << std::endl;
        return grpc::Status(grpc::StatusCode::INTERNAL, "Failed to open log file");
    }

    std::string line;
    while (std::getline(in, line)) {
        log::LogEntry entry;
        if (!ParseLogLine(line, entry, date)) {
            continue;
        }
        if (!keyword.empty() && line.find(keyword) == std::string::npos) {
            continue;
        }
        entries.push_back(entry);
    }

    return grpc::Status::OK;
}

grpc::Status WriteLogEntryToFile(const log::LogEntry& request) {
    std::filesystem::path directory = std::filesystem::path("log") / request.source();
    std::filesystem::create_directories(directory);

    std::string day = time_utils::FormatDay(request.timestamp());
    std::string time = time_utils::FormatTime(request.timestamp());

    std::filesystem::path file_path = directory / (day + ".log");

    std::ofstream out(file_path, std::ios::app);

    if (!out.is_open()) {
        std::cerr << "Failed to open log file!" << std::endl;
        return grpc::Status(grpc::StatusCode::INTERNAL, "Failed to open log file");
    }

    out << "[" << time << "] "
        << "[" << request.level() << "] "
        << "[" << request.source() << "] "
        << "[" << request.hostname() << "] "
        << request.message() << std::endl;
    out.close();

    return grpc::Status::OK;
}

} // namespace log_utils
