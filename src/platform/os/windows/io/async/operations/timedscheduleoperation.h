// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_TIMEDSCHEDULEOPERATION
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_TIMEDSCHEDULEOPERATION

#include <coroutine>
#include <chrono>
#include <optional>
#include <atomic>
#include "src/framework/parallel/task/cancellation/cancellationtoken.h"
#include "src/framework/parallel/task/cancellation/cancellationregistration.h"
#include "scheduleoperation.h"

namespace qor { namespace io { namespace async { namespace win {

    class IOService;
    
    class TimedScheduleOperation
    {
    public:

        TimedScheduleOperation(
            IOService& service,
            std::chrono::high_resolution_clock::time_point resumetime,
            CancellationToken cancellationToken) noexcept;

        TimedScheduleOperation(TimedScheduleOperation&& other) noexcept;
        ~TimedScheduleOperation();

        TimedScheduleOperation& operator=(TimedScheduleOperation&& other) = delete;
        TimedScheduleOperation(const TimedScheduleOperation& other) = delete;
        TimedScheduleOperation& operator=(const TimedScheduleOperation& other) = delete;

        bool await_ready() const noexcept;
        void await_suspend(std::coroutine_handle<> awaiter);
        void await_resume();

    private:

        friend class TimerQueue;
        friend class TimerThreadState;

        ScheduleOperation m_scheduleOperation;
        std::chrono::high_resolution_clock::time_point m_resumeTime;
        CancellationToken m_cancellationToken;
        std::optional<CancellationRegistration> m_cancellationRegistration;
        TimedScheduleOperation* m_next;
        std::atomic<std::uint32_t> m_refCount;
    };

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_TIMEDSCHEDULEOPERATION
