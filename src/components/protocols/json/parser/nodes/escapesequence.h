// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_ESCAPESEQUENCE
#define QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_ESCAPESEQUENCE

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/string.h"

namespace qor { namespace components{ namespace parser{ namespace json {

    class EscapeSequenceNode : public NodeAdapter< model::json::String >
    {
    public:

        EscapeSequenceNode() : NodeAdapter< model::json::String >(static_cast<uint64_t>(jsonToken::escape_sequence))
        {
            GetObject()->SetValue("");
        }

        EscapeSequenceNode(char c) : NodeAdapter< model::json::String >(static_cast<uint64_t>(jsonToken::escape_sequence))
        {
            GetObject()->SetValue(std::string(&c,1));
        }
        
        virtual ~EscapeSequenceNode() = default;
        virtual std::string ToString() const override 
        { 
            return GetObject()->GetValue();
        };
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_ESCAPESEQUENCE