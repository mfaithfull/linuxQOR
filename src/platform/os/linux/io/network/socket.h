// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_NETWORK_LINUX_SOCKET
#define QOR_PP_H_PLATFORM_NETWORK_LINUX_SOCKET

#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/factory/externalfactory.h"
#include "src/framework/io/network/socket.h"
#include "src/framework/io/network/sockets.h"

namespace qor{ namespace io{ namespace network{ namespace lin{

    class qor_pp_module_interface(LINQOR_SOCKETS) Socket : public network::Socket
    {
    public:

        Socket();
        Socket(int sock);
        Socket(const Socket& src);
        Socket(const network::sockets::eAddressFamily& AF, const network::sockets::eType& Type, const network::sockets::eProtocol& Protocol);
        virtual ~Socket();

        virtual int32_t Bind(const network::Address& Address);
        virtual int32_t Bind(const qor::io::async::Interface& ioContext, const network::Address& Address);
        virtual int32_t Listen(int32_t iBacklog);
        virtual int32_t Listen(const qor::io::async::Interface& ioContext, int32_t iBacklog);
        virtual ref_of<network::Socket>::type Accept(io::network::Address& Address);
        virtual task<int32_t> AcceptAsync(const qor::io::async::Interface& ioContext, network::Address& Address, network::Socket* Socket);
        virtual int32_t Connect(const network::Address& Address);
        virtual int32_t GetPeerName(network::Address& Address);
        virtual int32_t GetSockName(network::Address& Address);
        virtual int32_t GetSockOpt(int32_t iLevel, int32_t iOptName, char* pOptVal, int32_t* pOptLen);
        virtual int32_t SetSockOpt(int32_t iLevel, int32_t iOptName, const char* pOptVal, int32_t iOptLen);
        virtual task<int32_t> AsyncReceive(const qor::io::async::Interface& ioContext, char* pBuffer, int32_t iLen);
        virtual int32_t Receive(char* buf, int32_t len, int32_t flags);
        virtual int32_t ReceiveFrom(char* Buffer, int32_t iLen, int32_t iFlags, network::Address& From);
        virtual task<int32_t> AsyncSend(const qor::io::async::Interface& ioContext, const char* Buffer, int32_t iLen);
        virtual int32_t Send(const char* Buffer, int32_t iLen);
        virtual int32_t SendTo(const char* Buffer, int32_t iLen, int32_t iFlags, const network::Address& To);
        virtual task<int32_t> AsyncShutdown(const qor::io::async::Interface& ioContext, network::sockets::eShutdown how);
        virtual int32_t Shutdown(network::sockets::eShutdown how);
        virtual std::size_t ID(void);
        virtual int32_t GetLastError(void);
        virtual bool SetNonBlocking(bool nonBlocking);
        virtual bool IsAlive();

        virtual int32_t Peek(char* buf, int32_t len);
        virtual bool SetRecvTimeout(time_t readTimeoutSec, time_t readTimeoutuSec);
        virtual bool SetSendTimeout(time_t readTimeoutSec, time_t readTimeoutuSec);
        virtual bool SetTCPNoDelay(bool nodelay);
        virtual bool SetIPv6Only(bool ipv6only);

        static int AddressFamilyToLinux(const network::sockets::eAddressFamily& AF);
        static network::sockets::eAddressFamily AddressFamilyFromLinux(int domain);
        static int TypeToLinux(const network::sockets::eType& Type, bool closeOnExec);
        static network::sockets::eType TypeFromLinux(int type);
        static int ProtocolToLinux(const network::sockets::eProtocol& Protocol);
        static network::sockets::eProtocol ProtocolFromLinux(int protocol);

    private:

        template <typename T> inline ssize_t handle_EINTR(T fn)
        {
            ssize_t res = 0;
            while (true)
            {
                res = fn();
                if (res < 0 && errno == EINTR)
                {
                    std::this_thread::sleep_for(std::chrono::microseconds{1});
                    continue;
                }
                break;
            }
            return res;
        }

        ssize_t PollWaitForInput(time_t sec, time_t usec);
        ssize_t PollWaitForOutput(time_t sec, time_t usec);
    };

}}}}//qor::io::network::lin

#endif//QOR_PP_H_PLATFORM_NETWORK_LINUX_SOCKET
