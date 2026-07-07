// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_NETWORK_SERVER
#define QOR_PP_H_COMPONENTS_NETWORK_SERVER

#include "src/qor/flyers/interception/functioncontext.h"
#include "src/framework/app/workflow/workflow.h"
#include "src/framework/io/network/sockets.h"
#include "src/framework/io/network/socket.h"
#include "src/framework/io/async/service.h"
#include "src/framework/app/application/application.h"
#include "src/qor/flyers/log/debug.h"
#include "src/qor/flyers/error/error.h"
#include "src/framework/parallel/thread/threadpool.h"
#include "src/framework/data/pipeline/protocol.h"

namespace qor{ namespace io{ namespace network{ namespace components {

    class qor_pp_module_interface(QOR_NETSERVER) NetworkServer : public Workflow
    {
    public:

        NetworkServer(unsigned port, ref_of<pipeline::Protocol>::type protocol);

    private:

        //The 3 states of a network server workflow
        ref_of<workflow::State>::type bind;
        ref_of<workflow::State>::type listen;
        ref_of<workflow::State>::type accept;

        //Subsystems and features needed
        ref_of<network::Sockets>::type m_sockets;
        ref_of<async::Service>::type m_io;
        ref_of<thread::ThreadPool>::type m_threadPool;
        ref_of<async::Context::Session>::type m_ioSession;

        //Port and socket for connections to the server
        unsigned m_port;
        ref_of<network::Socket>::type m_serverSocket;
    };

}}}}//qor::io::network::components

#endif//QOR_PP_H_COMPONENTS_NETWORK_SERVER
