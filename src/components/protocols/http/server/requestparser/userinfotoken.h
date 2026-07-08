// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTPARSER_USERINFOTOKEN
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTPARSER_USERINFOTOKEN

#include "src/components/data/parser/state.h"
#include "src/components/data/parser/states/rfc5234.h"
#include "tokendefs.h"
#include "src/components/data/parser/states/oneormore.h"
#include "src/components/data/parser/nodes/char.h"
#include "nodes/userinfonode.h"
#include "unreservedtoken.h"
#include "pct_encodedtoken.h"
#include "sub_delimstoken.h"

namespace qor { namespace components { namespace protocols { namespace http {

    //userinfo      = *( unreserved / pct-encoded / sub-delims / ":" )
    class qor_pp_module_interface(QOR_HTTP) userinfo : public qor::components::parser::ZeroOrMore
    {
    public: userinfo(qor::components::parser::Parser* parser) :
                qor::components::parser::ZeroOrMore(parser,
                    new_ref<qor::components::parser::AnyOneOfSet>(parser,
                        new std::vector<ref_of<ParserState>::type>({{
                            new_ref<unreserved>(parser),
                            new_ref<pct_encoded>(parser),
                            new_ref<sub_delims>(parser),
                            new_ref<qor::components::parser::Specific>(parser, ':')
                            }})                            
                    ),
                    static_cast<uint64_t>(httpRequestToken::userinfo)
                )
        {}

        virtual ~userinfo() = default;
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    };

}}}}//qor::components::protocols::http

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTPARSER_USERINFOTOKEN