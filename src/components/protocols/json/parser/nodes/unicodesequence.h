// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_UNICODESEQUENCE
#define QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_UNICODESEQUENCE

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/codepoint.h"

namespace qor { namespace components{ namespace parser{ namespace json {

    class UnicodeSequenceNode : public NodeAdapter< model::json::CodePoint >
    {
    public:

        UnicodeSequenceNode(char c = 0) : NodeAdapter< model::json::CodePoint >(static_cast<uint64_t>(jsonToken::unicode_sequence))
        {
            GetObject()->SetValue(c);
        }
        
        virtual ~UnicodeSequenceNode() = default;
        virtual std::string ToString() const override 
        { 
            return "";//TODO:
        };
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_UNICODESEQUENCE