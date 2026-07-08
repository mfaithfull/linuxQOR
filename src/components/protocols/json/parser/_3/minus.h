// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_MINUS
#define QOR_PP_H_COMPONENTS_JSON_PARSER_MINUS

#include "../tokens.h"

namespace qor { namespace components { namespace parser { namespace json {

    class minus : public Specific
    {
    public: 
        minus(Parser* parser) : Specific(parser, 0x2D, static_cast<uint64_t>(jsonToken::minus)){ }
        virtual ~minus() = default;
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_MINUS
