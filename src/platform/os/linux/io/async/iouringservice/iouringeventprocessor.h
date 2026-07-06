// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURINGSERVICE
#define QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURINGSERVICE

#include <chrono>
#include "iouring.h"
#include "src/framework/io/async/eventprocessor.h"

namespace qor{ namespace io{ namespace async{ namespace lin{

    class qor_pp_module_interface(QOR_LINUXASYNCIOSERVICE) IOUringEventProcessor : public qor::io::async::EventProcessor
    {
    public:

        IOUringEventProcessor() : qor::io::async::EventProcessor(), uring(256) {}
        virtual ~IOUringEventProcessor() noexcept = default;

        const IOUring& Ring() const{ return uring; }

        virtual int Run()
        {
            if(m_StopRequested)
            {
                return 0;
            }

            while(!m_StopRequested)
            {
                {
                    std::mutex dummy;
                    std::unique_lock<std::mutex> lock(dummy);
                    [[maybe_unused]]auto timeout_status = uring.m_cond.wait_for(lock, std::chrono::seconds(5));//wait while there is nothing to do
                }

                int result = 0;//Keep doing IO until we're told to stop or there are no pending coroutines to be resumed
                while(!m_StopRequested && (result > 0 || (uring.ExpectationCount() > 0)))
                {
                    std::unique_lock<std::mutex> lock(uring.m_guard);
                    result = Event();
                }
            }
            return 0;
        }

        virtual void Stop()
        {
            m_StopRequested = true;
            uring.m_cond.notify_one();
            //uring.sem.release(1);
            sleep(1);
            //uring.sem.try_acquire_for(std::chrono::seconds(2));
        }

    private:

        virtual int Event();
        IOUring uring;
    };

}}}}//qor::io::async::lin

#endif//QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURINGSERVICE
