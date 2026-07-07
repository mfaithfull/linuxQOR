// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_NETWORK_ISOCKET
#define QOR_PP_H_PLATFORM_NETWORK_ISOCKET

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/factory/externalfactory.h"
#include "address.h"
#include "socketdefs.h"
#include "src/framework/io/async/types.h"
#include "src/framework/io/async/interface.h"
#include "src/framework/io/iodescriptor.h"

namespace qor{ namespace io { namespace network{

    class qor_pp_module_interface_gcc(QOR_SOCKETS) Socket;

    //Defines Socket interface to be implemented by each platform
    class ISocket
    {
    public:

        static constexpr int Invalid_Socket = -1;

        inline ISocket() = default;
        inline virtual ~ISocket() = default;
        virtual int32_t Bind(const Address& Address) = 0;
        virtual int32_t Bind(const async::Interface& ioContext, const Address& Address) = 0;
        virtual int32_t Listen(int32_t iBacklog) = 0;
        virtual int32_t Listen(const async::Interface& ioContext, int32_t iBacklog) = 0;
        virtual ref_of<Socket>::type Accept(Address& Address) = 0;
        virtual task<int32_t> AcceptAsync(const async::Interface& ioContext, Address& Address, Socket* Socket) = 0;
        virtual int32_t Connect(const Address& Address) = 0;
        virtual int32_t GetPeerName(Address& Address) = 0;
        virtual int32_t GetSockName(Address& Address) = 0;
        virtual int32_t GetSockOpt(int32_t iLevel, int32_t iOptName, char* pOptVal, int32_t* pOptLen) = 0;
        virtual int32_t SetSockOpt(int32_t iLevel, int32_t iOptName, const char* pOptVal, int32_t iOptLen) = 0;
        virtual int32_t Receive(char* buf, int32_t len, int32_t flags) = 0;
        virtual task<int32_t> AsyncReceive(const async::Interface& ioContext, char* pBuffer, int32_t iLen) = 0;
        virtual int32_t ReceiveFrom(char* Buffer, int32_t iLen, int32_t iFlags, Address& From) = 0;
        virtual int32_t Send(const char* Buffer, int32_t iLen) = 0;
        virtual task<int32_t> AsyncSend(const async::Interface& ioContext, const char* Buffer, int32_t iLen) = 0;
        virtual int32_t SendTo(const char* Buffer, int32_t iLen, int32_t iFlags, const Address& To) = 0;
        virtual int32_t Shutdown(sockets::eShutdown how) = 0;
        virtual task<int32_t> AsyncShutdown(const async::Interface& ioContext, sockets::eShutdown how) = 0;
        virtual std::size_t ID(void) = 0;
        virtual int32_t GetLastError(void) = 0;
        virtual bool SetNonBlocking(bool nonBlocking) = 0;
        virtual bool IsAlive() = 0;
    };

}}}//qor::io::network

#endif//QOR_PP_H_PLATFORM_NETWORK_ISOCKET
