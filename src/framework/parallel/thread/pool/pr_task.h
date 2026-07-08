// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_THREADPOOL_PRTASK
#define QOR_PP_H_FRAMEWORK_THREADPOOL_PRTASK

namespace qor { namespace thread{

    using priority_t = std::int8_t;     //A type used to indicate the priority of a task. Defined to be a signed integer with a width of exactly 8 bits (-128 to +127).

    enum pr : priority_t                //An enum containing some pre-defined priorities for convenience.
    {
        lowest = -128,
        low = -64,
        normal = 0,
        high = +64,
        highest = +127
    };
    
    struct [[nodiscard]] pr_task        //A helper struct to store a task with an assigned priority.
    {
        explicit pr_task(task_t&& task_, const priority_t priority_ = 0) noexcept(std::is_nothrow_move_constructible_v<task_t>) : task(std::move(task_)), priority(priority_) {}

        [[nodiscard]] friend bool operator<(const pr_task& lhs, const pr_task& rhs) noexcept
        {
            return lhs.priority < rhs.priority;
        }

        task_t task;
        priority_t priority = 0;
    };

}}//qor::thread

#endif//QOR_PP_H_FRAMEWORK_THREADPOOL_PRTASK
