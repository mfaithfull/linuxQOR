// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_SCHEDULEOPERATION
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_SCHEDULEOPERATION

#include <coroutine>

namespace qor { namespace io { namespace async { namespace win {

    class IOService;

    class ScheduleOperation
    {
    public:

        ScheduleOperation(IOService& service) noexcept : m_service(service){ }

        bool await_ready() const noexcept
        {
            return false;
        }

        void await_suspend(std::coroutine_handle<> awaiter) noexcept;

        void await_resume() const noexcept {}

    private:

        friend class IOService;
        friend class TimedScheduleOperation;
        friend class TimerThreadState;

        IOService& m_service;
        std::coroutine_handle<> m_awaiter;
        ScheduleOperation* m_next;
    };

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_SCHEDULEOPERATION
