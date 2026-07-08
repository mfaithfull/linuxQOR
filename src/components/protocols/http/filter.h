// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_SERVICEFILTER
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_SERVICEFILTER

#include "src/framework/data/pipeline/inlinefilter.h"
#include "server/requestparser/nodes/requestnode.h"
#include "server/requestparser/requesttoken.h"
#include "request/request.h"
#include "server/responsegenerator/responsegenerator.h"

namespace qor { namespace components { namespace protocols { namespace http {

    class qor_pp_module_interface_gcc(QOR_HTTP) HTTPFilter : public qor::pipeline::InlineFilter<qor::byte>
    {
    public:

        qor_pp_module_interface(QOR_HTTP) HTTPFilter(size_t itemCount = 0);
        qor_pp_module_interface(QOR_HTTP) virtual ~HTTPFilter();
        qor_pp_module_interface(QOR_HTTP) virtual void Filter(qor::byte* space, qor::byte* data, size_t& itemCount, size_t& writeCount);

    private:

        qor::ref_of<HTTPRequest>::type Parse(qor::byte* data, size_t& itemCount);

    };

}}}}//qor::components::protocols::http

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_SERVICEFILTER
