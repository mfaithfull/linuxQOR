// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include "timerqueue.h"
#include "../operations/timedscheduleoperation.h"

namespace qor { namespace io { namespace async { namespace win {

    TimerQueue::TimerEntry::TimerEntry(TimedScheduleOperation* timer) :
                m_dueTime(timer->m_resumeTime), m_timer(timer){ }

    TimerQueue::TimerQueue() noexcept
        : m_timerEntries(), m_overflowTimers(nullptr){ }

    TimerQueue::~TimerQueue()
    {
        assert(IsEmpty());
    }

    bool TimerQueue::IsEmpty() const noexcept
    {
        return m_timerEntries.empty() && m_overflowTimers == nullptr;
    }

    std::chrono::high_resolution_clock::time_point TimerQueue::EarliestDueTime() const noexcept
    {
        if(!m_timerEntries.empty())
        {
            if(m_overflowTimers != nullptr)
            {
                return std::min(
                    m_timerEntries.front().m_dueTime,
                    m_overflowTimers->m_resumeTime
                );
            }

            return m_timerEntries.front().m_dueTime;
        }
        else if(m_overflowTimers != nullptr)
        {
            return m_overflowTimers->m_resumeTime;
        }

        return TimePoint::max();
    }

    void TimerQueue::EnqueueTimer(TimedScheduleOperation* timer) noexcept
    {
        try
        {
            m_timerEntries.emplace_back(timer);
            std::push_heap(m_timerEntries.begin(), m_timerEntries.end(), CompareEntries);
        }
        catch(...)
        {
            const auto& newDueTime = timer->m_resumeTime;
            auto** current = &m_overflowTimers;
            while((*current) != nullptr && (*current)->m_resumeTime <= newDueTime)
            {
                current = &(*current)->m_next;
            }
            timer->m_next = *current;
            *current = timer;
        }

    }

    void TimerQueue::DequeueDueTimers(TimePoint currentTime, TimedScheduleOperation*& timerList) noexcept
    {
        while(!m_timerEntries.empty() && m_timerEntries.front().m_dueTime <= currentTime)
        {
            auto* timer = m_timerEntries.front().m_timer;
            std::pop_heap(m_timerEntries.begin(), m_timerEntries.end(), CompareEntries);
            m_timerEntries.pop_back();
            timer->m_next = timerList;
            timerList = timer;
        }

        while(m_overflowTimers != nullptr && m_overflowTimers->m_resumeTime <= currentTime)
        {
            auto* timer = m_overflowTimers;
            m_overflowTimers = timer->m_next;
            timer->m_next = timerList;
            timerList = timer;
        }
    }

    void TimerQueue::RemoveCancelledTimers(TimedScheduleOperation*& timerList) noexcept
    {
        //linear scan checking for cancellation requests

        const auto AddTimerToList = [&](TimedScheduleOperation* timer)
            {
                timer->m_next = timerList;
                timerList = timer;
            };

        const auto IsTimerCancelled = [](const TimerEntry& entry)
            {
                return entry.m_timer->m_cancellationToken.is_cancellation_requested();
            };

        auto firstCancelledEntry = std::find_if(
            m_timerEntries.begin(),
            m_timerEntries.end(),
            IsTimerCancelled);

        if(firstCancelledEntry != m_timerEntries.end())
        {
            auto nonCancelledEnd = firstCancelledEntry;
            AddTimerToList(nonCancelledEnd->m_timer);

            for(auto iter = firstCancelledEntry + 1;
                iter != m_timerEntries.end();
                ++iter)
            {
                if(IsTimerCancelled(*iter))
                {
                    AddTimerToList(iter->m_timer);
                }
                else
                {
                    *nonCancelledEnd++ = std::move(*iter);
                }
            }

            m_timerEntries.erase(nonCancelledEnd, m_timerEntries.end());

            std::make_heap( m_timerEntries.begin(), m_timerEntries.end(), CompareEntries);
        }

        {
            TimedScheduleOperation** current = &m_overflowTimers;
            while((*current) != nullptr)
            {
                auto* timer = (*current);
                if(timer->m_cancellationToken.is_cancellation_requested())
                {
                    *current = timer->m_next;
                    AddTimerToList(timer);
                }
                else
                {
                    current = &timer->m_next;
                }
            }
        }
    }

}}}}//qor::io::async::win
