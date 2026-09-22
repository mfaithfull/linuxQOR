// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PARALLEL_SIGNALTREE_BLOCKINGSTATE
#define QOR_PP_H_PARALLEL_SIGNALTREE_BLOCKINGSTATE



//TODO:
//#include <linux/futex.h>
//#include <sys/syscall.h>
//#include <unistd.h>

#include <atomic>
#include <chrono>
#include <climits>
#include <condition_variable>
#include <cstdint>
#include <ctime>
#include <mutex>

#include "processsharing.h"

namespace qor{ namespace par{ namespace detail {

    struct no_blocking_state {};

    template <process_sharing sharing>
    class blocking_state;

    struct alignas(64) process_shared_blocking_state
    {
        static_assert(std::atomic<int64_t>::is_always_lock_free);
        static_assert(std::atomic<uint32_t>::is_always_lock_free);

        std::atomic<int64_t>  nonEmptyTreeCount_{0};
        std::atomic<uint32_t> wakeSequence_{0};
        std::atomic<uint32_t> stopped_{0};
    };


    template <>
    class blocking_state<process_sharing::process_private>
    {
    public:

        void increment() noexcept;
        void decrement() noexcept;

        template <typename Clock, typename Duration>
        bool wait_until(std::chrono::time_point<Clock, Duration>);

        void stop() noexcept;
        uint64_t count() const noexcept;

    private:

        std::atomic<int64_t>  nonEmptyTreeCount_{0};
        std::mutex                 mutex_;
        std::condition_variable    conditionVariable_;
        bool                       stopped_{false};
    };


    template <>
    class blocking_state<process_sharing::process_shared>
    {
    public:

        explicit blocking_state(process_shared_blocking_state &) noexcept;

        void increment() noexcept;
        void decrement() noexcept;

        template <typename Clock, typename Duration>
        bool wait_until(std::chrono::time_point<Clock, Duration>);

        void stop() noexcept;
        uint64_t count() const noexcept;

    private:

        static void wake(std::atomic<uint32_t> &) noexcept;
        static void wait(std::atomic<uint32_t> &, uint32_t, ::timespec const *) noexcept;

        process_shared_blocking_state * state_;
    };

}}}//qor::par::detail


inline void qor::par::detail::blocking_state<qor::par::process_sharing::process_private>::increment() noexcept
{
    auto wakeBlockedThreads = nonEmptyTreeCount_.fetch_add(1, std::memory_order_release) == 0;

    if (wakeBlockedThreads)
    {
        std::lock_guard lock(mutex_);
        conditionVariable_.notify_all();
    }
}

inline void qor::par::detail::blocking_state<qor::par::process_sharing::process_private>::decrement() noexcept
{
    nonEmptyTreeCount_.fetch_sub(1, std::memory_order_release);
}


template <typename Clock, typename Duration>
inline bool qor::par::detail::blocking_state<qor::par::process_sharing::process_private>::wait_until(std::chrono::time_point<Clock, Duration> deadline)
{
    std::unique_lock lock(mutex_);

    if (stopped_)
        return false;

    if (not conditionVariable_.wait_until(lock, deadline, [this]
        {
            return (nonEmptyTreeCount_.load(std::memory_order_acquire) > 0) || stopped_;
        }))
    {
        return false;
    }

    return not stopped_;
}


inline void qor::par::detail::blocking_state<qor::par::process_sharing::process_private>::stop() noexcept
{
    {
        std::lock_guard lock(mutex_);
        stopped_ = true;
    }
    conditionVariable_.notify_all();
}


inline uint64_t qor::par::detail::blocking_state<qor::par::process_sharing::process_private>::count() const noexcept
{
    auto count = nonEmptyTreeCount_.load(std::memory_order_acquire);
    return (count > 0) ? static_cast<uint64_t>(count) : 0;
}


inline qor::par::detail::blocking_state<qor::par::process_sharing::process_shared>::blocking_state(process_shared_blocking_state & state) noexcept : state_(&state)
{
}


inline void qor::par::detail::blocking_state<qor::par::process_sharing::process_shared>::increment() noexcept
{
    auto wakeBlockedProcesses = state_->nonEmptyTreeCount_.fetch_add(1, std::memory_order_release) == 0;

    if (wakeBlockedProcesses)
    {
        state_->wakeSequence_.fetch_add(1, std::memory_order_release);
        wake(state_->wakeSequence_);
    }
}


inline void qor::par::detail::blocking_state<qor::par::process_sharing::process_shared>::decrement() noexcept
{
    state_->nonEmptyTreeCount_.fetch_sub(1, std::memory_order_release);
}


template <typename Clock, typename Duration>
inline bool qor::par::detail::blocking_state<qor::par::process_sharing::process_shared>::wait_until(std::chrono::time_point<Clock, Duration> deadline)
{
    for (;;)
    {
        if (state_->stopped_.load(std::memory_order_acquire) != 0)
            return false;

        if (state_->nonEmptyTreeCount_.load(std::memory_order_acquire) > 0)
            return true;

        auto const sequence = state_->wakeSequence_.load(std::memory_order_acquire);

        if (state_->stopped_.load(std::memory_order_acquire) != 0)
            return false;

        if (state_->nonEmptyTreeCount_.load(std::memory_order_acquire) > 0)
            return true;

        auto const now = Clock::now();
        if (now >= deadline)
            return false;

        auto const remaining = std::chrono::duration_cast<std::chrono::nanoseconds>(deadline - now);
        auto const seconds = std::chrono::duration_cast<std::chrono::seconds>(remaining);
        auto const nanoseconds = remaining - seconds;
        ::timespec timeout
        {
            static_cast<::time_t>(seconds.count()),
            static_cast<long>(nanoseconds.count())
        };

        wait(state_->wakeSequence_, sequence, &timeout);
    }
}

inline void qor::par::detail::blocking_state<qor::par::process_sharing::process_shared>::stop() noexcept
{
    state_->stopped_.store(1, std::memory_order_release);
    state_->wakeSequence_.fetch_add(1, std::memory_order_release);
    wake(state_->wakeSequence_);
}


inline uint64_t qor::par::detail::blocking_state<qor::par::process_sharing::process_shared>::count() const noexcept
{
    auto count = state_->nonEmptyTreeCount_.load(std::memory_order_acquire);
    return (count > 0) ? static_cast<uint64_t>(count) : 0;
}


inline void qor::par::detail::blocking_state<qor::par::process_sharing::process_shared>::wake(std::atomic<uint32_t> & sequence) noexcept
{
    auto * address = reinterpret_cast<uint32_t *>(&sequence);
    //TODO:
    //static_cast<void>(::syscall(SYS_futex, address, FUTEX_WAKE, INT_MAX, nullptr, nullptr, 0));
}


inline void qor::par::detail::blocking_state<qor::par::process_sharing::process_shared>::wait(std::atomic<uint32_t> & sequence, uint32_t expected, ::timespec const * timeout) noexcept
{
    auto * address = reinterpret_cast<uint32_t *>(&sequence);
    //TODO:
    //static_cast<void>(::syscall(SYS_futex, address, FUTEX_WAIT, expected, timeout, nullptr, 0));
}

#endif//QOR_PP_H_PARALLEL_SIGNALTREE_BLOCKINGSTATE
