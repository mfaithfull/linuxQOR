// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_SERVICEFILTER
#define QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_SERVICEFILTER

#include "src/framework/data/pipeline/inlinefilter.h"
#include "requestnode.h"
#include "echorequest.h"
#include "echorequestparser.h"

namespace qor { namespace components { namespace protocols { namespace echo {

    //Server side protocol filter
    
    class qor_pp_module_interface_gcc(QOR_ECHO) EchoServiceFilter : public qor::pipeline::InlineFilter<qor::byte>
    {
    public:

        qor_pp_module_interface(QOR_ECHO) EchoServiceFilter();
        qor_pp_module_interface(QOR_ECHO) EchoServiceFilter(size_t itemCount);
        qor_pp_module_interface(QOR_ECHO) virtual ~EchoServiceFilter();
        qor_pp_module_interface(QOR_ECHO) virtual size_t WriteAcknowledge(size_t& itemCount) override;

    private:

        void HandleRequest(ref_of<EchoRequest>::type Request);

        data::parser::Context m_sourceContext;
        data::Parser m_requestParser;
    };

}}}}//qor::components::protocols::echo

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_SERVICEFILTER
