// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_NETWORK_SESSION_PIPELINE
#define QOR_PP_H_COMPONENTS_NETWORK_SESSION_PIPELINE

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/data/pipeline/pipeline.h"
#include "src/framework/data/pipeline/protocol.h"
#include "src/components/io/pipeline/connectors/socketconnector/socketconnector.h"
#include "src/components/io/pipeline/connectors/socketconnector/socketsource.h"
#include "src/components/io/pipeline/connectors/socketconnector/socketsink.h"

namespace qor{ namespace io{ namespace network{ namespace components {

    class SessionPipeline : public pipeline::Pipeline
    {
    public:

        SessionPipeline(
            ref_of<network::Socket>::type socket,
            ref_of<async::Context::Session>::type ioSession,
            ref_of<pipeline::Protocol>::type protocol
        ) : m_filter(protocol->GetRequestFilter()), m_socket(socket), m_ioSession(ioSession),m_socketSessionConnector(m_socket, m_ioSession)
        {
            //Setup the pipeline
            SetFlowMode(pipeline::Element::FlowMode::Push);

            //The source comes from the socket connector and uses the protocol filter as it's write buffer
            m_socketSource.SetPlug(&m_socketSessionConnector);
            SetSource(&m_socketSource, m_filter.operator->());

            //The sink goes back to the socket connector and uses the protocol filter as it's read buffer
            m_socketSink.SetPlug(&m_socketSessionConnector);
            SetSink(&m_socketSink, m_filter.operator->());

            //Connect the source and the sink
            m_socketSource.SetSink(&m_socketSink);
            m_socketSink.SetSource(&m_socketSource);

            //Set the buffer capacity
            GetSink()->GetBuffer()->SetCapacity(m_ioBufferSize);
            GetSource()->GetBuffer()->SetCapacity(m_ioBufferSize);

            //Connect the socket session. In practice this is noop as we are passed an already connected socket
            m_socketSessionConnector.Connect();
        }

        virtual ~SessionPipeline()
        {
            m_socketSessionConnector.Disconnect();
        }

    private:

        const size_t m_ioBufferSize = 2048; //TODO: Make this configurable
        ref_of<pipeline::InlineFilter<byte>>::type m_filter;
        ref_of<network::Socket>::type m_socket;
        ref_of<async::Context::Session>::type m_ioSession;
        SocketConnector m_socketSessionConnector;
        SocketSource m_socketSource;
        SocketSink m_socketSink;

    };

}}}}//qor::io::network::components

#endif//QOR_PP_H_COMPONENTS_NETWORK_SESSION_PIPELINE