// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTSOURCE
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTSOURCE

#include <string>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/data/pipeline/iosource.h"
#include "../../request/request.h"
#include "../requestgenerator/requestgenerator.h"
#include "../requestgenerator/context.h"

namespace qor { namespace components { namespace protocols { namespace http {
    
    class qor_pp_module_interface(QOR_HTTP) HTTPSource : public pipeline::Source
    {

    public:

        HTTPSource();
        virtual ~HTTPSource() = default;        
        void SetRequest(ref_of<HTTPRequest>::type request);

    protected:

        size_t ReadBytes(byte* space, size_t bytesToRead);
        
        ref_of<Context>::type m_context;
        HTTPRequestGenerator m_generator;        
    };


}}}}//qor::components::protocols::http

#endif // QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTSOURCE
