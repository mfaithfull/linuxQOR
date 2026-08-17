// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_INTEGER
#define QOR_PP_H_DATA_JSON_PARSER_INTEGER

#include "../../tokens.h"
#include "../_2/digit1_9.h"
#include "../_4/zero.h"

namespace qor { namespace data { namespace parser { namespace json {

    class qor_pp_module_interface(QOR_JSON) integer : public AnyOneOf_t< zero, Sequence_t<digit1_9, ZeroOrMore_t<DIGIT<uint32_t>>> >
    {
    public: 
        integer(Parser* parser);
        virtual ~integer();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    private:

        zero m_zero;
        digit1_9 m_d;
        DIGIT<uint32_t> m_td;
        ZeroOrMore_t<DIGIT<uint32_t>> m_tail;
        Sequence_t<digit1_9, ZeroOrMore_t<DIGIT<uint32_t>>> m_body;
        

    };

}}}}//qor::data::parser::json

namespace qor{
    qor_pp_declare_source_of(data::parser::json::integer, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<data::parser::json::integer>::type, memory::FastSource)
}

#endif//QOR_PP_H_DATA_JSON_PARSER_INTEGER
