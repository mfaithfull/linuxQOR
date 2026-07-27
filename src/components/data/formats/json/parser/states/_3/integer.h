// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_INTEGER
#define QOR_PP_H_DATA_JSON_PARSER_INTEGER

#include "../../tokens.h"
#include "../_2/digit1_9.h"
#include "../_4/zero.h"

namespace qor { namespace data { namespace parser { namespace json {

    class qor_pp_module_interface(QOR_JSON) integer : public AnyOneOf
    {
    public: 
        integer(Parser* parser);
        virtual ~integer();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_INTEGER
