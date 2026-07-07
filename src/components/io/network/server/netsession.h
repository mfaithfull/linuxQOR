// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_NETWORK_SESSION
#define QOR_PP_H_COMPONENTS_NETWORK_SESSION

#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/framework/app/workflow/workflow.h"
#include "src/framework/io/network/sockets.h"
#include "src/framework/io/network/socket.h"
#include "src/framework/io/async/service.h"
#include "src/framework/app/application/application.h"
#include "src/qor/flyers/log/debug.h"
#include "src/qor/flyers/error/error.h"
#include "src/framework/parallel/thread/threadpool.h"
#include "src/framework/data/pipeline/pipeline.h"
#include "sessionpipeline.h"

namespace qor{ namespace io{ namespace network { namespace components {

    class NetworkSession : public Workflow
    {
    public:

        NetworkSession(ref_of<network::Socket>::type socket, ref_of<pipeline::Protocol>::type protocol) :
            connected(new_ref<workflow::State>(this)),
            running(new_ref<workflow::State>(this)),
            disconnect(new_ref<workflow::State>(this)),
            m_socket(socket)
        {
            log::debug("Setting up a client session {0}", m_socket->m_fd);

            connected->Enter = [this,protocol]()->void
            {
                qor_pp_ofcontext;
                log::debug("Servicing a connected client {0}", m_socket->m_fd);
                auto application = AppBuilder().TheApplication();
                auto ioService = application(qor_shared).GetRole(qor_shared)->GetFeature<async::Service>();
                m_ioSession = ioService(qor_shared).GetSession();
                m_pipeline = new_ref<SessionPipeline>(m_socket, m_ioSession, protocol);
                SetState(running);
            };

            running->Enter = [this]()->void
            {
                qor_pp_ofcontext;
                size_t unitsPumped = 0;
                if(!m_pipeline->PumpSome(unitsPumped,-1) || unitsPumped == 0)
                {
                    SetState(disconnect);
                }
                else
                {
                    log::debug("Responded to client {0} with {1} bytes.", m_socket->m_fd, unitsPumped);
                }
            };

            disconnect->Enter = [this]()->void
            {
                qor_pp_ofcontext;
                log::debug("Disconnecting client {0}", m_socket->m_fd);
                m_socket->Shutdown(network::sockets::ShutdownReadWrite);
                SetComplete(EXIT_SUCCESS);
                PopState();
            };

            SetInitialState(connected);
        }

        ~NetworkSession()
        {
            log::debug("Ending client session {0}", m_socket->m_fd);
#ifndef NDEBUG
            CurrentThread::GetCurrent().SetName("pool");//reset the thread name as we're about to hand it back to the pool
#endif//DEBUG
        }

    private:

        //The 3 states of a per-client session
        ref_of<workflow::State>::type connected;
        ref_of<workflow::State>::type running;
        ref_of<workflow::State>::type disconnect;

        //session data
        ref_of<network::Socket>::type m_socket;
        ref_of<async::Context::Session>::type m_ioSession;
        ref_of<SessionPipeline>::type m_pipeline;

    };

}}}}//qor::io::network::components

#endif//QOR_PP_H_COMPONENTS_NETWORK_SESSION
