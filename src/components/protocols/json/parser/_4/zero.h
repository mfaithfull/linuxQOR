// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_ZERO
#define QOR_PP_H_COMPONENTS_JSON_PARSER_ZERO

#include "../tokens.h"

namespace qor { namespace components { namespace parser { namespace json {

    class zero : public Specific
    {
    public: 
        zero(Parser* parser) : Specific(parser, (byte)0x30, static_cast<uint64_t>(jsonToken::zero)){ }
        virtual ~zero() = default;
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_ZERO
