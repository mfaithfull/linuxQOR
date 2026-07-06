// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_ASYNCIOSERVICE_ASYNCIOEVENTPROCESSOR
#define QOR_PP_H_FRAMEWORK_ASYNCIOSERVICE_ASYNCIOEVENTPROCESSOR

#include "src/qor/essentials/datastructures/guid.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/externalfactory.h"
#include "src/framework/parallel/sync/asyncmanualresetevent.h"
#include "src/framework/parallel/task/task.h"
#include "src/framework/parallel/task/syncwait.h"
#include "src/framework/io/iodescriptor.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/error/commonerrormessages.h"

namespace qor { namespace io{ namespace async{

    class EventProcessor
    {
    public:

        EventProcessor() :
            m_freeRun(false),
            m_StopRequested(false){ }

        inline virtual ~EventProcessor() noexcept = default;

        virtual int Run()
        {
            qor_pp_ofcontext;
            fatal(Error_empty_base_called);
            while(!m_StopRequested)
            {
            }
            return 0;
        }

        virtual void Stop() { m_StopRequested = true; }
        virtual void Reset() { m_StopRequested = false;}

        virtual bool Enroll(io::Descriptor& /*ioDescriptor*/) const {return false;}

    protected:

        virtual int Event(){return 0;};
        bool m_freeRun;
        bool m_StopRequested;
    };

    }}//io::async

    qor_pp_declare_factory_of(io::async::EventProcessor, ExternalFactory);
    constexpr GUID AsyncIOEventProcessorGUID = {0xe92b1e2d, 0x2295, 0x4f5e, {0xb0, 0xba, 0xea, 0xb9, 0x97, 0x27, 0xb5, 0x22}};
    qor_pp_declare_guid_of(io::async::EventProcessor,AsyncIOEventProcessorGUID);

}//qor

#endif//QOR_PP_H_FRAMEWORK_ASYNCIOSERVICE_ASYNCIOEVENTPROCESSOR
