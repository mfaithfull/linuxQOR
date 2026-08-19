// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_PROTOCOL
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_PROTOCOL

#include "src/framework/data/pipeline/protocol.h"
#include "filter.h"

namespace qor { namespace components { namespace protocols { namespace http {

    class qor_pp_module_interface(QOR_HTTP) HTTPProtocol : public qor::pipeline::Protocol
    {
    public:

        HTTPProtocol();
        virtual ~HTTPProtocol();
        virtual io::network::sockets::eAddressFamily GetAddressFamily() const override;
        virtual ref_of<qor::pipeline::InlineFilter<byte>>::type GetNewRequestFilter() const override;
        virtual ref_of<qor::pipeline::InlineFilter<byte>>::type GetNewResponseFilter() const override;
        virtual size_t GetRequestBufferSize();
        virtual size_t GetResponseBufferSize();
        static std::string GetReasonPhrase(unsigned int statusCode);
    };
}}}}

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_PROTOCOL

