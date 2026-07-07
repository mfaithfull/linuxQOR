// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_SOCKET
#define QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_SOCKET

#include "src/qor/essentials/current/currentthread.h"
#include "src/framework/io/network/socket.h"
#include "src/framework/io/network/sockets.h"
#include "src/framework/io/network/address.h"
#include "src/framework/io/network/addressinfo.h"
#include "src/framework/data/pipeline/connection.h"
#include "src/framework/io/async/context.h"

namespace qor{ namespace io{ namespace network { namespace components{

    class qor_pp_module_interface_gcc(QOR_SOCKETCONNECTOR) SocketConnector : public pipeline::Plug, virtual public qor::io::network::ISocket
    {
    public:

		qor_pp_module_interface(QOR_SOCKETCONNECTOR) SocketConnector();
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) SocketConnector(ref_of<qor::io::network::Socket>::type connectedSocket, ref_of<qor::io::async::Context::Session>::type session);
		qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual ~SocketConnector() noexcept;

        //Plug interface
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual bool Connect();													//Device specific connection
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual void Disconnect(void);											//Device specific disconnection
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual qor::pipeline::Element* GetSink() const;
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual qor::pipeline::Element* GetSource() const;
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) void Configure(const std::string &host, int port, const std::string &ip = "", qor::io::network::sockets::eAddressFamily address_family = qor::io::network::sockets::eAddressFamily::AF_INet, qor::io::network::addrinfo_flags socket_flags = 0, bool tcp_nodelay = false, bool ipv6_v6only = false, time_t timeout_sec = 0);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) const qor::io::network::Address& RemoteAddress();
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) void SetRemoteAddress(qor::io::network::Address& address);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) bool ConnectToAddress(const std::string &host, int port, const std::string &ip, qor::io::network::sockets::eAddressFamily address_family, qor::io::network::addrinfo_flags socket_flags, bool tcp_nodelay, bool ipv6_v6only, time_t timeout_sec = 0 );
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) bool SetNonBlocking(bool nonBlocking);

        //ISocket implementation over owned socket instance

        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual int32_t Bind(const qor::io::network::Address& Address);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual int32_t Bind(const async::Interface& ioContext, const qor::io::network::Address& Address);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual int32_t Listen(int32_t iBacklog);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual int32_t Listen(const async::Interface& ioContext, int32_t iBacklog);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual ref_of<qor::io::network::Socket>::type Accept(qor::io::network::Address& Address);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) ref_of<qor::io::network::Socket>::type Accept(const async::Interface& ioContext, qor::io::network::Address& Address);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual task<int32_t> AcceptAsync(const async::Interface& ioContext, qor::io::network::Address& Address, qor::io::network::Socket* Socket);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual int32_t Connect(const qor::io::network::Address& Address);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual int32_t GetPeerName(qor::io::network::Address& Address);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual int32_t GetSockName(qor::io::network::Address& Address);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual int32_t GetSockOpt(int32_t iLevel, int32_t iOptName, char* pOptVal, int32_t* pOptLen);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual int32_t SetSockOpt(int32_t iLevel, int32_t iOptName, const char* pOptVal, int32_t iOptLen);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual int32_t Receive(char* buf, int32_t len, int32_t flags);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual task<int32_t> AsyncReceive(const async::Interface& ioContext, char* pBuffer, int32_t iLen);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual int32_t ReceiveFrom(char* Buffer, int32_t iLen, int32_t iFlags, qor::io::network::Address& From);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual int32_t Send(const char* buf, int32_t len);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual task<int32_t> AsyncSend(const async::Interface& ioContext, const char* Buffer, int32_t iLen);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual int32_t SendTo(const char* Buffer, int32_t iLen, int32_t iFlags, const qor::io::network::Address& To);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual int32_t Shutdown(qor::io::network::sockets::eShutdown how);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) task<int32_t> Shutdown(const async::Interface& ioContext, qor::io::network::sockets::eShutdown how);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual task<int32_t> AsyncShutdown(const async::Interface& ioContext, qor::io::network::sockets::eShutdown how);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual std::size_t ID(void);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual int32_t GetLastError(void);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual bool IsAlive();

	protected:

        std::string m_host;
        std::string m_ip;
        unsigned  m_port;
        qor::io::network::sockets::eAddressFamily m_addressFamily;
        qor::io::network::addrinfo_flags m_socketFlags;
        bool m_tcpNodelay;
        bool m_ipv6Only;
        time_t m_timeoutSec;
        ref_of<qor::io::network::Socket>::type m_Socket;
        qor::io::network::Address m_remoteAddress;
        qor::ref_of<qor::pipeline::Sink>::type m_sink;
        qor::ref_of<qor::pipeline::Source>::type m_source;
        ref_of<async::Context::Session>::type m_Session;

    private:

        qor_pp_module_interface(QOR_SOCKETCONNECTOR) void SetupHintsForGetAddressInfo(qor::io::network::AddressInfo& hints, const std::string &host, const std::string &ip, qor::io::network::sockets::eAddressFamily address_family, qor::io::network::addrinfo_flags socket_flags, std::string& node);
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) bool AttemptConnections(ref_of<qor::io::network::Sockets>::type socketsSubsystem, std::vector<qor::io::network::AddressInfo>& addresses);
    };

}}}}//qor::io::network::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_SOCKET
