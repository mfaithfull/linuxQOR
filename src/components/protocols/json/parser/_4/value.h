// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_VALUE
#define QOR_PP_H_COMPONENTS_JSON_PARSER_VALUE

#include "../tokens.h"

namespace qor { namespace components { namespace parser { namespace json {

    //value = false / null / true / object / array / number / string
    class qor_pp_module_interface(QOR_JSON) value : public AnyOneOf
    {
        public: value(Parser* parser);
        virtual ~value();

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_VALUE
