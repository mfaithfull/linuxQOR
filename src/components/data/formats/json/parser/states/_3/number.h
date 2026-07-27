// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_NUMBER
#define QOR_PP_H_DATA_JSON_PARSER_NUMBER

#include "../../tokens.h"
#include "minus.h"
#include "integer.h"
#include "fraction.h"
#include "../_2/exp.h"

namespace qor { namespace data { namespace parser { namespace json {

    //number = [ minus ] int [ frac ] [ exp ]
    class qor_pp_module_interface(QOR_JSON) number : public Sequence
    {
    public: 
        number(Parser* parser);
        virtual ~number();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_NUMBER
