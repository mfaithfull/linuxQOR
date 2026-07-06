// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_TIMERTHREADSTATE
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_TIMERTHREADSTATE

#include <chrono>
#include "src/framework/parallel/thread/thread.h"
#include "src/platform/os/windows/common/handles/handle.h"
#include "waitabletimerevent.h"
#include "../sync/autoresetevent.h"

namespace qor { namespace io { namespace async { namespace win {

    class TimedScheduleOperation;
    
    class TimerThreadState final
    {
    public:

        TimerThreadState();
        ~TimerThreadState();
        TimerThreadState(const TimerThreadState& other) = delete;
        TimerThreadState& operator=(const TimerThreadState& other) = delete;

        void RequestTimerCancellation() noexcept;
        void Run() noexcept;
        void WakeUpTimerThread() noexcept;

        ref_of<AutoResetEvent>::type m_wakeUpEvent;
        ref_of<WaitableTimerEvent>::type m_waitableTimerEvent;

        std::atomic<TimedScheduleOperation*> m_newlyQueuedTimers;
        std::atomic<bool> m_timerCancellationRequested;
        std::atomic<bool> m_shutDownRequested;

        Thread m_thread;

    };

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_TIMERQUEUE