// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_UNESCAPED
#define QOR_PP_H_COMPONENTS_JSON_PARSER_UNESCAPED

#include "../tokens.h"

namespace qor { namespace components { namespace parser { namespace json {

    class unescaped : public components::parser::AnyOneOf
    {        
    public: unescaped(Parser* parser) :
                AnyOneOf(parser,
                    new_ref<OneOfARange>(parser, (byte)0x20, (byte)0x21, static_cast<uint64_t>(eToken::Lexical)),
                    new_ref<AnyOneOf>(parser,
                        new_ref<OneOfARange>(parser, (byte)0x23, (byte)0x5B, static_cast<uint64_t>(eToken::Lexical)),
                        new_ref<OneOfARange>(parser, (byte)0x5D, (byte)(0xFF)/*0x10FFFF*/, static_cast<uint64_t>(eToken::Lexical)),
                        static_cast<uint64_t>(eToken::Lexical)
                    ),
                    static_cast<uint64_t>(jsonToken::unescaped)){ }

        virtual ~unescaped() = default;
        virtual void Prepare()
        {
            //log::debug("Looking for an unescaped character...");
        }

        void Emit()
        {
            //log::debug("Emitting an unescaped character: ");
        }

        void Fail()
        {
            //log::debug("...Didn't find an unescaped character.");
        }
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_UNESCAPED