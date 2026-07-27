// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_TEXT_PARSER_NODES_BOM
#define QOR_PP_H_DATA_TEXT_PARSER_NODES_BOM

#include <string>
#include "src/components/data/parser/node.h"
#include "../tokens.h"

namespace qor { namespace data{ namespace parser{ namespace text {

    class BOMNode : public Node
    {
    public:

        enum class Size
        {
            Sixteen,
            ThrirtyTwo
        };

        BOMNode() : Node(static_cast<uint64_t>(textToken::bom)){};
        virtual ~BOMNode() = default;     

        void SetBOMType(textToken type)
        {
            m_BOMType = type;
        }

        textToken GetBOMType()
        {
            return m_BOMType;
        }

        void SetIndicator(Size size)
        {
            m_size = size;
        }

        Size GetIndicator()
        {
            return m_size;
        }

    private:   
        textToken m_BOMType{textToken::utf8BOM};
        Size m_size{Size::Sixteen};
    };

}}}}//qor::data::parser::text

#endif//QOR_PP_H_DATA_TEXT_PARSER_NODES_BOM
