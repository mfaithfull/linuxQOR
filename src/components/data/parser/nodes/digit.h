// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PARSER_NODES_DIGIT
#define QOR_PP_H_COMPONENTS_PARSER_NODES_DIGIT

#include "../node.h"
#include "../tokens.h"

namespace qor { namespace components{ namespace parser{

    class DigitNode : public Node
    {
    public:

        DigitNode(int digitVal) : Node(static_cast<uint64_t>(eToken::Digit)) , m_value(digitVal){ }
        
        virtual ~DigitNode() = default;

        unsigned int GetValue()
        {
            return m_value;
        }

    private:
    
        unsigned int m_value;
    };

}}}//qor::components::parser

#endif//QOR_PP_H_COMPONENTS_PARSER_NODES_DIGIT