#include "utils/pubsub_utils.h"

#include <memory>
#include <mutex>
#include <queue>
#include <string>

namespace pubsub_utils {

LogPubSub& LogPubSub::Instance() {
    static auto& instance = *new LogPubSub();
    return instance;
}

void LogPubSub::Publish(const std::string& source, const std::string& message) {
    mu_.lock();
    auto& queues = subscribers_[source];
    for(auto queue : queues) {
        queue->push(message);
    }
    mu_.unlock();
}

std::shared_ptr<std::queue<std::string>> LogPubSub::Subscribe(const std::string& source) {
    auto queue = std::make_shared<std::queue<std::string>>();

    mu_.lock();
    subscribers_[source].push_back(queue);
    mu_.unlock();

    return queue;
}

} // namespace pubsub_utils
