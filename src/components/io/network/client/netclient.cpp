// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "netclient.h"

using namespace qor::pipeline;

namespace qor{ namespace io{ namespace network{ namespace components {

    NetworkClient::NetworkClient() : BaseClient(){ }

    NetworkClient::NetworkClient(ref_of<pipeline::Protocol>::type protocol) : BaseClient(), m_protocol(protocol){ }

    NetworkClient::~NetworkClient() = default;

    void NetworkClient::SetProtocol(ref_of<pipeline::Protocol>::type protocol)
    {
        m_protocol = protocol;
    }

    void NetworkClient::SetSink(pipeline::Element* sink, const pipeline::Buffer& sinkBuffer)
    {
        if(sink && sink->IsSink())
        {
            m_responsePipeline.SetFlowMode(Element::FlowMode::Push);
            m_responsePipeline.SetSource(m_socketClientConnector.GetSource());
            m_responsePipeline.GetSource()->SetBuffer(sinkBuffer);
            m_responsePipeline.SetSink(sink);
        }
    }

    void NetworkClient::SetSource(pipeline::Element* source, const pipeline::Buffer& sourceBuffer)
    {
        if(source && source->IsSource())
        {
            m_requestPipeline.SetFlowMode(Element::FlowMode::Push);
            m_socketClientConnector.GetSink()->SetBuffer(sourceBuffer);
            m_requestPipeline.SetSource(source);
            m_requestPipeline.SetSink(m_socketClientConnector.GetSink());
            m_requestPipeline.GetSource()->SetSink(m_socketClientConnector.GetSink());
        }
    }

    void NetworkClient::Configure(const std::string &host, int port, const std::string &ip, network::sockets::eAddressFamily address_family, network::addrinfo_flags socket_flags, bool tcp_nodelay, bool ipv6_v6only, time_t timeout_sec)
    {
        m_socketClientConnector.Configure(host, port, ip, address_family, socket_flags, tcp_nodelay, ipv6_v6only, timeout_sec);
    }

    bool NetworkClient::Connect(const std::string &host, int port, const std::string &ip, network::sockets::eAddressFamily address_family, network::addrinfo_flags socket_flags, bool tcp_nodelay, bool ipv6_v6only, time_t timeout_sec)
    {
        qor_pp_ofcontext;
        m_socketClientConnector.Configure(host, port, ip, address_family, socket_flags, tcp_nodelay, ipv6_v6only, timeout_sec);
        return Connect();
    }

    bool NetworkClient::Connect()
    {
        qor_pp_ofcontext;
        if(m_protocol.IsNull())
        {
            serious("No protocol set for NetworkClient connection.");
            return false;
        }
        else
        {
            return m_socketClientConnector.Connect();
        }
    }

    void NetworkClient::Disconnect()
    {
        qor_pp_ofcontext;
        m_socketClientConnector.Disconnect();
    }

}}}}//qor::io::network::components
