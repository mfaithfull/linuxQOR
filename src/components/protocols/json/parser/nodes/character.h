// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_CHARACTER
#define QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_CHARACTER

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/character.h"

namespace qor { namespace components{ namespace parser{ namespace json {

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
            char c = static_cast<char>(GetObject()->GetValue());
            return std::string(&c,1); 
        };
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_CHARACTER