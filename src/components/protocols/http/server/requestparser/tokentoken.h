// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTPARSER_TOKENTOKEN
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTPARSER_TOKENTOKEN

#include "src/components/data/parser/state.h"
#include "src/components/data/parser/states/rfc5234.h"
#include "tokendefs.h"
#include "src/components/data/parser/states/oneormore.h"
#include "src/components/data/parser/nodes/char.h"
#include "tchartoken.h"
#include "nodes/tokennode.h"
#include "nodes/methodnode.h"
#include "nodes/fieldnamenode.h"
#include "nodes/protocolversionnode.h"

namespace qor { namespace components { namespace protocols { namespace http {

    class qor_pp_module_interface(QOR_HTTP) token : public parser::OneOrMore
    {
    public: token(parser::Parser* parser) : parser::OneOrMore(parser,
                    new_ref<tchar>(parser),
                    static_cast<uint64_t>(httpRequestToken::token)
                )
        {}

        virtual ~token() = default;
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

    class qor_pp_module_interface(QOR_HTTP) protocol_version : public parser::OneOrMore
    {
    public:
        protocol_version(parser::Parser* parser);
        virtual ~protocol_version();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

    class qor_pp_module_interface(QOR_HTTP) method : public parser::OneOrMore
    {
    public:
        method(parser::Parser* parser);
        virtual ~method();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

    class qor_pp_module_interface(QOR_HTTP) field_name : public parser::OneOrMore
    {
    public: field_name(parser::Parser* parser) : parser::OneOrMore(parser,
                    new_ref<tchar>(parser),
                    static_cast<uint64_t>(httpRequestToken::field_name)
                )
        {}

        virtual ~field_name() = default;
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

}}}}//qor::components::protocols::http

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTPARSER_TOKENTOKEN