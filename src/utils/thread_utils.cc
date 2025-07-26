#include "thread_utils.h"

#include <functional>
#include <thread>

namespace thread_utils {

void AsyncExecute(std::function<void()> task) {
    std::thread(std::move(task)).detach();
}

} // namespace thread_utils
