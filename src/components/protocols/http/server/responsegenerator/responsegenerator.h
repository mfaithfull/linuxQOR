// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSEGENERATOR
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSEGENERATOR

#include <string>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/app/workflow/workflow.h"
#include "../../response/response.h"
#include "src/components/data/parser/context.h"

namespace qor { namespace components { namespace protocols { namespace http { namespace response {
    
    class qor_pp_module_interface(QOR_HTTP) Generator : public Workflow
    {

    public:

        Generator(ref_of<parser::Context>::type context);
        virtual inline ~Generator() = default;
        virtual int Run();

        inline parser::Context* GetContext()
        {
            return m_context;
        }

        inline void SetContext(ref_of<parser::Context>::type context)
        {
            m_context = context;
        }

        inline ref_of<HTTPResponse>::type GetResponse()
        {
            return m_response;
        }

        inline void SetResponse(ref_of<HTTPResponse>::type response)
        {
            m_response = response;
        }

    private:

        ref_of<parser::Context>::type m_context;
        ref_of<HTTPResponse>::type m_response;
    };

}}}}}//qor::components::protocols::http::response

#endif // QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSEGENERATOR