// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSETOKEN
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSETOKEN

#include "src/components/data/parser/state.h"
#include "src/components/data/parser/states/rfc5234.h"
#include "tokendefs.h"
#include "src/components/data/parser/states/oneormore.h"
#include "src/components/data/parser/nodes/char.h"
#include "request_linetoken.h"
#include "field_linetoken.h"

namespace qor { namespace components { namespace protocols { namespace http { namespace response {

    class qor_pp_module_interface(QOR_HTTP) Initial : public qor::data::parser::Sequence
    {
    public: initial(qor::data::Parser* parser) :
                qor::data::parser::Sequence( parser,
                    new_ref<status_line>(parser),
                    new_ref<qor::data::parser::Sequence>( parser,
                        new_ref<qor::data::parser::ZeroOrMore>( parser,
                            new_ref<field_line>(parser)
                        ),
                        new_ref<qor::data::parser::CRLF>(parser)
                    ),
                    static_cast<uint64_t>(httpResponseToken::response))
            {}

        virtual ~initial() = default;

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail()
        {
            auto node = GetParser()->PopNode();
            if(node.IsNotNull() && node->GetToken() != m_token)
            {
                GetParser()->PushNode(node);
            }
        }
    };
    
}}}}}//qor::components::protocols::http::response

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSETOKEN