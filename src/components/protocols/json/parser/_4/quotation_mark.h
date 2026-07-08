// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_QUOTATION_MARK
#define QOR_PP_H_COMPONENTS_JSON_PARSER_QUOTATION_MARK

#include "../tokens.h"

namespace qor { namespace components { namespace parser { namespace json {

    class quotation_mark : public Specific
    {
        public: quotation_mark(Parser* parser) : Specific(parser, (byte)0x22, static_cast<uint64_t>(jsonToken::quotation_mark)){ } 
        virtual ~quotation_mark() = default;
        virtual void Prepare()
        {
            //log::debug("Looking for a quotation mark...");
        }

        void Emit()
        {
            //log::debug("Emitting a quotation mark: ");
        }

        void Fail()
        {
            //log::debug("...Didn't find a quotation mark.");            
        }

    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_QUOTATION_MARK