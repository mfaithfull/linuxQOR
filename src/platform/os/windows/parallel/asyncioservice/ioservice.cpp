// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "ioservice.h"
#include "operations/scheduleoperation.h"
#include "timer/timerthreadstate.h"
#include "src/framework/parallel/sync/onscopeexit.h"
#include "sync/iocompletionport.h"
#include "iostate.h"
#include "src/platform/os/windows/common/constants.h"
#include "src/platform/os/windows/api_layer/kernel/kernel32.h"

namespace qor { namespace io { namespace async { namespace win {

    IOService::IOService() : IOService(0){ }

    IOService::IOService(std::uint32_t concurrencyHint)
        : m_threadState(0),
        m_workCount(0),
        m_IOCP(new_ref<IOCompletionPort>(concurrencyHint)),
        m_scheduleOperations(nullptr),
        m_timerState(nullptr)
    { }

    IOService::~IOService()
    {
        assert(m_scheduleOperations.load(std::memory_order_relaxed)== nullptr);
        assert(m_threadState.load(std::memory_order_relaxed) < active_thread_count_increment);

        delete m_timerState.load(std::memory_order_relaxed);
    }

    ScheduleOperation IOService::Schedule() noexcept
    {
        return ScheduleOperation(*this);
    }

    std::uint64_t IOService::ProcessEvents()
    {
        std::uint64_t eventCount = 0;
        if(TryEnterEventLoop())
        {
            auto exitLoop = on_scope_exit([&] {ExitEventLoop();});

            constexpr bool waitForEvent = true;
            while(TryProcessOneEvent(waitForEvent))
            {
                ++eventCount;
            }
        }
        return eventCount;
    }

    std::uint64_t IOService::ProcessPendingEvents()
    {
        std::uint64_t eventCount = 0;
        if(TryEnterEventLoop())
        {
            auto exitLoop = on_scope_exit([&] {ExitEventLoop();});

            constexpr bool waitForEvent = false;
            while(TryProcessOneEvent(waitForEvent))
            {
                ++eventCount;
            }
        }
        return eventCount;
    }

    std::uint64_t IOService::ProcessOneEvent()
    {
        std::uint64_t eventCount = 0;
        if(TryEnterEventLoop())
        {
            auto exitLoop = on_scope_exit([&] {ExitEventLoop();});

            constexpr bool waitForEvent = true;
            if(TryProcessOneEvent(waitForEvent))
            {
                ++eventCount;
            }
        }
        return eventCount;
    }

    std::uint64_t IOService::ProcessOnePendingEvent()
    {
        std::uint64_t eventCount = 0;
        if(TryEnterEventLoop())
        {
            auto exitLoop = on_scope_exit([&]{ExitEventLoop();});

            constexpr bool waitForEvent = false;
            if(TryProcessOneEvent(waitForEvent))
            {
                ++eventCount;
            }
        }
        return eventCount;
    }

    void IOService::Stop() noexcept
    {
        const auto oldState = m_threadState.fetch_or(stop_requested_flag, std::memory_order_release);
        if((oldState & stop_requested_flag) == 0)
        {
            for(auto activeThreadCount = oldState / active_thread_count_increment;
                activeThreadCount > 0;
                --activeThreadCount)
            {
                PostWakeUpEvent();
            }
        }
    }

    void IOService::Reset()
    {
      	const auto oldState = m_threadState.fetch_and(~stop_requested_flag, std::memory_order_relaxed);
        // Check that there were no active threads running the event loop.
	    assert(oldState == stop_requested_flag);
    }

    bool IOService::IsStopRequested() const noexcept
    {
    	return (m_threadState.load(std::memory_order_acquire) & stop_requested_flag) != 0;
    }

    void IOService::NotifyWorkStarted() noexcept
    {
	    m_workCount.fetch_add(1, std::memory_order_relaxed);
    }

    void IOService::NotifyWorkFinished() noexcept
    {
	    if (m_workCount.fetch_sub(1, std::memory_order_relaxed) == 1)
	    {
		    Stop();
	    }
    }

    const platform::win::Handle& IOService::NativeIOCPHandle() noexcept
    {
        return m_IOCP->Handle();
    }

    void IOService::ScheduleImpl(ScheduleOperation* operation) noexcept
    {
        if(!m_IOCP->PostQueuedCompletionStatus(0, reinterpret_cast<unsigned long long>(operation->m_awaiter.address()), nullptr))
        {
            auto* head = m_scheduleOperations.load(std::memory_order_acquire);
            do
            {
                operation->m_next = head;
            } while (!m_scheduleOperations.compare_exchange_weak(
                head,
                operation,
                std::memory_order_release,
                std::memory_order_acquire));
        }
    }

    void IOService::TryRescheduleOverflowOperations() noexcept
    {
        auto* operation = m_scheduleOperations.exchange(nullptr, std::memory_order_acquire);
        while(operation != nullptr)
        {
            auto* next = operation->m_next;
            if(!m_IOCP->PostQueuedCompletionStatus(0, reinterpret_cast<unsigned long long>(operation->m_awaiter.address()),nullptr))
            {
                auto* tail = operation;
                while(tail->m_next != nullptr)
                {
                    tail = tail->m_next;
                }

                ScheduleOperation* head = nullptr;
                while(!m_scheduleOperations.compare_exchange_weak(
                    head,
                    operation,
                    std::memory_order_release,
                    std::memory_order_relaxed))
                {
                    tail->m_next= head;
                }
                return;
            }
            operation = next;
        }
    }

    bool IOService::TryEnterEventLoop() noexcept
    {
        auto currentState = m_threadState.load(std::memory_order_relaxed);
        do
        {
            if((currentState & stop_requested_flag) != 0)
            {
                return false;
            }
        } while (!m_threadState.compare_exchange_weak(
            currentState,
            currentState + active_thread_count_increment,
            std::memory_order_relaxed));
        return true;
    }

    void IOService::ExitEventLoop() noexcept
    {
        m_threadState.fetch_sub(active_thread_count_increment, std::memory_order_relaxed);
    }

    bool IOService::TryProcessOneEvent(bool waitForEvent)
    {
        if(IsStopRequested())
        {
            return false;
        }

        const unsigned long timeout = waitForEvent ? Infinite_Timeout : 0;

        while(true)
        {
            TryRescheduleOverflowOperations();

            unsigned long numberofBytesTransferred = 0;
            unsigned long long completionKey = 0;
            struct overlapped* overlapped = nullptr;
            bool ok = m_IOCP->GetQueuedCompletionStatus(numberofBytesTransferred, completionKey, overlapped, timeout);

            if(overlapped != nullptr)
            {
                unsigned long errorCode = ok ? Success : qor::win::api::Kernel32::GetLastError();

                auto* state = static_cast<io_state*>(reinterpret_cast<struct overlapped*>(overlapped));
                if(state && state->m_callback && state->m_callback != ((io_state::callback_type*)0xdddddddddddddddd) && state->m_callback != ((io_state::callback_type*)0xcdcdcdcdcdcdcdcd))
                {
                    state->m_callback( state, errorCode, numberofBytesTransferred, completionKey);
                }

                return true;
            }
            else if(ok)
            {
                if(completionKey != 0)
                {
                    std::coroutine_handle<>::from_address(
                        reinterpret_cast<void*>(completionKey)).resume();
                    return true;
                }
                if(IsStopRequested())
                {
                    return false;
                }
            }
            else
            {
                unsigned long errorCode = qor::win::api::Kernel32::GetLastError();
                if(errorCode == WAIT_TIMEOUT)
                {
                    return false;
                }

                continuable("Error retrieving item from io_service queue: GetQueuedCompletionStatus");
            }
        }
    }

    void IOService::PostWakeUpEvent() noexcept
    {
        m_IOCP->PostQueuedCompletionStatus(0,0,nullptr);
    }

    TimerThreadState* IOService::EnsureTimerThreadStarted()
    {
        auto* timerState = m_timerState.load(std::memory_order_acquire);
        if(timerState == nullptr)
        {
            auto newTimerState = std::make_unique<TimerThreadState>();
            if(m_timerState.compare_exchange_strong(
                timerState,
                newTimerState.get(),
                std::memory_order_release,
                std::memory_order_acquire))
            {
                // We managed to install our timer_thread_state before some
			    // other thread did, don't free it here - it will be freed in
			    // the IOService destructor.
                timerState = newTimerState.release();
            }
        }
        return timerState;
    }

    bool IOService::Enroll(io::IODescriptor& ioDescriptor) const
    {
        return m_IOCP->Enroll(ioDescriptor);
    }

}}}}//qor::io::async::win