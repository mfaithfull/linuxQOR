// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_UNICODESEQUENCE
#define QOR_PP_H_DATA_JSON_PARSER_UNICODESEQUENCE

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace json {

    class qor_pp_module_interface(QOR_JSON) unicodeSequence : public Sequence
    {
    public: 
        unicodeSequence(Parser* parser);
        virtual ~unicodeSequence();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_UNICODESEQUENCE
