#ifndef GRPC_LOG_AGGREGATOR_LOG_UTILS_H
#define GRPC_LOG_AGGREGATOR_LOG_UTILS_H

#include <filesystem>
#include <vector>

#include <grpcpp/grpcpp.h>

#include "log.pb.h"

namespace log_utils {

bool ParseLogLine(const std::string& line, log::LogEntry& entry, const std::string& date);
grpc::Status ReadLogFileToEntries(const std::filesystem::path& file_path, const std::string& keyword, std::vector<log::LogEntry>& entries);
grpc::Status WriteLogEntryToFile(const log::LogEntry& entry);

} // namespace log_utils

#endif //GRPC_LOG_AGGREGATOR_LOG_UTILS_H
