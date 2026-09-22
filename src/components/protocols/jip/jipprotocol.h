// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_JIP_PROTOCOL
#define QOR_PP_H_COMPONENTS_PROTOCOLS_JIP_PROTOCOL

#include "src/framework/data/pipeline/protocol.h"

namespace qor { namespace components { namespace protocols { namespace jip {

    class qor_pp_module_interface_gcc(QOR_JIP) JipProtocol : public qor::pipeline::Protocol
    {
    public:

        qor_pp_module_interface(QOR_JIP) JipProtocol();
        qor_pp_module_interface(QOR_JIP) virtual ~JipProtocol();
        qor_pp_module_interface(QOR_JIP) virtual io::network::sockets::eAddressFamily GetAddressFamily() const;
        qor_pp_module_interface(QOR_JIP) virtual ref_of<pipeline::InlineFilter<byte>>::type GetNewRequestFilter() const override;
        qor_pp_module_interface(QOR_JIP) virtual ref_of<pipeline::InlineFilter<byte>>::type GetNewResponseFilter() const override;        
    };
}}}}

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_JIP_PROTOCOL

