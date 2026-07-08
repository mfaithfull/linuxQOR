// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_DIGIT1_9
#define QOR_PP_H_COMPONENTS_JSON_PARSER_DIGIT1_9

#include "../tokens.h"

namespace qor { namespace components { namespace parser { namespace json {

    class qor_pp_module_interface(QOR_JSON) digit1_9 : public OneOfARange
    {
        public: digit1_9(Parser* parser);
        virtual ~digit1_9();
        virtual void Emit();
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_DIGIT1_9
