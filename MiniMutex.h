#include <atomic>

#ifndef MINIMUTEX_H
#define MINIMUTEX_H
namespace NTE {
    class MiniMutex {
        std::atomic<bool> flag{ false };

    public:
        void lock();
        void unlock();
    };
}
#endif