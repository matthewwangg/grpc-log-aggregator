#include "utils/time_utils.h"

#include <chrono>
#include <string>

#include <google/protobuf/timestamp.pb.h>

namespace time_utils {

std::string FormatDay(const google::protobuf::Timestamp& ts) {
    auto tp = std::chrono::system_clock::time_point{std::chrono::seconds{ts.seconds()}};
    auto local_time = std::chrono::zoned_time{std::chrono::current_zone(), tp};
    return std::format("{:%Y-%m-%d}", local_time);
}

std::string FormatTime(const google::protobuf::Timestamp& ts) {
    auto tp = std::chrono::system_clock::time_point{std::chrono::seconds{ts.seconds()}};
    auto truncated = std::chrono::floor<std::chrono::seconds>(tp);
    auto local_time = std::chrono::zoned_time{std::chrono::current_zone(), truncated};
    return std::format("{:%H:%M:%S}", local_time);
}

} // namespace time_utils
