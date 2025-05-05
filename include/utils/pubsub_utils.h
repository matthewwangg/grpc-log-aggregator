#ifndef GRPC_LOG_AGGREGATOR_PUBSUB_UTILS_H
#define GRPC_LOG_AGGREGATOR_PUBSUB_UTILS_H

#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

namespace pubsub_utils {

class LogPubSub {
public:
    static LogPubSub& Instance();

    void Publish(const std::string& source, const std::string& message);
    std::shared_ptr<std::queue<std::string>> Subscribe(const std::string& source);

private:
    LogPubSub() = default;

    std::mutex mu_;
    std::unordered_map<std::string, std::vector<std::shared_ptr<std::queue<std::string>>>> subscribers_;
};

} // namespace pubsub_utils

#endif //GRPC_LOG_AGGREGATOR_PUBSUB_UTILS_H
