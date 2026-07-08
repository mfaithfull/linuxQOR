// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_ARRAY
#define QOR_PP_H_COMPONENTS_JSON_PARSER_ARRAY

#include "../tokens.h"

namespace qor { namespace components { namespace parser { namespace json {

    //array = begin-array [ value *( value-separator value ) ] end-array
    class qor_pp_module_interface(QOR_JSON) array : public Sequence
    {
    public: 
        array(Parser* parser);
        virtual ~array();

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_ARRAY
