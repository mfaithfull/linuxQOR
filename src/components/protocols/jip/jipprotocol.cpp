// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "jipprotocol.h"
#include "jipservicefilter.h"
#include "jipresponsefilter.h"

namespace qor { namespace components { namespace protocols { namespace jip {    
    
    JipProtocol::JipProtocol() : qor::pipeline::Protocol(){ }
    
    JipProtocol::~JipProtocol() = default;

    io::network::sockets::eAddressFamily JipProtocol::GetAddressFamily() const
    {
        return io::network::sockets::eAddressFamily::AF_INet;
    }

    ref_of<pipeline::InlineFilter<byte>>::type JipProtocol::GetNewRequestFilter() const
    {            
        return new_ref<JipServiceFilter>();
    }

    ref_of<pipeline::InlineFilter<byte>>::type JipProtocol::GetNewResponseFilter() const
    {            
        return new_ref<JipResponseFilter>();
    }

}}}}//qor::components::protocols::jip