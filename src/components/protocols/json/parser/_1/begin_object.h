// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_BEGIN_OBJECT
#define QOR_PP_H_COMPONENTS_JSON_PARSER_BEGIN_OBJECT

#include "../tokens.h"
#include "../_4/ws.h"

namespace qor { namespace components { namespace parser { namespace json {

    //begin-object    = ws %x7B ws  ; { left curly bracket
    class begin_object : public Sequence
    {
        public: begin_object(Parser* parser) :
            Sequence(parser,
                new_ref<ws>(parser),
                new_ref<Sequence>(parser,
                    new_ref<Specific>(parser, (byte)0x7B),
                    new_ref<ws>(parser)
                ),
            static_cast<uint64_t>(jsonToken::begin_object)){ }
        virtual ~begin_object() = default;
        virtual void Emit(){ };//Emit nothing
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_BEGIN_OBJECT
