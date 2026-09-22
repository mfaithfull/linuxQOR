// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_JIP_SERVICEFILTER
#define QOR_PP_H_COMPONENTS_PROTOCOLS_JIP_SERVICEFILTER

#include "src/framework/data/pipeline/inlinefilter.h"
#include "src/components/data/parser/context.h"
#include "requestnode.h"
#include "jiprequest.h"

namespace qor { namespace components { namespace protocols { namespace jip {

    //Server side protocol filter

    class qor_pp_module_interface_gcc(QOR_JIP) JipServiceFilter : public qor::pipeline::InlineFilter<qor::byte>
    {
    public:

        qor_pp_module_interface(QOR_JIP) JipServiceFilter();
        qor_pp_module_interface(QOR_JIP) JipServiceFilter(size_t itemCount);
        qor_pp_module_interface(QOR_JIP) virtual ~JipServiceFilter();
        qor_pp_module_interface(QOR_JIP) virtual size_t WriteAcknowledge(size_t& itemCount) override;

    private:

        void HandleRequest(ref_of<JipRequest>::type Request);

        data::parser::Context m_sourceContext;
        data::Parser m_requestParser;
    };

}}}}//qor::components::protocols::jip

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_JIP_SERVICEFILTER
