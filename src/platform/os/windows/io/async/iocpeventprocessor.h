// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCPEVENTPROCESSOR
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCPEVENTPROCESSOR

#include <chrono>
#include "src/framework/io/async/eventprocessor.h"
#include "ioservice.h"
#include "operations/scheduleoperation.h"

namespace qor { namespace io {namespace async { namespace win {

    class IOCPEventProcessor : public qor::io::async::EventProcessor
    {
    public:

        inline IOCPEventProcessor() : qor::io::async::EventProcessor() {}
        inline virtual ~IOCPEventProcessor() noexcept = default;

        inline virtual int Run()
        {
            uint64_t eventCount = m_service.ProcessEvents();
            return 0;
        }

        inline virtual void Stop()
        {
            m_service.Stop();
        }

        inline ScheduleOperation Schedule()
        {
            return m_service.Schedule();
        }

        inline virtual bool Enroll(io::Descriptor& ioDescriptor) const
        {
            return m_service.Enroll(ioDescriptor);
        }

    private:

        IOService m_service;

    };

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCPEVENTPROCESSOR
