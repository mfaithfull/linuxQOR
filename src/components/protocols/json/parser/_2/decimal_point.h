// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_DECIMAL_POINT
#define QOR_PP_H_COMPONENTS_JSON_PARSER_DECIMAL_POINT

#include "../tokens.h"

namespace qor { namespace components { namespace parser { namespace json {

    class decimal_point : public Specific
    {
        public: decimal_point(Parser* parser) : Specific(parser, (byte)0x2E, static_cast<uint64_t>(jsonToken::decimal_point)){ }
        virtual ~decimal_point() = default;
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_DECIMAL_POINT
