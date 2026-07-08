// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_TRUE
#define QOR_PP_H_COMPONENTS_JSON_PARSER_TRUE

#include "../tokens.h"

namespace qor { namespace components { namespace parser { namespace json {

    //true  = %x74.72.75.65      ; true
    class qor_pp_module_interface(QOR_JSON) _true : public Sequence
    {
    public: 
        _true(Parser* parser);
        virtual ~_true();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_TRUE
