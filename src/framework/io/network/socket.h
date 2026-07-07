// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_NETWORK_SOCKET
#define QOR_PP_H_PLATFORM_NETWORK_SOCKET

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
#include "isocket.h"

namespace qor{ namespace io { namespace network{

    //Base partial implementation of Socket
    class qor_pp_module_interface_gcc(QOR_SOCKETS) Socket : public io::Descriptor, public ISocket
    {
    public:

        qor_pp_module_interface(QOR_SOCKETS) Socket();
        qor_pp_module_interface(QOR_SOCKETS) ~Socket();
        qor_pp_module_interface(QOR_SOCKETS) Socket(const sockets::eAddressFamily AF, const sockets::eType Type, const sockets::eProtocol Protocol);

        qor_pp_module_interface(QOR_SOCKETS) virtual int32_t Bind(const Address& Address);
        qor_pp_module_interface(QOR_SOCKETS) virtual int32_t Bind(const async::Interface& ioContext, const Address& Address);
        qor_pp_module_interface(QOR_SOCKETS) virtual int32_t Listen(int32_t iBacklog);
        qor_pp_module_interface(QOR_SOCKETS) virtual int32_t Listen(const async::Interface& ioContext, int32_t iBacklog);
        qor_pp_module_interface(QOR_SOCKETS) virtual ref_of<Socket>::type Accept(Address& Address);
        qor_pp_module_interface(QOR_SOCKETS) ref_of<Socket>::type Accept(const async::Interface& ioContext, Address& Address);
        qor_pp_module_interface(QOR_SOCKETS) virtual task<int32_t> AcceptAsync(const async::Interface& ioContext, Address& Address, Socket* Socket);
        qor_pp_module_interface(QOR_SOCKETS) virtual int32_t Connect(const Address& Address);
        qor_pp_module_interface(QOR_SOCKETS) virtual int32_t GetPeerName(Address& Address);
        qor_pp_module_interface(QOR_SOCKETS) virtual int32_t GetSockName(Address& Address);
        qor_pp_module_interface(QOR_SOCKETS) virtual int32_t GetSockOpt(int32_t iLevel, int32_t iOptName, char* pOptVal, int32_t* pOptLen);
        qor_pp_module_interface(QOR_SOCKETS) virtual int32_t SetSockOpt(int32_t iLevel, int32_t iOptName, const char* pOptVal, int32_t iOptLen);
        qor_pp_module_interface(QOR_SOCKETS) virtual int32_t Receive(char* buf, int32_t len, int32_t flags);
        qor_pp_module_interface(QOR_SOCKETS) task<int32_t> Receive(const async::Interface& ioContext, char* Buffer, int32_t iLen);
        qor_pp_module_interface(QOR_SOCKETS) virtual task<int32_t> AsyncReceive(const async::Interface& ioContext, char* pBuffer, int32_t iLen);
        qor_pp_module_interface(QOR_SOCKETS) virtual int32_t ReceiveFrom(char* Buffer, int32_t iLen, int32_t iFlags, Address& From);
        qor_pp_module_interface(QOR_SOCKETS) virtual int32_t Send(const char* Buffer, int32_t iLen);
        qor_pp_module_interface(QOR_SOCKETS) task<int32_t> Send(const async::Interface& ioContext, const char* Buffer, int32_t iLen);
        qor_pp_module_interface(QOR_SOCKETS) virtual task<int32_t> AsyncSend(const async::Interface& ioContext, const char* Buffer, int32_t iLen);
        qor_pp_module_interface(QOR_SOCKETS) virtual int32_t SendTo(const char* Buffer, int32_t iLen, int32_t iFlags, const Address& To);
        qor_pp_module_interface(QOR_SOCKETS) virtual int32_t Shutdown(sockets::eShutdown how);
        qor_pp_module_interface(QOR_SOCKETS) task<int32_t> Shutdown(const async::Interface& ioContext, sockets::eShutdown how);
        qor_pp_module_interface(QOR_SOCKETS) virtual task<int32_t> AsyncShutdown(const async::Interface& ioContext, sockets::eShutdown how);
        qor_pp_module_interface(QOR_SOCKETS) virtual std::size_t ID(void);
        qor_pp_module_interface(QOR_SOCKETS) virtual int32_t GetLastError(void);
        qor_pp_module_interface(QOR_SOCKETS) virtual bool SetNonBlocking(bool nonBlocking);
        qor_pp_module_interface(QOR_SOCKETS) virtual bool IsAlive();
    };

    }}//io::network

    qor_pp_declare_factory_of(io::network::Socket, ExternalFactory);
    constexpr GUID SocketGUID = {0x53573d1b, 0x4f8c, 0x4620, {0xa6, 0x65, 0x5d, 0x58, 0x39, 0x03, 0xd4, 0xb8}};
    qor_pp_declare_guid_of(io::network::Socket,SocketGUID);

}//qor

#endif//QOR_PP_H_PLATFORM_NETWORK_SOCKET
