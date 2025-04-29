#ifndef GRPC_LOG_AGGREGATOR_TIME_UTILS_H
#define GRPC_LOG_AGGREGATOR_TIME_UTILS_H

#include <string>

#include <google/protobuf/timestamp.pb.h>

namespace time_utils {

std::string FormatDay(const google::protobuf::Timestamp& ts);
std::string FormatTime(const google::protobuf::Timestamp& ts);

} // namespace time_utils

#endif //GRPC_LOG_AGGREGATOR_TIME_UTILS_H
