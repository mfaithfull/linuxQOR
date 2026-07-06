// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURING_READOP
#define QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURING_READOP

#include <system_error>
#include <coroutine>
#include "iouringawaiter.h"

namespace qor{ namespace io{ namespace async{ namespace lin{

    class ReadOperation
    {
        public:

        ReadOperation(IOUring &uring, int fd, byte* buffer, int32_t len, long offset) : m_sqe(uring.GetSQE())
        {
            m_sqe.PrepareRead( fd, buffer, len, offset);
            m_ring = const_cast<IOUring*>(&uring);
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

#endif//QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_READOP
