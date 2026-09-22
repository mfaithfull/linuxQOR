// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_JIP_RESPONSEFILTER
#define QOR_PP_H_COMPONENTS_PROTOCOLS_JIP_RESPONSEFILTER

#include "src/framework/data/pipeline/inlinefilter.h"
#include "src/components/data/parser/context.h"
//#include "jipresponseparser.h"
//#include "responsenode.h"
#include "echoresponse.h"

namespace qor { namespace components { namespace protocols { namespace jip {

    class qor_pp_module_interface_gcc(QOR_JIP) JipResponseFilter : public qor::pipeline::InlineFilter<qor::byte>
    {
    public:

        qor_pp_module_interface(QOR_JIP) JipResponseFilter();
        qor_pp_module_interface(QOR_JIP) JipResponseFilter(size_t itemCount);
        qor_pp_module_interface(QOR_JIP) virtual ~JipResponseFilter();
        qor_pp_module_interface(QOR_JIP) virtual size_t WriteAcknowledge(size_t& itemCount);

    private:

        void HandleResponse(ref_of<JipResponse>::type Response);

        data::parser::Context m_sourceContext;
        data::Parser m_responseParser;
    };

}}}}//qor::components::protocols::jip

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_JIP_RESPONSEFILTER
