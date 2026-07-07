// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_NETWORK_CLIENT
#define QOR_PP_H_COMPONENTS_NETWORK_CLIENT

#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/memory/reference/reference.h"
#include "src/framework/io/network/sockets.h"
#include "src/framework/io/network/socket.h"
#include "src/framework/data/pipeline/pipeline.h"
#include "src/framework/data/pipeline/protocol.h"
#include "src/components/io/pipeline/connectors/socketconnector/socketconnector.h"

namespace qor{ namespace io{ namespace network { namespace components {

    //Abstract base client that provides Send and Receive over a pair of Pipelines
    //This doesn't assume anything about what the pipelines are connected to, local or remote, or what data is flowing
    //It doesn't therefore know how to connect up the pipelines, this must be specialized
    //It does assume simple bufferred, stateless, pipelines which can be reset for each send/receive
    //If you do something more complex than that Send and Receive can be overridden
    class BaseClient
    {
    public:

        BaseClient(){ }
        virtual ~BaseClient() = default;
        virtual bool Connect() = 0;

        virtual bool Send()
        {
            qor_pp_ofcontext;
            m_requestPipeline.ResetStream();
            return m_requestPipeline.PumpSome() > 0 ? true : false;
        }

        virtual bool Receive()
        {
            qor_pp_ofcontext;
            m_responsePipeline.ResetStream();
            return m_responsePipeline.PumpSome() > 0 ? true : false;
        }

    protected:

        pipeline::Pipeline m_requestPipeline;
        pipeline::Pipeline m_responsePipeline;

    };

    /*
    NetworkClient:
        A client that uses a socket connector at the device end of it's pipelines.
        It writes to the socket on Send and Reads from it on Receive.
        Configuration for the socket is exposed and this client is independent of whatever protocol is plugged in.
    Usage:
        Construct an instance
        SetProtocol (can be done as part of construction)
        Configure (can be done as part of connect)
        Connect
        Send
        Receive
        Disconnect        
    */

    class qor_pp_module_interface(QOR_NETCLIENT) NetworkClient : public BaseClient
    {
    public:

        NetworkClient();
        NetworkClient(const NetworkClient&) = delete;
        NetworkClient& operator = (const NetworkClient&) = delete;
        NetworkClient(   ref_of<pipeline::Protocol>::type protocol);
        virtual ~NetworkClient();
        void SetProtocol(ref_of<pipeline::Protocol>::type protocol);
        void SetSink(   pipeline::Element* sink, const pipeline::Buffer& sinkBuffer);
        void SetSource( pipeline::Element* source, const pipeline::Buffer& sourceBuffer);
        void Configure( const std::string &host, int port, const std::string &ip = "", network::sockets::eAddressFamily address_family = network::sockets::eAddressFamily::AF_INet, network::addrinfo_flags socket_flags = 0, bool tcp_nodelay = false, bool ipv6_v6only = false, time_t timeout_sec = 0);
        bool Connect(   const std::string &host, int port, const std::string &ip = "", network::sockets::eAddressFamily address_family = network::sockets::eAddressFamily::AF_INet, network::addrinfo_flags socket_flags = 0, bool tcp_nodelay = false, bool ipv6_v6only = false, time_t timeout_sec = 0);
        bool Connect();
        void Disconnect();

    protected:

        SocketConnector m_socketClientConnector;
        ref_of<pipeline::Protocol>::type m_protocol;

    };

}}}}//qor::io::network::components

#endif//QOR_PP_H_COMPONENTS_NETWORK_CLIENT
