// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_E
#define QOR_PP_H_DATA_JSON_PARSER_E

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace json {

    class _e : public AnyOneOf
    {
        public: _e(Parser* parser) :
            AnyOneOf(parser, 
                new_ref<Specific<uint32_t>>(parser, (uint32_t)0x65),
                new_ref<Specific<uint32_t>>(parser, (uint32_t)0x45),
                static_cast<uint64_t>(jsonToken::_e)){ }
        virtual ~_e() = default;
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_E
