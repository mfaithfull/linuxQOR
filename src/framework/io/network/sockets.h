// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_NETWORK_SOCKETS
#define QOR_PP_H_PLATFORM_NETWORK_SOCKETS

#include <vector>
#include "src/platform/isubsystem.h"
#include "socket.h"
#include "addressinfo.h"
#include "src/framework/io/async/context.h"

//All libraries providing an implementation of Sockets also need to export this function so that the linker can find them
namespace qor{ bool qor_pp_module_interface(QOR_SOCKETS) ImplementsSockets();}

namespace qor{ namespace io { namespace network{

    //Empty base implementation of Sockets subsystem
    class qor_pp_module_interface(QOR_SOCKETS) Sockets : public ISubsystem
    {
    public:

        Sockets() = default;
        virtual ~Sockets() = default;

        virtual void Setup();
        virtual void Shutdown();

        virtual ref_of<Socket>::type CreateSocket(const sockets::eAddressFamily AF, const sockets::eType Type, const sockets::eProtocol Protocol, ref_of<async::Context::Session>::type ioSession) const;
        virtual int GetAddressInfo(const std::string& node, const std::string& service, const AddressInfo& hints, std::vector<AddressInfo>& results) const;

    private:

        Sockets(const Sockets&) = delete;
        Sockets& operator = (const Sockets&) = delete;
    };

    }}//io::network

    qor_pp_declare_factory_of(io::network::Sockets, ExternalFactory);
    constexpr GUID SocketsGUID = {0xec0a4971, 0x76ed, 0x4afd, {0x89, 0x23, 0x45, 0x3d, 0x5a, 0x05, 0x49, 0x5e}};
    qor_pp_declare_guid_of(io::network::Sockets,SocketsGUID);

}//qor

#endif//QOR_PP_H_PLATFORM_NETWORK_SOCKETS
