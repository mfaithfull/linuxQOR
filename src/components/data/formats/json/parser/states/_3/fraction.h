// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_FRACTION
#define QOR_PP_H_DATA_JSON_PARSER_FRACTION

#include "../../tokens.h"
#include "../_2/decimal_point.h"

namespace qor { namespace data { namespace parser { namespace json {

    class qor_pp_module_interface(QOR_JSON) fraction : public Sequence_t< decimal_point, Sequence_t< DIGIT<uint32_t>, ZeroOrMore_t< DIGIT<uint32_t> > > >
    {
    public: 
        fraction(Parser* parser);
        virtual ~fraction();

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

        decimal_point dp;
        DIGIT<uint32_t> pd;
        DIGIT<uint32_t> sd;
        ZeroOrMore_t<DIGIT<uint32_t>> continuation;
        Sequence_t< DIGIT<uint32_t>, ZeroOrMore_t< DIGIT<uint32_t> > > f;        

    };

}}}}//qor::data::parser::json

namespace qor{
    qor_pp_declare_source_of(data::parser::json::fraction, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<data::parser::json::fraction>::type, memory::FastSource)
}

#endif//QOR_PP_H_DATA_JSON_PARSER_FRACTION
