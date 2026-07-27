// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_NODES_HEXDIGIT
#define QOR_PP_H_DATA_PARSER_NODES_HEXDIGIT

#include "../node.h"
#include "../tokens.h"

namespace qor { namespace data{ namespace parser{

    class HexDigitNode : public Node
    {
    public:

        HexDigitNode(int digitVal) : Node(static_cast<uint64_t>(eToken::HexDigit)) , m_value(digitVal){ }
        
        virtual ~HexDigitNode() = default;

        unsigned int GetValue()
        {
            return m_value;
        }

    private:
    
        unsigned int m_value;
    };

}}}//qor::data::parser

#endif//QOR_PP_H_DATA_PARSER_NODES_HEXDIGIT
