// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_NULL
#define QOR_PP_H_COMPONENTS_JSON_PARSER_NULL

#include "../tokens.h"

namespace qor { namespace components { namespace parser { namespace json {

    //null  = %x6e.75.6c.6c      ; null    
    class qor_pp_module_interface(QOR_JSON) _null : public Sequence
    {
    public: 
        _null(Parser* parser);
        virtual ~_null();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_NULL
