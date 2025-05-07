#include "utils/pubsub_utils.h"

#include <memory>
#include <mutex>
#include <string>

namespace pubsub_utils {

LogPubSub& LogPubSub::Instance() {
    static auto& instance = *new LogPubSub();
    return instance;
}

void LogPubSub::Publish(const std::string& source, const std::string& message) {
    std::lock_guard<std::mutex> lock(mu_);
    auto& queues = subscribers_[source];
    for(auto queue : queues) {
        {
            std::lock_guard<std::mutex> q_lock(queue->mu_);
            queue->messages_.push(message);
        }
        queue->cv_.notify_one();
    }
}

std::shared_ptr<LogPubSub::SubscriberQueue> LogPubSub::Subscribe(const std::string& source) {
    auto queue = std::make_shared<LogPubSub::SubscriberQueue>();

    std::lock_guard<std::mutex> lock(mu_);
    subscribers_[source].push_back(queue);

    return queue;
}

} // namespace pubsub_utils
