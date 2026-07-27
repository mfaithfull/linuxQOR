// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "echoprotocol.h"

namespace qor { namespace components { namespace protocols { namespace echo {

    //Server side request and response protocol pipeline
    
    EchoProtocol::EchoProtocol() : qor::pipeline::Protocol()
    {
        m_requestFilter = new_ref<EchoServiceFilter>();
        m_responseFilter = new_ref<EchoResponseFilter>();
    }
    
    EchoProtocol::~EchoProtocol() = default;

    io::network::sockets::eAddressFamily EchoProtocol::GetAddressFamily() const
    {
        return io::network::sockets::eAddressFamily::AF_INet;
    }

    ref_of<pipeline::InlineFilter<byte>>::type EchoProtocol::GetRequestFilter()
    {            
        return m_requestFilter;
    }

    ref_of<pipeline::InlineFilter<byte>>::type EchoProtocol::GetResponseFilter()
    {            
        return m_responseFilter;
    }

    size_t EchoProtocol::GetMaxEchoSize()
    {
        return maxEchoSize;
    }

}}}}//qor::components::protocols::echo