// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/platform/platform.h"
#include "socketconnector.h"
#include "socketsink.h"
#include "socketsource.h"

namespace qor{ namespace io { namespace network{ namespace components{

    SocketConnector::SocketConnector() : Plug()
    {
        m_host = "";
        m_ip = "";
        m_port = 0;
        m_addressFamily = qor::io::network::sockets::AF_Unspecified;
        m_socketFlags = 0;
        m_tcpNodelay = false;
        m_ipv6Only = false;
        m_timeoutSec = 0;

        m_sink = new_ref<SocketSink>();
        m_source = new_ref<SocketSource>();
        m_sink->SetPlug(this);
        m_source->SetPlug(this);
    }

    SocketConnector::SocketConnector(
        ref_of<qor::io::network::Socket>::type connectedSocket,
        ref_of<qor::io::async::Context::Session>::type session
        ) : SocketConnector()
    {
        m_Socket = connectedSocket;
        m_Session = session;
        m_connected = true;
    }

    SocketConnector::~SocketConnector() noexcept
    {
        if (m_connected)
        {
            Disconnect();
        }
    }

    qor::pipeline::Element* SocketConnector::GetSink() const
    {
        return m_sink;
    }

    qor::pipeline::Element* SocketConnector::GetSource() const
    {
        return m_source;
    }

    const qor::io::network::Address& SocketConnector::RemoteAddress()
    {
        return m_remoteAddress;
    }

    void SocketConnector::SetRemoteAddress(qor::io::network::Address& address)
    {
        m_remoteAddress = address;
    }

    bool SocketConnector::Connect()
    {
        if(!m_connected)
        {
            m_connected = ConnectToAddress(m_host, m_port, m_ip, m_addressFamily, m_socketFlags, m_tcpNodelay, m_ipv6Only, m_timeoutSec);
        }
        return m_connected;
    }

    void SocketConnector::SetupHintsForGetAddressInfo(network::AddressInfo& hints, const std::string &host, const std::string &ip, qor::io::network::sockets::eAddressFamily address_family, qor::io::network::addrinfo_flags socket_flags, std::string& node)
    {
        hints.socktype = network::sockets::eType::Sock_Stream;
        hints.protocol = network::sockets::eProtocol::IPProto_IP;

        if (!ip.empty())
        {
            node = ip;
            // Ask getaddrinfo to convert IP in c-string to address
            hints.family = network::sockets::eAddressFamily::AF_Unspecified;
            hints.flags.DontUseHostNameResolution(true);
        }
        else
        {
            if (!host.empty())
            {
                node = host;
            }
            hints.family = address_family;
            hints.flags = socket_flags;
        }

    }

    bool SocketConnector::AttemptConnections(ref_of<qor::io::network::Sockets>::type socketsSubsystem, std::vector<network::AddressInfo>& addresses)
    {
        for(auto address : addresses)
        {
            m_remoteAddress.sa_family = static_cast<unsigned short>(address.family);
            m_remoteAddress.SetPort( address.address.sa.IPAddress.sin_port);
            memcpy(m_remoteAddress.sa.sa_data, address.address.sa.sa_data, 30/*address.address.byte_size*/);
            m_Socket = socketsSubsystem->CreateSocket(address.family, address.socktype, address.protocol, ref_of<qor::io::async::Context::Session>::type());

            m_connected = m_Socket->Connect(m_remoteAddress) == 0;
            return m_connected;
        }
        return false;
    }

    void SocketConnector::Configure(const std::string &host, int port, const std::string &ip,
        qor::io::network::sockets::eAddressFamily address_family, qor::io::network::addrinfo_flags socket_flags, bool tcp_nodelay, bool ipv6_v6only, time_t timeout_sec)
    {
        m_host = host;
        m_ip = ip;
        m_port = port;
        m_addressFamily = address_family;
        m_socketFlags = socket_flags;
        m_tcpNodelay = tcp_nodelay;
        m_ipv6Only = ipv6_v6only;
        m_timeoutSec = timeout_sec;
    }

    bool SocketConnector::ConnectToAddress(const std::string &host, int port, const std::string &ip, qor::io::network::sockets::eAddressFamily address_family, qor::io::network::addrinfo_flags socket_flags, bool /*tcp_nodelay*/, bool /*ipv6_v6only*/, time_t /*timeout_sec*/)
    {
        if(address_family == qor::io::network::sockets::eAddressFamily::AF_Unix)
        {
            return false;//TODO: ConnectUnixSocketToAddress();
        }

        auto socketsSubsystem = ThePlatform(qor_shared)->GetSubsystem<network::Sockets>();
        auto service = std::to_string(port);
        std::string node;
        network::AddressInfo hints;
        std::vector<network::AddressInfo> results;

        SetupHintsForGetAddressInfo(hints, host, ip, address_family, socket_flags, node);

        if(socketsSubsystem->GetAddressInfo(node, service, hints, results /*timeout_sec*/) > 0)
        {
            return AttemptConnections(socketsSubsystem, results);
        }
        else
        {
            return false;
        }
    }

    void SocketConnector::Disconnect()
    {
        if(m_Session.IsNotNull())
        {
            sync_wait(m_Socket->Shutdown(*m_Session, network::sockets::eShutdown::ShutdownReadWrite));
        }
        else
        {
            m_Socket->Shutdown(network::sockets::eShutdown::ShutdownReadWrite);
        }
        m_connected = false;
        m_Socket.Dispose();
    }

    bool SocketConnector::SetNonBlocking(bool nonBlocking)
    {
        return m_Socket->SetNonBlocking(nonBlocking);
    }

        //ISocket implementation over owned socket instance

    int32_t SocketConnector::Bind(const qor::io::network::Address& Address)
    {
        return m_Socket->Bind(Address);
    }

    int32_t SocketConnector::Bind(const async::Interface& ioContext, const qor::io::network::Address& Address)
    {
        return m_Socket->Bind(ioContext, Address);
    }

    int32_t SocketConnector::Listen(int32_t iBacklog)
    {
        return m_Socket->Listen(iBacklog);
    }

    int32_t SocketConnector::Listen(const async::Interface& ioContext, int32_t iBacklog)
    {
        return m_Socket->Listen(ioContext, iBacklog);
    }

    ref_of<qor::io::network::Socket>::type SocketConnector::Accept(qor::io::network::Address& Address)
    {
        return m_Socket->Accept(Address);
    }

    ref_of<qor::io::network::Socket>::type SocketConnector::Accept(const async::Interface& ioContext, qor::io::network::Address& Address)
    {
        return m_Socket->Accept(ioContext, Address);
    }

    task<int32_t> SocketConnector::AcceptAsync(const async::Interface& ioContext, qor::io::network::Address& Address, qor::io::network::Socket* Socket)
    {
        return m_Socket->AcceptAsync(ioContext, Address, Socket);
    }

    int32_t SocketConnector::Connect(const qor::io::network::Address& Address)
    {
        return m_Socket->Connect(Address);
    }

    int32_t SocketConnector::GetPeerName(qor::io::network::Address& Address)
    {
        return m_Socket->GetPeerName(Address);
    }

    int32_t SocketConnector::GetSockName(qor::io::network::Address& Address)
    {
        return m_Socket->GetSockName(Address);
    }

    int32_t SocketConnector::GetSockOpt(int32_t iLevel, int32_t iOptName, char* pOptVal, int32_t* pOptLen)
    {
        return m_Socket->GetSockOpt(iLevel, iOptName, pOptVal, pOptLen);
    }

    int32_t SocketConnector::SetSockOpt(int32_t iLevel, int32_t iOptName, const char* pOptVal, int32_t iOptLen)
    {
        return m_Socket->SetSockOpt(iLevel, iOptName, pOptVal, iOptLen);
    }

    int32_t SocketConnector::Receive(char* buf, int32_t len, int32_t flags)
    {
        if(m_Session.IsNotNull())
        {
            return sync_wait(m_Socket->Receive(*m_Session, buf, len));
        }
        else
        {
            return m_Socket->Receive(buf, len, flags);
        }
    }

    task<int32_t> SocketConnector::AsyncReceive(const async::Interface& ioContext, char* pBuffer, int32_t iLen)
    {
        return m_Socket->AsyncReceive(ioContext, pBuffer, iLen);
    }

    int32_t SocketConnector::ReceiveFrom(char* Buffer, int32_t iLen, int32_t iFlags, qor::io::network::Address& From)
    {
        return m_Socket->ReceiveFrom(Buffer, iLen, iFlags, From);
    }

    int32_t SocketConnector::Send(const char* buf, int32_t len)
    {
        if(m_Session.IsNotNull())
        {
            return sync_wait(m_Socket->Send(*m_Session, buf, len));
        }
        else
        {
            return m_Socket->Send(buf, len);
        }
    }

    task<int32_t> SocketConnector::AsyncSend(const async::Interface& ioContext, const char* Buffer, int32_t iLen)
    {
        return m_Socket->AsyncSend(ioContext, Buffer, iLen);
    }

    int32_t SocketConnector::SendTo(const char* Buffer, int32_t iLen, int32_t iFlags, const qor::io::network::Address& To)
    {
        return m_Socket->SendTo(Buffer, iLen, iFlags, To);
    }

    int32_t SocketConnector::Shutdown(qor::io::network::sockets::eShutdown how)
    {
        return m_Socket->Shutdown(how);
    }

    task<int32_t> SocketConnector::Shutdown(const async::Interface& ioContext, qor::io::network::sockets::eShutdown how)
    {
        return m_Socket->Shutdown(ioContext, how);
    }

    task<int32_t> SocketConnector::AsyncShutdown(const async::Interface& ioContext, qor::io::network::sockets::eShutdown how)
    {
        return m_Socket->AsyncShutdown(ioContext, how);
    }

    std::size_t SocketConnector::ID(void)
    {
        return m_Socket->ID();
    }

    int32_t SocketConnector::GetLastError(void)
    {
        return m_Socket->GetLastError();
    }

    bool SocketConnector::IsAlive()
    {
        return m_Socket->IsAlive();
    }

}}}}//qor::io::network::components
