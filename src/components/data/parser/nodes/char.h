// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PARSER_NODES_CHAR
#define QOR_PP_H_COMPONENTS_PARSER_NODES_CHAR

#include "../node.h"
#include "../tokens.h"

namespace qor { namespace components{ namespace parser{

    //A byte char node
    class CharNode : public Node
    {
    public:

        CharNode(char charVal, uint64_t token) : Node(token) , m_value(charVal){ }

        virtual ~CharNode() = default;

        char GetValue()
        {
            return m_value;
        }

    private:
    
        char m_value;
    };

}}}//qor::components::parser

#endif//QOR_PP_H_COMPONENTS_PARSER_NODES_CHAR