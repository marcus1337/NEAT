#include "MiniMutex.h"

void MiniMutex::lock()
{
    while (flag.exchange(true, std::memory_order_relaxed));
    std::atomic_thread_fence(std::memory_order_acquire);
}

void MiniMutex::unlock()
{
    std::atomic_thread_fence(std::memory_order_release);
    flag.store(false, std::memory_order_relaxed);
}