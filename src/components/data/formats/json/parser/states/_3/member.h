// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_MEMBER
#define QOR_PP_H_DATA_JSON_PARSER_MEMBER

#include "../../tokens.h"
#include "../_4/string.h"
#include "name_separator.h"
#include "../_4/value.h"

namespace qor { namespace data { namespace parser { namespace json {

    //member = string name-separator value
    class qor_pp_module_interface(QOR_JSON) member : public Sequence_t< string, Sequence_t<name_separator, value> >
    {
        public: member(Parser* parser);
        virtual ~member();

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    private:
        value m_v;
        name_separator m_ns;
        string m_str;
        Sequence_t<name_separator, value> m_tail;
        
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_MEMBER
