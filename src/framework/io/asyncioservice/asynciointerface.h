// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_ASYNCIOINTERFACE
#define QOR_PP_H_FRAMEWORK_ASYNCIOINTERFACE

#include "asynciotypes.h"
#include <future>
#include "src/framework/io/network/address.h"

namespace qor { namespace io{ namespace network {
    class qor_pp_module_interface(QOR_SOCKETS) Socket;
}}}//qor::io::network

namespace qor { namespace io{ namespace async{

    class AsyncIOInterface
    {
    public:

        inline AsyncIOInterface() = default;
        inline virtual ~AsyncIOInterface() = default;

        virtual inline task<int> Send(io::IODescriptor* ioDescriptor, byte* buffer, size_t len, int flags) const = 0;
        virtual inline task<int> Recv(io::IODescriptor* ioDescriptor, byte* buffer, size_t len) const = 0;
        virtual inline task<int> Read(io::IODescriptor* ioDescriptor, byte* buffer, size_t len, long offset) const = 0;
        virtual inline task<int> Write(io::IODescriptor* ioDescriptor, byte* buffer, size_t len, long offset) const = 0;
        virtual inline task<int> Listen(io::IODescriptor* ioDescriptor, int backlog) const = 0;
        virtual inline task<int> Shutdown(io::IODescriptor* ioDescriptor, int how) const = 0;
        virtual inline task<int> Bind(io::IODescriptor* ioDescriptor, const io::network::Address& Address) const = 0;
        virtual inline task<int> Accept(io::IODescriptor* ioDescriptor, const io::network::Address& Address, io::network::Socket* Socket) const = 0;
        virtual inline bool Enroll(io::IODescriptor& ioDescriptor) const = 0;
    };

}}}//qor::io::async

#endif//QOR_PP_H_FRAMEWORK_ASYNCIOINTERFACE