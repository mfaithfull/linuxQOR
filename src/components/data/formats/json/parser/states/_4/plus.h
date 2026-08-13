// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_PLUS
#define QOR_PP_H_DATA_JSON_PARSER_PLUS

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace json {

    class plus : public Specific<uint32_t>
    {
        public: plus(Parser* parser) : Specific<uint32_t>(parser, 0x2B, static_cast<uint64_t>(jsonToken::plus)){ }
        virtual ~plus() = default;
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_PLUS
