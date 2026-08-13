// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_END_ARRAY
#define QOR_PP_H_DATA_JSON_PARSER_END_ARRAY

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace json {

    //end-array       = ws %x5D ws  ; ] right square bracket
    class end_array : public Sequence
    {
        public: end_array(Parser* parser) :
            Sequence(parser,
                new_ref<ws>(parser),
                new_ref<Sequence>(parser,
                    new_ref<Specific<uint32_t>>(parser, 0x5D),
                    new_ref<ws>(parser)
                ),
            static_cast<uint64_t>(jsonToken::end_array)){ }
        virtual ~end_array() = default;
        virtual void Emit(){ };//Emit nothing
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_END_ARRAY
