// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_FALSE
#define QOR_PP_H_DATA_JSON_PARSER_FALSE

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace json {

    //false = %x66.61.6c.73.65   ; false
    class qor_pp_module_interface(QOR_JSON) _false : public Sequence
    {
    public: 
        _false(Parser* parser);
        virtual ~_false();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_FALSE
