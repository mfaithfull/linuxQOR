// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_CHARACTER
#define QOR_PP_H_COMPONENTS_JSON_PARSER_CHARACTER

#include "../tokens.h"
#include "escapesequence.h"
#include "../_4/unescaped.h"

namespace qor { namespace components { namespace parser { namespace json {

    class qor_pp_module_interface(QOR_JSON) character : public AnyOneOf
    {
    public: 
        character(Parser* parser);
        virtual ~character();
        virtual void Emit();
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_CHARACTER
