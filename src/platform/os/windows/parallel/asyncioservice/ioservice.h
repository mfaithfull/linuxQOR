// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_IOSERVICE
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_IOSERVICE

#include <cstdint>
#include <chrono>

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/parallel/task/cancellation/cancellationtoken.h"
#include "src/platform/os/windows/common/handles/handle.h"
#include "sync/iocompletionport.h"

namespace qor { namespace async { namespace win {

    class TimerThreadState;
    class ScheduleOperation;
    class TimedScheduleOperation;

    class IOService final
    {
    public:

        IOService();
        IOService(std::uint32_t concurrencyHint);
        ~IOService();
        IOService(IOService&& other) = delete;
        IOService(const IOService& other) = delete;
        IOService& operator=(IOService&& other) = delete;
        IOService& operator=(const IOService& other) = delete;

        [[nodiscard]]
        ScheduleOperation Schedule() noexcept;

        std::uint64_t ProcessEvents();
        std::uint64_t ProcessPendingEvents();
        std::uint64_t ProcessOneEvent();
        std::uint64_t ProcessOnePendingEvent();

        void Stop() noexcept;
        void Reset();
        bool IsStopRequested() const noexcept;
        void NotifyWorkStarted() noexcept;
        void NotifyWorkFinished() noexcept;
        const platform::win::Handle& NativeIOCPHandle() noexcept;
        bool Enroll(io::IODescriptor& ioDescriptor) const;

    private:

        friend class TimerThreadState;
        friend class ScheduleOperation;
        friend class TimedScheduleOperation;

        void ScheduleImpl(ScheduleOperation* operation) noexcept;
        void TryRescheduleOverflowOperations() noexcept;
        bool TryEnterEventLoop() noexcept;
        void ExitEventLoop() noexcept;
        bool TryProcessOneEvent(bool waitForEvent);
        void PostWakeUpEvent() noexcept;
        TimerThreadState* EnsureTimerThreadStarted();

        static constexpr std::uint32_t stop_requested_flag = 1;
        static constexpr std::uint32_t active_thread_count_increment = 2;

        // Bit 0: stop_requested_flag
		// Bit 1-31: count of active threads currently running the event loop
        std::atomic<std::uint32_t> m_threadState;
        std::atomic<std::uint32_t> m_workCount;

        ref_of<IOCompletionPort>::type m_IOCP;

        std::atomic<ScheduleOperation*> m_scheduleOperations;
        std::atomic<TimerThreadState*> m_timerState;

        template<typename REP, typename RATIO>
        [[nodiscard]]
        TimedScheduleOperation ScheduleAfter( const std::chrono::duration<REP, RATIO>& duration, CancellationToken cancellationToken = {}) noexcept
        {
            return TimedScheduleOperation{
                *this,
                std::chrono::high_resolution_clock::now() + duration,
                std::move(cancellationToken)
            };
        }
    };

}}}//qor::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_IOSERVICE
