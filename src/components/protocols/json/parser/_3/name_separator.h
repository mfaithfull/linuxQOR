// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_NAME_SEPARATOR
#define QOR_PP_H_COMPONENTS_JSON_PARSER_NAME_SEPARATOR

#include "../tokens.h"
#include "../_4/ws.h"

namespace qor { namespace components { namespace parser { namespace json {

    //name-separator  = ws %x3A ws  ; : colon
    class name_separator : public Sequence
    {
        public: name_separator(Parser* parser) :
            Sequence(parser,
                new_ref<ws>(parser),
                new_ref<Sequence>(parser,
                    new_ref<Specific>(parser, (byte)0x3A),
                    new_ref<ws>(parser)
                ),
            static_cast<uint64_t>(jsonToken::name_separator)){ }
        virtual ~name_separator() = default;
        virtual void Prepare()
        {
            //log::debug("Looking for a name separator...");
        }

        void Emit()
        {
            //log::debug("Consuming a name separator: ");
        }

        void Fail()
        {
            //log::debug("...Didn't find a name separator.");
        }

    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_NAME_SEPARATOR
