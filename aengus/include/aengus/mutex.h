#ifndef AENGUS_INCLUDE_AENGUS_MUTEX_H
#define AENGUS_INCLUDE_AENGUS_MUTEX_H

#include <atomic>
#include <mutex>

// 根据当前 CPU 架构，调用对应的 CPU 指令，让当前线程短暂地暂停执行（Spin-Wait Hint），降低自旋等待时的资源消耗
#if defined(__x86_64__) || defined(__i386__)
#include <immintrin.h>
#elif defined(__aarch64__) || defined(__arm__)
#include <arm_acle.h>
#endif


inline void cpu_relax() noexcept
{
#if defined(__x86_64__) || defined(__i386__)
    _mm_pause();
#elif defined(__aarch64__) || defined(__arm__)
    __yield();
#endif
}

namespace aengus{

#if defined(__linux__) && defined(__GLIBC__)

class Spinlock
{
public:
    Spinlock() noexcept
    {
        pthread_spin_init(&m_mutex, PTHREAD_PROCESS_PRIVATE);
    }

    ~Spinlock() noexcept
    {
        pthread_spin_destroy(&m_mutex);
    }

    Spinlock(const Spinlock&) = delete;
    Spinlock& operator=(const Spinlock&) = delete;

    void lock() noexcept
    {
        pthread_spin_lock(&m_mutex);
    }

    void unlock() noexcept
    {
        pthread_spin_unlock(&m_mutex);
    }

private:
    pthread_spinlock_t m_mutex{};
};

#else

class Spinlock
{
public:
    using Lock = std::lock_guard<Spinlock>;

    Spinlock() noexcept = default;
    ~Spinlock() noexcept = default;

    Spinlock(const Spinlock&) = delete;
    Spinlock& operator=(const Spinlock&) = delete;

    void lock() noexcept
    {
        while(m_mutex.test_and_set(std::memory_order_acquire))
        {
            while(m_mutex.test(std::memory_order_relaxed))
            {
                cpu_relax();
            }
        }
    }

    [[nodiscard]] bool try_lock() noexcept
    {//属性说明符nodiscard，提示编译器：如果调用者丢弃了被标记函数的返回值，编译器应当发出警告。
        return !m_mutex.test_and_set(std::memory_order_acquire);
    }

    void unlock() noexcept
    {
        m_mutex.clear(std::memory_order_release);
    }

private:
    std::atomic_flag m_mutex = ATOMIC_FLAG_INIT;
};

#endif

}// namespace aengus

#endif