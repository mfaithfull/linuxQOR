// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTGENERATOR
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTGENERATOR

#include <string>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/app/workflow/workflow.h"
#include "../../request/request.h"
#include "context.h"

namespace qor { namespace components { namespace protocols { namespace http {
    
    class qor_pp_module_interface(QOR_HTTP) HTTPRequestGenerator : public Workflow
    {

    public:

        HTTPRequestGenerator(ref_of<Context>::type context);
        virtual ~HTTPRequestGenerator() = default;
        virtual int Run();
        
        inline Context* GetContext()
        {
            return m_context;
        }

        inline void SetContext(ref_of<class Context>::type context)
        {
            m_context = context;
        }        

        inline ref_of<HTTPRequest>::type GetRequest()
        {
            return m_request;
        }

        inline void SetRequest(ref_of<HTTPRequest>::type request)
        {
            m_request = request;
        }

    private:

        ref_of<Context>::type m_context;
        ref_of<HTTPRequest>::type m_request;
    };

}}}}//qor::components::protocols::http

#endif // QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTGENERATOR