// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_NODES_CHARACTER
#define QOR_PP_H_DATA_JSON_PARSER_NODES_CHARACTER

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/character.h"
#include "src/qor/essentials/text/strings/strings.h"
#include "src/qor/essentials/text/codepage/codepages/utf8.h"

namespace qor { namespace data{ namespace parser{ namespace json {

    class CharacterNode : public NodeAdapter< model::json::Character >
    {
    public:

        CharacterNode(unsigned int c = 'c') : NodeAdapter< model::json::Character >(static_cast<uint64_t>(jsonToken::character))
        {
            GetObject()->SetValue(c);
        }
        
        virtual ~CharacterNode() = default;
        virtual std::string ToString() const override 
        { 
            char32_t c = static_cast<char32_t>(GetObject()->GetValue());
            UTF8CodePage codePage;
            char8_t space[6];
            char8_t* p = &space[0];
            size_t available = 1;
            if(codePage.Encode(c, p, available))
            {
                return std::string((const char*)space, (p - &space[0]));
            }

            return ""; 
        };
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_NODES_CHARACTER