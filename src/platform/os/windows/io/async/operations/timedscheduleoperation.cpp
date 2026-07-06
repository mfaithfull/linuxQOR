// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <chrono>
#include "timedscheduleoperation.h"
#include "../ioservice.h"
#include "../timer/timerthreadstate.h"

namespace qor { namespace io { namespace async { namespace win {

    TimedScheduleOperation::TimedScheduleOperation(
        IOService& service,
        std::chrono::high_resolution_clock::time_point resumeTime,
        CancellationToken cancellationToken) noexcept :
        m_scheduleOperation(service),
        m_resumeTime(resumeTime),
        m_cancellationToken(std::move(cancellationToken)),
        m_refCount(2)
    { }

    TimedScheduleOperation::TimedScheduleOperation(
        TimedScheduleOperation&& other) noexcept :
        m_scheduleOperation(std::move(other.m_scheduleOperation)),
        m_resumeTime(std::move(other.m_resumeTime)),
        m_cancellationToken(std::move(other.m_cancellationToken)),
        m_refCount(2)
    { }

    TimedScheduleOperation::~TimedScheduleOperation() = default;

    bool TimedScheduleOperation::await_ready() const noexcept
    {
        return m_cancellationToken.is_cancellation_requested();
    }

    void TimedScheduleOperation::await_suspend(std::coroutine_handle<> awaiter)
    {
        m_scheduleOperation.m_awaiter = awaiter;
        auto& service = m_scheduleOperation.m_service;

        auto* timerState = service.EnsureTimerThreadStarted();

        if(m_cancellationToken.can_be_cancelled())
        {
            m_cancellationRegistration.emplace(m_cancellationToken, [timerState]{
                timerState->RequestTimerCancellation();
            });
        }

        auto* prev = timerState->m_newlyQueuedTimers.load(std::memory_order_acquire);
        do
        {
            m_next = prev;
        } while (!timerState->m_newlyQueuedTimers.compare_exchange_weak(
            prev,
            this,
            std::memory_order_release,
            std::memory_order_acquire));

        if(prev == nullptr)
        {
            timerState->WakeUpTimerThread();
        }

        if(m_refCount.fetch_sub(1, std::memory_order_acquire) == 1)
        {
            service.ScheduleImpl(&m_scheduleOperation);
        }

    }

    void TimedScheduleOperation::await_resume()
    {
        m_cancellationRegistration.reset();
        m_cancellationToken.throw_if_cancellation_requested();
    }

}}}}//qor::io::async::win
