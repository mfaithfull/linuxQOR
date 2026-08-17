// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_CHARACTER
#define QOR_PP_H_DATA_JSON_PARSER_CHARACTER

#include "../../tokens.h"
#include "escapesequence.h"
#include "../_4/unescaped.h"

namespace qor { namespace data { namespace parser { namespace json {

    class qor_pp_module_interface(QOR_JSON) character : public AnyOneOf_t< unescaped, deferred<escapeSequence> >
    {
    public: 
        character(Parser* parser);
        virtual ~character();
        virtual void Emit();

    private:
        unescaped m_un;
        deferred<escapeSequence> m_escaped;
        static UTF8CodePage s_codePage;
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_CHARACTER
