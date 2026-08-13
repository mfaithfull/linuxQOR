// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_QUOTATION_MARK
#define QOR_PP_H_DATA_JSON_PARSER_QUOTATION_MARK

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace json {

    class quotation_mark : public Specific<uint32_t>
    {
        public: quotation_mark(Parser* parser) : Specific<uint32_t>(parser, 0x22, static_cast<uint64_t>(jsonToken::quotation_mark)){ } 
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

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_QUOTATION_MARK