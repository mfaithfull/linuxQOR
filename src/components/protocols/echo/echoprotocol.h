// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_PROTOCOL
#define QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_PROTOCOL

#include "src/framework/data/pipeline/protocol.h"
#include "echoservicefilter.h"
#include "echoresponsefilter.h"

namespace qor { namespace components { namespace protocols { namespace echo {

    constexpr size_t maxEchoSize = 1024;

    //Request and response pipeline protocol
    class qor_pp_module_interface_gcc(QOR_ECHO) EchoProtocol : public qor::pipeline::Protocol
    {
    public:

        qor_pp_module_interface(QOR_ECHO) EchoProtocol();
        qor_pp_module_interface(QOR_ECHO) virtual ~EchoProtocol();
        qor_pp_module_interface(QOR_ECHO) virtual io::network::sockets::eAddressFamily GetAddressFamily() const;
        qor_pp_module_interface(QOR_ECHO) virtual ref_of<pipeline::InlineFilter<byte>>::type GetRequestFilter() override;
        qor_pp_module_interface(QOR_ECHO) virtual ref_of<pipeline::InlineFilter<byte>>::type GetResponseFilter() override;
        qor_pp_module_interface(QOR_ECHO) size_t GetMaxEchoSize();

    private:

        ref_of<EchoServiceFilter>::type m_requestFilter;
        ref_of<EchoResponseFilter>::type m_responseFilter;

    };
}}}}

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_PROTOCOL

