// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "timerthreadstate.h"
#include "timerqueue.h"
#include "../operations/timedscheduleoperation.h"
#include "../ioservice.h"
#include "src/platform/os/windows/common/constants.h"
#include "src/platform/os/windows/common/structures.h"
#include "src/platform/os/windows/api_layer/kernel/kernel32.h"
#undef max

namespace qor { namespace io { namespace async { namespace win {

    TimerThreadState::TimerThreadState() :
        m_wakeUpEvent(new_ref<AutoResetEvent>()),
        m_waitableTimerEvent(new_ref<WaitableTimerEvent>()),
        m_newlyQueuedTimers(nullptr),
        m_timerCancellationRequested(false),
        m_shutDownRequested(false),
        m_thread([this]{this->Run();})
    { }

    TimerThreadState::~TimerThreadState()
    {
        m_shutDownRequested.store(true, std::memory_order_release);
        WakeUpTimerThread();
        m_thread.Join();
    }

    void TimerThreadState::RequestTimerCancellation() noexcept
    {
        const bool wastimerCancellationAlreadyRequested =
        m_timerCancellationRequested.exchange(true, std::memory_order_release);
        if(!wastimerCancellationAlreadyRequested)
        {
            WakeUpTimerThread();
        }
    }

    void TimerThreadState::Run() noexcept
    {
        using clock = std::chrono::high_resolution_clock;
        using TimePoint = clock::time_point;

        TimerQueue timerQueue;

        const unsigned long waitHandleCount = 2;
        const void* waitHandles[waitHandleCount] =
            {
                m_wakeUpEvent->Handle().Use(),
                m_waitableTimerEvent->Handle().Use()
            };

        TimePoint lastSetWaitEventTime = TimePoint::max();
        TimedScheduleOperation* timersReadyToResume = nullptr;

        unsigned long timeout = Infinite_Timeout;

        while(!m_shutDownRequested.load(std::memory_order_relaxed))
        {
            const unsigned long waitResult = qor::win::api::Kernel32::WaitForMultipleObjectsEx(
                waitHandleCount,
                waitHandles,
                0, //waitAll
                timeout,
                0);//alertable
            if(waitResult == Wait_Object0 || waitResult == Wait_Failed)
            {
       			// Wake-up event (WAIT_OBJECT_0)
                //
                // We are only woken up for:
                // - handling timer cancellation
                // - handling newly queued timers
                // - shutdown
                //
                // We also handle WAIT_FAILED here so that we remain responsive
                // to new timers and cancellation even if the OS fails to perform
                // the wait operation for some reason.

                //Handle cancelled timers

                if(m_timerCancellationRequested.exchange(false, std::memory_order_acquire))
                {
                    timerQueue.RemoveCancelledTimers(timersReadyToResume);
                }

                //Handle newly queued timers
                auto* newTimers = m_newlyQueuedTimers.exchange(nullptr, std::memory_order_acquire);
                while(newTimers != nullptr)
                {
                    auto* timer = newTimers;
                    newTimers = timer->m_next;

                    if(timer->m_cancellationToken.is_cancellation_requested())
                    {
                        timer->m_next = timersReadyToResume;
                        timersReadyToResume = timer;
                    }
                    else
                    {
                        timerQueue.EnqueueTimer(timer);
                    }
                }
            }
            else if(waitResult == (Wait_Object0 + 1))
            {
                lastSetWaitEventTime = TimePoint::max();
            }

            if(!timerQueue.IsEmpty())
            {
                TimePoint currentTime = clock::now();

                timerQueue.DequeueDueTimers(currentTime, timersReadyToResume);

                if(!timerQueue.IsEmpty())
                {
                    auto earliestDueTime = timerQueue.EarliestDueTime();
                    assert(earliestDueTime > currentTime);

                    if(earliestDueTime != lastSetWaitEventTime)
                    {
                        using ticks = std::chrono::duration<__int64, std::ratio<1, 10'000'000>>;
                        auto timeUntilNextDueTime = earliestDueTime - currentTime;

                        platform::win::LARGE_INTEGER dueTime;
                        dueTime.QuadPart = -std::chrono::duration_cast<ticks>(timeUntilNextDueTime).count();

                        const long period = 0;//no repeat on the timer

                        const bool resumeFromSuspend = false;

                        bool ok = m_waitableTimerEvent->Set(dueTime, period, resumeFromSuspend);
                        if(ok)
                        {
                            lastSetWaitEventTime = earliestDueTime;
                            timeout = Infinite_Timeout;
                        }
                        else
                        {
                            using namespace std::literals::chrono_literals;
                            if(timeUntilNextDueTime > 1s)
                            {
                                timeout = 1000;
                            }
                            else if(timeUntilNextDueTime > 1ms)
                            {
                                timeout = static_cast<unsigned long>(
                                    std::chrono::duration_cast<std::chrono::milliseconds>(
                                        timeUntilNextDueTime).count());
                            }
                            else
                            {
                                timeout = 1;
                            }
                        }
                    }
                }
            }

            //Now schedule any ready to run timers
            while(timersReadyToResume != nullptr)
            {
                auto* timer = timersReadyToResume;
                auto* nextTimer = timer->m_next;

                if(timer->m_refCount.fetch_sub(1, std::memory_order_release) == 1)
                {
                    timer->m_scheduleOperation.m_service.ScheduleImpl(
                        &timer->m_scheduleOperation);
                }

                timersReadyToResume = nextTimer;
            }
        }
    }

    void TimerThreadState::WakeUpTimerThread() noexcept
    {
        m_wakeUpEvent->Set();
    }

}}}}//qor::io::async::win
