// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_END_OBJECT
#define QOR_PP_H_COMPONENTS_JSON_PARSER_END_OBJECT

#include "../tokens.h"
#include "src/qor/flyers/log/debug.h"

namespace qor { namespace components { namespace parser { namespace json {

    //end-object      = ws %x7D ws  ; } right curly bracket
    class end_object : public Sequence
    {
        public: end_object(Parser* parser) :
            Sequence(parser,
                new_ref<ws>(parser),
                new_ref<Sequence>(parser,
                    new_ref<Specific>(parser, (byte)0x7D),
                    new_ref<ws>(parser)
                ),
            static_cast<uint64_t>(jsonToken::end_object)){ }
        virtual ~end_object() = default;
        virtual void Prepare()
        {
            log::debug("Looking for an end_object...");
        }

        void Emit()
        {
            log::debug("Consuming an end_object: ");
        }

        void Fail()
        {
            log::debug("...Didn't find an end_object.");
        }
        
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_END_OBJECT
