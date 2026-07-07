// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/io/async/context.h"
#include "sockets.h"

namespace qor{ namespace io { namespace network{

    Sockets::Sockets() = default;

    Sockets::~Sockets() = default;

    void Sockets::Setup(){ }

    void Sockets::Shutdown(){ }

    ref_of<Socket>::type Sockets::CreateSocket(sockets::eAddressFamily /*AF*/, sockets::eType /*Type*/, sockets::eProtocol /*Protocol*/, ref_of<async::Context::Session>::type /*ioSession*/) const
    {
        ref_of<Socket>::type nulref;
        return nulref;
    }

    int Sockets::GetAddressInfo(const std::string& /*node*/, const std::string& /*service*/, const AddressInfo& /*hints*/, std::vector<AddressInfo>& /*results*/) const
    {
        return -1;
    }

}}}//qor::io::network
