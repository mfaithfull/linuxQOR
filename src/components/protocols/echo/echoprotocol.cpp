// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "echoprotocol.h"

namespace qor { namespace components { namespace protocols { namespace echo {

    //Server side request and response protocol pipeline
    
    EchoProtocol::EchoProtocol() : qor::pipeline::Protocol(){ }
    
    EchoProtocol::~EchoProtocol() = default;

    io::network::sockets::eAddressFamily EchoProtocol::GetAddressFamily() const
    {
        return io::network::sockets::eAddressFamily::AF_INet;
    }

    ref_of<pipeline::InlineFilter<byte>>::type EchoProtocol::GetNewRequestFilter() const
    {            
        return new_ref<EchoServiceFilter>();
    }

    ref_of<pipeline::InlineFilter<byte>>::type EchoProtocol::GetNewResponseFilter() const
    {            
        return new_ref<EchoResponseFilter>();
    }

    size_t EchoProtocol::GetMaxEchoSize() const
    {
        return maxEchoSize;
    }

}}}}//qor::components::protocols::echo