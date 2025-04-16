// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef QOR_PP_H_FRAMEWORK_THREADPOOL_PRTASK
#define QOR_PP_H_FRAMEWORK_THREADPOOL_PRTASK

namespace qor { namespace framework{

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

}}//qor::framework

#endif//QOR_PP_H_FRAMEWORK_THREADPOOL_PRTASK
