// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_ASYNCIOCONTEXT
#define QOR_PP_H_FRAMEWORK_ASYNCIOCONTEXT

#include <future>
#include "types.h"
#include "eventprocessor.h"
#include "initiator.h"
#include "src/qor/memory/instance/pool.h"
#include "src/qor/memory/instance/instance.h"
#include "src/framework/parallel/task/syncwait.h"
#include "src/framework/parallel/thread/threadpool.h"
#include "interface.h"
#include "src/qor/flyers/log/informative.h"
#include "src/qor/flyers/log/debug.h"
#include "src/framework/parallel/sync/mutex.h"
#include "src/framework/io/network/isocket.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/externalfactory.h"
#include "src/qor/memory/reference/newref.h"

namespace qor { namespace io{ namespace async{

    class Context : public Interface
    {
    public:

        inline Context(ref_of<thread::ThreadPool>::type threadPool) : m_threadPool(threadPool)
        {
            m_initiator = new_ref<Initiator>();
            if(m_initiator->RequiresBackgroundProcessor())
            {
                m_processor = new_ref<EventProcessor>();
            }
            m_initiator->ConnectToProcessor(m_processor);
            Inflate();
        }

        inline ~Context() = default;

        virtual void Inflate()
        {
            if(m_processor.IsNotNull())
            {
                m_processor->Reset();
                m_processorResult = m_threadPool->SubmitTask( [this]()
                {
                    CurrentThread::GetCurrent().SetName("IO Thread");                    
                    return m_processor->Run();
                });
            }
        }

        virtual void Deflate()
        {
            if(m_processor.IsNotNull())
            {
                m_processor->Stop();
                m_processorResult.get();
            }
        }

        virtual inline task<int> Bind(io::Descriptor* ioDescriptor, const io::network::Address& Address) const
        {
            auto ioResult = co_await m_initiator->Bind(ioDescriptor, Address);
            co_return ioResult.status_code;
        }

        virtual inline task<int> Listen(io::Descriptor* ioDescriptor, int backlog) const
        {
            auto ioResult = co_await m_initiator->Listen(ioDescriptor, backlog);
            co_return ioResult.status_code;
        }

        virtual inline task<int> Accept(io::Descriptor* ioDescriptor, const io::network::Address& Address, io::network::Socket* Socket) const
        {
            auto ioResult = co_await m_initiator->Accept(ioDescriptor, Address, Socket);
            co_return ioResult.status_code;
        }

        virtual inline task<int> Read(io::Descriptor* ioDescriptor, byte* buffer, size_t len, long offset) const
        {
            auto ioResult = co_await m_initiator->Read(ioDescriptor, buffer, len, offset);
            co_return ioResult.status_code;
        }

        virtual inline task<int> Write(io::Descriptor* ioDescriptor, byte* buffer, size_t len, long offset) const
        {
            auto ioResult = co_await m_initiator->Write(ioDescriptor, buffer, len, offset);
            co_return ioResult.status_code;
        }

        virtual inline task<int> Send(io::Descriptor* ioDescriptor, byte* buffer, size_t len, int flags) const
        {
            auto ioResult = co_await m_initiator->Send(ioDescriptor, buffer, len, flags);
            co_return ioResult.status_code;
        }

        virtual inline task<int> Recv(io::Descriptor* ioDescriptor, byte* buffer, size_t len) const
        {
            auto ioResult = co_await m_initiator->Recv(ioDescriptor, buffer, len);
            co_return ioResult.status_code;
        }

        virtual inline task<int> Shutdown(io::Descriptor* ioDescriptor, int how) const
        {
            auto ioResult = co_await m_initiator->Shutdown(ioDescriptor, how);
            co_return ioResult.status_code;
        }

        inline bool Enroll(io::Descriptor& ioDescriptor) const
        {
            return m_processor->Enroll(ioDescriptor);
        }

        //Provides synchronisaed access to a shared IO Context
        class Session : public Interface
        {
            public:

            Session(Context* sharedContext) : m_sharedContext(*sharedContext){ }

            virtual ~Session() = default;

            virtual inline task<int> Bind(io::Descriptor* ioDescriptor, const io::network::Address& Address) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Bind(ioDescriptor, Address);
            }

            virtual inline task<int> Listen(io::Descriptor* ioDescriptor, int backlog) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Listen(ioDescriptor, backlog);
            }

            virtual inline task<int> Accept(io::Descriptor* ioDescriptor, const io::network::Address& Address, io::network::Socket* Socket) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Accept(ioDescriptor, Address, Socket);
            }

            virtual inline task<int> Read(io::Descriptor* ioDescriptor, byte* buffer, size_t len, long offset) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Read(ioDescriptor, buffer, len, offset);
            }

            virtual inline task<int> Write(io::Descriptor* ioDescriptor, byte* buffer, size_t len, long offset) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Write(ioDescriptor, buffer, len, offset);
            }

            virtual inline task<int> Send(io::Descriptor* ioDescriptor, byte* buffer, size_t len, int flags) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Send(ioDescriptor, buffer, len, flags);
            }

            virtual inline task<int> Recv(io::Descriptor* ioDescriptor, byte* buffer, size_t len) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Recv(ioDescriptor, buffer, len);
            }

            virtual inline task<int> Shutdown(io::Descriptor* ioDescriptor, int how) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Shutdown(ioDescriptor, how);
            }

            virtual inline bool Enroll(io::Descriptor& ioDescriptor) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Enroll(ioDescriptor);
            }

        private:

            Context& m_sharedContext;
        };

        ref_of<Session>::type GetSession()
        {
            return new_ref<Session>(this);
        }

    private:

        ref_of<Initiator>::type m_initiator;
        ref_of<EventProcessor>::type m_processor;
        ref_of<thread::ThreadPool>::type m_threadPool;
        std::future<int> m_processorResult;
        Mutex m_access;
        friend class Session;
    };

    }}//io::async

    qor_pp_declare_instancer_of(io::async::Context, PoolInstancer);

}//qor::io::async

#endif//QOR_PP_H_FRAMEWORK_ASYNCIOCONTEXT