// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_EXP
#define QOR_PP_H_COMPONENTS_JSON_PARSER_EXP

#include "../tokens.h"
#include "../_3/minus.h"
#include "../_4/plus.h"
#include "../_1/_e.h"

namespace qor { namespace components { namespace parser { namespace json {

    //exp = e [ minus / plus ] 1*DIGIT
    class qor_pp_module_interface(QOR_JSON) exp : public Sequence
    {
    public: 
        exp(Parser* parser);
        virtual ~exp();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_EXP
