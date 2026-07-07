// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXAMPLES_CLIENT_WORKFLOW
#define QOR_PP_H_EXAMPLES_CLIENT_WORKFLOW

#include "src/components/io/network/client/netclient.h"
#include "src/components/io/network/server/netserver.h"
#include "src/components/io/pipeline/sources/stdinsource/stdinsource.h"
#include "src/components/io/pipeline/sinks/stdoutsink/stdoutsink.h"
#include "src/framework/app/workflow/workflow.h"

using namespace qor;
using namespace qor::io;
using namespace qor::io::network;
using namespace qor::io::network::components;

class ClientWorkflow : public Workflow
{
public:

    ClientWorkflow();
    virtual ~ClientWorkflow() noexcept(true) = default;

private:

    qor::workflow::State::ref connect;    
    qor::workflow::State::ref send;
    qor::workflow::State::ref receive;    

    NetworkClient m_client;

    pipeline::ByteBuffer m_requestBuffer;
    pipeline::ByteBuffer m_responseBuffer;
    io::components::StdInSource m_source;
    io::components::StdOutSink m_sink;

};

#endif//QOR_PP_H_EXAMPLES_CLIENT_WORKFLOW
