// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_SERVICEFILTER
#define QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_SERVICEFILTER

#include "src/framework/data/pipeline/inlinefilter.h"
#include "requestnode.h"
#include "echorequest.h"

namespace qor { namespace components { namespace protocols { namespace echo {

    //Server side protocol filter
    
    class qor_pp_module_interface_gcc(QOR_ECHO) EchoServiceFilter : public qor::pipeline::InlineFilter<qor::byte>
    {
    public:

        qor_pp_module_interface(QOR_ECHO) EchoServiceFilter();
        qor_pp_module_interface(QOR_ECHO) EchoServiceFilter(size_t itemCount);
        qor_pp_module_interface(QOR_ECHO) virtual ~EchoServiceFilter();
        qor_pp_module_interface(QOR_ECHO) qor::ref_of<EchoRequest>::type Parse(qor::byte* data, size_t& itemCount);
        qor_pp_module_interface(QOR_ECHO) virtual void Filter(qor::byte* space, qor::byte* data, size_t& itemCount, size_t& writeCount);

    };

}}}}//qor::components::protocols::echo

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_SERVICEFILTER
