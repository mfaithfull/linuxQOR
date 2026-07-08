// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_DIGIT1_9
#define QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_DIGIT1_9

#include "src/components/data/parser/node.h"
#include "../tokens.h"
#include "../../model/digit1_9.h"

namespace qor { namespace components{ namespace parser{ namespace json {

    class Digit1_9Node : public NodeAdapter< model::json::Digit1_9 >
    {
    public:

        Digit1_9Node(char c = 0) : NodeAdapter< model::json::Digit1_9 >(static_cast<uint64_t>(jsonToken::digit1_9))
        {
            GetObject()->SetValue(c);
        }
        
        virtual ~Digit1_9Node() = default;
        
        virtual std::string ToString() const override 
        { 
            char c = GetObject()->GetValue();
            return std::string(&c,1); 
        };
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_NODES_DIGIT1_9