// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_ASYNCIOINITIATOR
#define QOR_PP_H_FRAMEWORK_ASYNCIOINITIATOR

#include "types.h"
#include "src/framework/io/iodescriptor.h"
#include "src/framework/io/network/address.h"
#include "eventprocessor.h"
#include "src/qor/flyers/error/error.h"

namespace qor { namespace io{ namespace network{
    class Socket;
}}}//qor::io::network

namespace qor { namespace io{ namespace async{

    //queues up async io requests
    class qor_pp_module_interface(QOR_ASYNCIOSERVICE) Initiator
    {
    public:

        Initiator();
        Initiator(const Initiator&) = delete;
        Initiator& operator = (const Initiator&) = delete;
        virtual ~Initiator() noexcept;
        virtual void ConnectToProcessor(EventProcessor* /*processor*/);
        virtual bool RequiresBackgroundProcessor();
        virtual IOTask Bind(io::Descriptor* ioDescriptor, const io::network::Address& Address);
        virtual IOTask Listen(io::Descriptor* ioDescriptor, int backlog);
        virtual IOTask Accept(io::Descriptor* ioDescriptor, const io::network::Address& Address, io::network::Socket* Socket);
        virtual IOTask Read(io::Descriptor* ioDescriptor, byte* buffer, size_t len, long offset);
        virtual IOTask Write(io::Descriptor* ioDescriptor, byte* buffer, size_t len, long offset);
        virtual IOTask Send(io::Descriptor* ioDescriptor, const byte* buffer, size_t len, int flags);
        virtual IOTask Recv(io::Descriptor* ioDescriptor, byte* buffer, size_t len);
        virtual IOTask Shutdown(io::Descriptor* ioDescriptor, int how);
    };

    }}//io::async

    qor_pp_declare_factory_of(io::async::Initiator, ExternalFactory);
    constexpr GUID AsyncIOInitiatorGUID = {0xf21b2107, 0x2f64, 0x432e, {0xbe, 0xd5, 0x74, 0xae, 0xaf, 0x2c, 0x88, 0x10}};
    qor_pp_declare_guid_of(io::async::Initiator,AsyncIOInitiatorGUID);

}//qor

#endif//QOR_PP_H_FRAMEWORK_ASYNCIOINITIATOR