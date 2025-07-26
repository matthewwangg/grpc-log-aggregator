#ifndef GRPC_LOG_AGGREGATOR_THREAD_UTILS_H
#define GRPC_LOG_AGGREGATOR_THREAD_UTILS_H

#include <functional>

namespace thread_utils {

void AsyncExecute(std::function<void()> task);

} // namespace thread_utils

#endif //GRPC_LOG_AGGREGATOR_THREAD_UTILS_H
