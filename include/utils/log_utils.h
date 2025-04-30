#ifndef GRPC_LOG_AGGREGATOR_LOG_UTILS_H
#define GRPC_LOG_AGGREGATOR_LOG_UTILS_H

#include <grpcpp/grpcpp.h>

#include "log.pb.h"

namespace log_utils {

grpc::Status WriteLogEntryToFile(const log::LogEntry& entry);

} // namespace log_utils

#endif //GRPC_LOG_AGGREGATOR_LOG_UTILS_H
