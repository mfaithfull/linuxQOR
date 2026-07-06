// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURING_SENDOP
#define QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURING_SENDOP

#include <system_error>
#include <coroutine>
#include "iouringawaiter.h"

namespace qor{ namespace io{ namespace async{ namespace lin{

    class SendOperation
    {
        public:

        SendOperation(IOUring &uring, int fd, const byte* buffer, size_t len, int flags) : m_sqe(uring.GetSQE())
        {
            m_sqe.PrepareSend(fd, buffer, len, flags);
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

#endif//QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURING_SENDOP