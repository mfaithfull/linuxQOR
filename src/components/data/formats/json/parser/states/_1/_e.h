// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_E
#define QOR_PP_H_DATA_JSON_PARSER_E

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace json {

    class _e : public AnyOneOf_t< Specific<uint32_t>,Specific<uint32_t> >
    {
    public: 
        _e(Parser* parser) : AnyOneOf_t< Specific<uint32_t>,Specific<uint32_t> >(
            parser, &e1, &e2, static_cast<uint64_t>(jsonToken::_e)), 
            e1(parser, 0x65), e2(parser,0x45)
            { }
        virtual ~_e() = default;

    private:
        Specific<uint32_t> e1;
        Specific<uint32_t> e2;
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_E
