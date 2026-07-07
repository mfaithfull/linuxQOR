// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/framework/parallel/thread/thread.h"
#include "socket.h"
#include "src/framework/io/async/types.h"
#include "src/framework/parallel/task/syncwait.h"

namespace qor{ namespace io { namespace network{

    Socket::Socket()
    {
        m_objectType = guid_of<Socket>::guid();
    }

    Socket::Socket(const sockets::eAddressFamily /*AF*/, const sockets::eType /*Type*/, const sockets::eProtocol /*Protocol*/)
    {
        m_objectType = guid_of<Socket>::guid();
    }

    Socket::~Socket() = default;

    task<int32_t> Socket::AcceptAsync(const async::Interface& /*ioContext*/, Address& /*ClientAddress*/, Socket* /*Socket*/)
    {
        //Provide an override implementation specific to your platform
         return []()->task<int32_t>
        {
            co_return -1;
        }();
    }

    ref_of<Socket>::type Socket::Accept(const async::Interface& ioContext, Address& ClientAddress)
    {
        ref_of<network::Socket>::type clientSocket = new_ref<network::Socket>(
            network::sockets::eAddressFamily::AF_INet,
            network::sockets::eType::Sock_Stream,
            network::sockets::eProtocol::IPProto_IP);

        [[maybe_unused]]auto result = sync_wait(AcceptAsync(ioContext, ClientAddress, clientSocket));
        ioContext.Enroll(*clientSocket);
        return clientSocket;
    }

    int32_t Socket::Bind(const Address& /*Address*/)
    {
        //Provide an override implementation specific to your platform
        return -1;
    }

    int32_t Socket::Bind(const async::Interface& /*ioWaiter*/, const Address& /*Address*/)
    {
        //Provide an override implementation specific to your platform
        return -1;
    }

    int32_t Socket::Listen(int32_t /*iBacklog*/)
    {
        //Provide an override implementation specific to your platform
        return -1;
    }

    int32_t Socket::Listen(const async::Interface& /*ioWaiter*/, int32_t /*iBacklog*/)
    {
        return -1;
    }

    ref_of<Socket>::type Socket::Accept(Address& /*Address*/)
    {
        //Provide an override implementation specific to your platform
        ref_of<Socket>::type nullref;
        return nullref;
    }

    int32_t Socket::Connect(const Address& /*Address*/)
    {
        //Provide an override implementation specific to your platform
        return -1;
    }

    int32_t Socket::GetPeerName(Address& /*Address*/)
    {
        //Provide an override implementation specific to your platform
        return -1;
    }

    int32_t Socket::GetSockName(Address& /*Address*/)
    {
        //Provide an override implementation specific to your platform
        return -1;
    }

    int32_t Socket::GetSockOpt(int32_t /*iLevel*/, int32_t /*iOptName*/, char* /*pOptVal*/, int32_t* /*pOptLen*/)
    {
        //Provide an override implementation specific to your platform
        return -1;
    }

    int32_t Socket::SetSockOpt(int32_t /*iLevel*/, int32_t /*iOptName*/, const char* /*pOptVal*/, int32_t /*iOptLen*/)
    {
        //Provide an override implementation specific to your platform
        return -1;
    }

    task<int32_t> Socket::AsyncReceive(const async::Interface& /*ioContext*/, char* /*pBuffer*/, int32_t /*iLen*/)
    {
        //Provide an override implementation specific to your platform
        return []()->task<int32_t>
        {
            co_return -1;
        }();
    }

    task<int32_t> Socket::Receive(const async::Interface& ioContext, char* Buffer, int32_t iLen)
    {
        return AsyncReceive(ioContext, Buffer, iLen);
    }

    int32_t Socket::Receive(char* /*buf*/, int32_t /*len*/, int32_t /*flags*/)
    {
        //Provide an override implementation specific to your platform
        return -1;
    }

    int32_t Socket::ReceiveFrom(char* /*Buffer*/, int32_t /*iLen*/, int32_t /*iFlags*/, Address& /*From*/)
    {
        //Provide an override implementation specific to your platform
        return -1;
    }

    task<int32_t> Socket::AsyncSend(const async::Interface& /*ioContext*/, const char* /*Buffer*/, int32_t /*iLen*/)
    {
        //Provide an override implementation specific to your platform
        qor::io::async::Result result;
        return []()->task<int32_t>
        {
            co_return -1;
        }();

    }

    task<int32_t> Socket::Send(const async::Interface& ioContext, const char* Buffer, int32_t iLen)
    {
        return AsyncSend(ioContext, Buffer, iLen);
    }

    int32_t Socket::Send(const char* /*Buffer*/, int32_t /*iLen*/)
    {
        //Provide an override implementation specific to your platform
        return -1;
    }

    int32_t Socket::SendTo(const char* /*Buffer*/, int32_t /*iLen*/, int32_t /*iFlags*/, const Address& /*To*/)
    {
        //Provide an override implementation specific to your platform
        return -1;
    }

    int32_t Socket::Shutdown(sockets::eShutdown /*how*/)
    {
        //Provide an override implementation specific to your platform
        return -1;
    }

    task<int32_t> Socket::Shutdown(const async::Interface& ioContext, sockets::eShutdown how)
    {
        return AsyncShutdown(ioContext, how);
    }

    task<int32_t> Socket::AsyncShutdown(const async::Interface& /*ioContext*/, sockets::eShutdown /*how*/)
    {
        //Provide an override implementation specific to your platform
        return []()->task<int32_t>
        {
            co_return -1;
        }();
    }

    std::size_t Socket::ID(void)
    {
        //Provide an override implementation specific to your platform
        return (std::size_t)( -1);
    }

    int32_t Socket::GetLastError(void)
    {
        //Provide an override implementation specific to your platform
        return -1;
    }

    bool Socket::SetNonBlocking(bool /*nonBlocking*/)
    {
        //Provide an override implementation specific to your platform
        return false;
    }

    bool Socket::IsAlive()
    {
        return false;
    }

}}}//qor::io::network
