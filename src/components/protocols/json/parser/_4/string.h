// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_STRING
#define QOR_PP_H_COMPONENTS_JSON_PARSER_STRING

#include "../tokens.h"
#include "../_2/character.h"
#include "../_4/quotation_mark.h"

namespace qor { namespace components { namespace parser { namespace json {

    class qor_pp_module_interface(QOR_JSON) string : public Sequence
    {
    public: 
        string(Parser* parser);
        virtual ~string();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_STRING
