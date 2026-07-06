// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURING_SHUTDOWNOP
#define QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURING_SHUTDOWNOP

#include <system_error>
#include <coroutine>
#include "iouringawaiter.h"

namespace qor{ namespace io{ namespace async{ namespace lin{

    class ShutdownOperation
    {
    public:

        ShutdownOperation(IOUring &uring, int fd, int how) : m_sqe(uring.GetSQE())
        {
            m_sqe.PrepareShutdown(fd, how);
            m_ring = &uring;
        }

        auto operator co_await()
        {
            return IOUringAwaiter{m_sqe, m_ring};
        }

    private:
        IOUring* m_ring;
        IOUring::SQE m_sqe;
    };

}}}}//qor::io::async::lin

#endif//QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURING_SHUTDOWNOP