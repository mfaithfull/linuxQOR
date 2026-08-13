// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_BEGIN_ARRAY
#define QOR_PP_H_DATA_JSON_PARSER_BEGIN_ARRAY

#include "../../tokens.h"
#include "../_4/ws.h"

namespace qor { namespace data { namespace parser { namespace json {

    // begin-array     = ws %x5B ws  ; [ left square bracket
    class begin_array : public Sequence
    {
        public: begin_array(Parser* parser) :
            Sequence(parser,
                new_ref<ws>(parser),
                new_ref<Sequence>(parser,
                    new_ref<Specific<uint32_t>>(parser, (uint32_t)0x5B),
                    new_ref<ws>(parser)
                ),
            static_cast<uint64_t>(jsonToken::begin_array)){ }
        virtual ~begin_array() = default;
        virtual void Emit(){ };//Emit nothing
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_BEGIN_ARRAY
