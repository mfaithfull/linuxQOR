// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_TIMERQUEUE
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_TIMERQUEUE

#include <chrono>
#include <vector>

namespace qor { namespace io { namespace async { namespace win {

    class TimedScheduleOperation;

    class TimerQueue final
    {
    public:

        using TimePoint = std::chrono::high_resolution_clock::time_point;

        TimerQueue() noexcept;
        ~TimerQueue();

        bool IsEmpty() const noexcept;

        TimePoint EarliestDueTime() const noexcept;
        void EnqueueTimer(TimedScheduleOperation* timer) noexcept;
        void DequeueDueTimers(TimePoint currentTime, TimedScheduleOperation*& timerList) noexcept;
        void RemoveCancelledTimers(TimedScheduleOperation*& timerList) noexcept;

    private:

        struct TimerEntry
        {
            TimerEntry(TimedScheduleOperation* timer);

            TimePoint m_dueTime;
            TimedScheduleOperation* m_timer;
        };

        static bool CompareEntries(const TimerEntry& a, const TimerEntry& b) noexcept
        {
            return a.m_dueTime > b.m_dueTime;
        }

        //A heap sorted list of active timer entries
        std::vector<TimerEntry> m_timerEntries;

        //link list of overflow
        TimedScheduleOperation* m_overflowTimers;
    };

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_TIMERQUEUE