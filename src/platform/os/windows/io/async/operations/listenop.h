// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_LISTENOP
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_LISTENOP

#include <system_error>
#include <coroutine>
#include "iocpawaiter.h"

namespace qor { namespace io { namespace async { namespace win {

    class ListenOperation
    {
    public:

        ListenOperation(int fd, byte* buffer, int32_t len)
        { }

        auto operator co_await()
        {
            return IOCPAwaiter{};
        }

    };

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCP_LISTENOP