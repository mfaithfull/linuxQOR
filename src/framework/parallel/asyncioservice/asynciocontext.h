// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_ASYNCIOCONTEXT
#define QOR_PP_H_FRAMEWORK_ASYNCIOCONTEXT

#include <future>
#include "asynciotypes.h"
#include "asyncioeventprocessor.h"
#include "asyncioinitiator.h"
#include "src/qor/memory/instance/pool.h"
#include "src/qor/memory/instance/instance.h"
#include "src/framework/parallel/task/syncwait.h"
#include "src/framework/parallel/thread/threadpool.h"
#include "asynciointerface.h"
#include "src/qor/flyers/log/informative.h"
#include "src/framework/parallel/sync/mutex.h"
#include "src/framework/io/network/isocket.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/externalfactory.h"
#include "src/qor/memory/reference/newref.h"

namespace qor { namespace async{

    class AsyncIOContext : public AsyncIOInterface
    {
    public:
        
        inline AsyncIOContext(ref_of<thread::ThreadPool>::type threadPool) : m_threadPool(threadPool)
        {
            m_initiator = new_ref<AsyncIOInitiator>();
            if(m_initiator->RequiresBackgroundProcessor())
            {
                m_processor = new_ref<AsyncIOEventProcessor>();
            }
            m_initiator->ConnectToProcessor(m_processor);
            Inflate();
        }

        inline ~AsyncIOContext() = default;

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
        
        inline bool Enroll(io::IODescriptor& ioDescriptor) const
        {
            return m_processor->Enroll(ioDescriptor);
        }

        virtual inline task<int> Send(io::IODescriptor* ioDescriptor, byte* buffer, size_t len, int flags) const
        {
            auto ioResult = co_await m_initiator->Send(ioDescriptor, buffer, len, flags);
            co_return ioResult.status_code;
        }

        virtual inline task<int> Recv(io::IODescriptor* ioDescriptor, byte* buffer, size_t len) const
        {
            auto ioResult = co_await m_initiator->Recv(ioDescriptor, buffer, len);
            co_return ioResult.status_code;
        }

        virtual inline task<int> Read(io::IODescriptor* ioDescriptor, byte* buffer, size_t len, long offset) const
        {
            auto ioResult = co_await m_initiator->Read(ioDescriptor, buffer, len, offset);
            co_return ioResult.status_code;
        }

        virtual inline task<int> Write(io::IODescriptor* ioDescriptor, byte* buffer, size_t len, long offset) const
        {
            auto ioResult = co_await m_initiator->Write(ioDescriptor, buffer, len, offset);
            co_return ioResult.status_code;
        }

        virtual inline task<int> Shutdown(io::IODescriptor* ioDescriptor, int how) const
        {
            auto ioResult = co_await m_initiator->Shutdown(ioDescriptor, how);
            co_return ioResult.status_code;
        }

        virtual inline task<int> Listen(io::IODescriptor* ioDescriptor, int backlog) const
        {
            auto ioResult = co_await m_initiator->Listen(ioDescriptor, backlog);
            co_return ioResult.status_code;
        }

        virtual inline task<int> Bind(io::IODescriptor* ioDescriptor, const io::network::Address& Address) const
        {
            auto ioResult = co_await m_initiator->Bind(ioDescriptor, Address);
            co_return ioResult.status_code;
        }

        virtual inline task<int> Accept(io::IODescriptor* ioDescriptor, const io::network::Address& Address, io::network::Socket* Socket) const
        {
            auto ioResult = co_await m_initiator->Accept(ioDescriptor, Address, Socket);
            co_return ioResult.status_code;
        }

        //Provides synchronisaed access to a shared IO Context
        class Session : public AsyncIOInterface
        {
            public:

            Session(AsyncIOContext* sharedContext) : m_sharedContext(*sharedContext){ }

            virtual ~Session() = default;

            virtual inline bool Enroll(io::IODescriptor& ioDescriptor) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Enroll(ioDescriptor);
            }

            virtual inline task<int> Send(io::IODescriptor* ioDescriptor, byte* buffer, size_t len, int flags) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Send(ioDescriptor, buffer, len, flags);
            }

            virtual inline task<int> Recv(io::IODescriptor* ioDescriptor, byte* buffer, size_t len) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Recv(ioDescriptor, buffer, len);
            }

            virtual inline task<int> Read(io::IODescriptor* ioDescriptor, byte* buffer, size_t len, long offset) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Read(ioDescriptor, buffer, len, offset);
            }

            virtual inline task<int> Write(io::IODescriptor* ioDescriptor, byte* buffer, size_t len, long offset) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Write(ioDescriptor, buffer, len, offset);
            }

            virtual inline task<int> Shutdown(io::IODescriptor* ioDescriptor, int how) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Shutdown(ioDescriptor, how);
            }

            virtual inline task<int> Listen(io::IODescriptor* ioDescriptor, int backlog) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Listen(ioDescriptor, backlog);
            }

            virtual inline task<int> Bind(io::IODescriptor* ioDescriptor, const io::network::Address& Address) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Bind(ioDescriptor, Address);
            }

            virtual inline task<int> Accept(io::IODescriptor* ioDescriptor, const io::network::Address& Address, io::network::Socket* Socket) const
            {
                Lock lock(m_sharedContext.m_access);
                return m_sharedContext.Accept(ioDescriptor, Address, Socket);
            }

        private:

            AsyncIOContext& m_sharedContext;
        };

        ref_of<Session>::type GetSession()
        {
            return new_ref<Session>(this);
        }

    private:

        ref_of<AsyncIOInitiator>::type m_initiator;
        ref_of<AsyncIOEventProcessor>::type m_processor;
        ref_of<thread::ThreadPool>::type m_threadPool;
        std::future<int> m_processorResult;
        Mutex m_access;
        friend class Session;
    };

    }//async

    qor_pp_declare_instancer_of(async::AsyncIOContext, PoolInstancer);

}//qor::async

#endif//QOR_PP_H_FRAMEWORK_ASYNCIOCONTEXT