// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "iouringeventprocessor.h"
#include "src/platform/os/linux/io/async/asyncioservice.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log//informative.h"

qor_pp_module_provide(QOR_LINUXASYNCIOSERVICE, EventProcessor)

namespace qor{ namespace io{ namespace async{ namespace lin{

    int IOUringEventProcessor::Event()
    {
        io_uring_cqe *temp = nullptr;
        __kernel_timespec ts{ .tv_sec = 0, .tv_nsec = 50000 /*/ (1 << scale)*/ };
        sigset_t sigmask;
        memset(&sigmask, 0, sizeof(sigset_t));
        unsigned wait_nr = uring.ExpectationCount();
        if( wait_nr == 0)
        {
            wait_nr = 1;
        }
        int result = io_uring_submit_and_wait_timeout(uring.get(), &temp, wait_nr, &ts, &sigmask);

        if(result >= 0)
        {
            if(temp != nullptr)
            {
                return uring.ConsumeCQEntries();
                /*
                if( result == 0)
                {
                    return uring.ConsumeCQEntries();
                }
                else
                {
                    return uring.ConsumeCQEntries(temp, wait_nr);
                }
                */
            }
            else
            {
                return 0;
            }
        }
        else if (result < 0)
        {
            if(result == -EINTR || result == -EAGAIN)
            {
                return 0;
            }
            if(result != -ETIME)
            {
                serious("IOUring error");
            }
        }
        return result;
    }

}}}}//qor::io::async::lin