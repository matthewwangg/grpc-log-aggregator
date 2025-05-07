#ifndef GRPC_LOG_AGGREGATOR_PUBSUB_UTILS_H
#define GRPC_LOG_AGGREGATOR_PUBSUB_UTILS_H

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

namespace pubsub_utils {

class LogPubSub {
public:
    struct SubscriberQueue {
        std::queue<log::LogEntry> entries_;
        std::mutex mu_;
        std::condition_variable cv_;
    };

    static LogPubSub& Instance();

    void Publish(const log::LogEntry& entry);
    std::shared_ptr<SubscriberQueue> Subscribe(const std::string& source);

private:
    LogPubSub() = default;

    std::mutex mu_;
    std::unordered_map<std::string, std::vector<std::shared_ptr<SubscriberQueue>>> subscribers_;
};

} // namespace pubsub_utils

#endif //GRPC_LOG_AGGREGATOR_PUBSUB_UTILS_H
