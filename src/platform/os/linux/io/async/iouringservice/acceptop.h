// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURING_ACCEPTOP
#define QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURING_ACCEPTOP

#include <system_error>
#include <coroutine>
#include "iouringawaiter.h"

namespace qor{ namespace io{ namespace async{ namespace lin{

    class AcceptOperation
    {
        public:

        AcceptOperation(IOUring &uring, int fd, struct sockaddr *addr, socklen_t *addrlen, int flags) : m_sqe(uring.GetSQE())
        {
            m_sqe.PrepareAccept(fd, addr, addrlen, flags);
            m_ring = const_cast<IOUring*>(&uring);
        }

        auto operator co_await()
        {
            IOUringAwaiter awaiter {m_sqe, m_ring};
            return awaiter;
        }

    private:
        IOUring* m_ring;
        IOUring::SQE m_sqe;
    };

}}}}//qor::io::async::lin

#endif//QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURING_ACCEPTOP
