// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_E
#define QOR_PP_H_COMPONENTS_JSON_PARSER_E

#include "../tokens.h"

namespace qor { namespace components { namespace parser { namespace json {

    class _e : public AnyOneOf
    {
        public: _e(Parser* parser) :
            AnyOneOf(parser, 
                new_ref<Specific>(parser, (byte)0x65),
                new_ref<Specific>(parser, (byte)0x45),
                static_cast<uint64_t>(jsonToken::_e)){ }
        virtual ~_e() = default;
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_E
