// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_MEMBER
#define QOR_PP_H_DATA_JSON_PARSER_MEMBER

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace json {

    //member = string name-separator value
    class qor_pp_module_interface(QOR_JSON) member : public Sequence
    {
        public: member(Parser* parser);
        virtual ~member();

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_MEMBER
