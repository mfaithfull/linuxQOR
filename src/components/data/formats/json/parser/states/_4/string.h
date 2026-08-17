// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_STRING
#define QOR_PP_H_DATA_JSON_PARSER_STRING

#include "../../tokens.h"
#include "../_2/character.h"
#include "../_4/quotation_mark.h"

namespace qor { namespace data { namespace parser { namespace json {

    class qor_pp_module_interface(QOR_JSON) string : public Sequence_t< quotation_mark, Sequence_t< ZeroOrMore_t< character >, quotation_mark > >
    {
    public: 
        string(Parser* parser);
        virtual ~string();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    private:

        quotation_mark m_qopen;
        quotation_mark m_qclose;
        character m_char;
        ZeroOrMore_t< character > m_body;
        Sequence_t< ZeroOrMore_t< character >, quotation_mark > m_tail;
        static UTF8CodePage s_codePage;
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_STRING
