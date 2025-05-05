#include "utils/pubsub_utils.h"

namespace pubsub_utils {

LogPubSub& LogPubSub::Instance() {
    static const auto& instance = *new LogPubSub();
    return instance;
}

void LogPubSub::Publish() {

}

void LogPubSub::Subscribe() {

}

} // namespace pubsub_utils
