// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_PLUS
#define QOR_PP_H_COMPONENTS_JSON_PARSER_PLUS

#include "../tokens.h"

namespace qor { namespace components { namespace parser { namespace json {

    class plus : public Specific
    {
        public: plus(Parser* parser) : Specific(parser, (byte)0x2B, static_cast<uint64_t>(jsonToken::plus)){ }
        virtual ~plus() = default;
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_PLUS
