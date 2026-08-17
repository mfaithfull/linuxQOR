// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_RESPONSEFILTER
#define QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_RESPONSEFILTER

#include "src/framework/data/pipeline/inlinefilter.h"
#include "src/components/data/parser/context.h"
#include "echoresponseparser.h"
#include "responsenode.h"
#include "echoresponse.h"

namespace qor { namespace components { namespace protocols { namespace echo {

    //Buffer analogue used client side to hold and parse response    
    class qor_pp_module_interface_gcc(QOR_ECHO) EchoResponseFilter : public qor::pipeline::InlineFilter<qor::byte>
    {
    public:

        qor_pp_module_interface(QOR_ECHO) EchoResponseFilter();
        qor_pp_module_interface(QOR_ECHO) EchoResponseFilter(size_t itemCount);
        qor_pp_module_interface(QOR_ECHO) virtual ~EchoResponseFilter();
        qor_pp_module_interface(QOR_ECHO) virtual size_t WriteAcknowledge(size_t& itemCount);

    private:

        void HandleResponse(ref_of<EchoResponse>::type Response);

        pipeline::BufferContext<byte> m_sourceContext;
        data::Parser m_responseParser;
    };

}}}}//qor::components::protocols::echo

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_RESPONSEFILTER
