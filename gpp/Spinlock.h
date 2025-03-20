

#ifndef GPP_SPINLOCK_H
#define GPP_SPINLOCK_H

namespace gpp{

class SpinLock {
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:

    void lock() noexcept {
        for (int tries = 0; ; ++tries) {
            if (!flag.test_and_set(std::memory_order_acquire)) return;

            if (tries < 10) {
                std::this_thread::yield();
            } else if (tries < 20) {
                std::this_thread::sleep_for(std::chrono::microseconds(1));
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(tries < 30 ? 1 : 5));
            }
        }
    }

    bool try_lock() noexcept {
        return !flag.test(std::memory_order_relaxed) &&
               !flag.test_and_set(std::memory_order_acquire);
    }

    void unlock() noexcept {
        flag.clear(std::memory_order_release);
    }
};
}
#endif
