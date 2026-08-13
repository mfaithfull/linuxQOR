// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_VALUE_SEPARATOR
#define QOR_PP_H_DATA_JSON_PARSER_VALUE_SEPARATOR

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace json {

    //value-separator = ws %x2C ws  ; , comma
    class value_separator : public Sequence
    {
        public: value_separator(Parser* parser) :
            Sequence(parser,
                new_ref<ws>(parser),
                new_ref<Sequence>(parser,
                    new_ref<Specific<uint32_t>>(parser, 0x2c),
                    new_ref<ws>(parser)
                ),
            static_cast<uint64_t>(jsonToken::value_separator)){ }
        virtual ~value_separator() = default;
        virtual void Emit(){ };//Emit nothing
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_VALUE_SEPARATOR
