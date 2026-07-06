// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURINGAWAITER
#define QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURINGAWAITER

#include <coroutine>
#include "iouring.h"
#include "src/framework/io/async/service.h"

namespace qor{ namespace io{ namespace async{ namespace lin{

    struct qor_pp_module_interface(QOR_LINUXASYNCIOSERVICE) IOUringAwaiter
    {
        IOUring::SQE entry;
        IOUring* m_ring;
        qor::io::async::Request requestData;

        IOUringAwaiter(IOUring::SQE& sqe, IOUring* ring) : entry(sqe), m_ring(ring) {}

        bool await_ready()
        {
            return false;
        }

        void await_suspend(std::coroutine_handle<> handle) noexcept
        {
            requestData.handle = handle;
            entry.SetData(&requestData);
            m_ring->RemoteSubmit();
            m_ring->m_guard.unlock();//We must have the ring in order to submit entires and here we must give it up to allow it to be submitted.
        }

        int await_resume()
        {
            return requestData.statusCode;
        }
    };

}}}}//qor::io::async::lin

#endif//QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURINGAWAITER
