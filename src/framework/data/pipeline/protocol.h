// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_NETWORK_PROTOCOL
#define QOR_PP_H_PIPELINE_NETWORK_PROTOCOL

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/io/network/socketdefs.h"
#include "source.h"
#include "sink.h"
#include "inlinefilter.h"

namespace qor{ namespace pipeline{

    //Base type for pipeline compatible network protocols
    class Protocol
    {
    public:

        Protocol(){}
        virtual ~Protocol(){}

        virtual qor::io::network::sockets::eAddressFamily GetAddressFamily() const
        {
            return qor::io::network::sockets::eAddressFamily::AF_Unspecified;
        }

        virtual qor::io::network::sockets::eType FramingType() const
        {
            return qor::io::network::sockets::eType::Sock_Stream;
        }

        virtual qor::io::network::sockets::eProtocol ProtocolType() const
        {
            return qor::io::network::sockets::eProtocol::IPProto_IP;
        }

        virtual qor::ref_of<qor::pipeline::InlineFilter<byte>>::type GetRequestFilter()
        {
            return ref_of<qor::pipeline::InlineFilter<byte>>::type();
        }

        virtual qor::ref_of<qor::pipeline::InlineFilter<byte>>::type GetResponseFilter()
        {
            return ref_of<qor::pipeline::InlineFilter<byte>>::type();
        }

    };

}}//qor::pipeline


#endif//QOR_PP_H_PIPELINE_NETWORK_PROTOCOL