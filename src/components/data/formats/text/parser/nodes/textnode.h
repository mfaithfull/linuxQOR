// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_TEXT_PARSER_NODES_TEXT
#define QOR_PP_H_DATA_TEXT_PARSER_NODES_TEXT

#include <string>
#include "src/components/data/parser/node.h"
#include "../tokens.h"

namespace qor { namespace data{ namespace parser{ namespace text {

    class TextNode : public NodeAdapter< std::string >
    {
    public:
        TextNode() : NodeAdapter< std::string >(static_cast<uint64_t>(textToken::text)){};
        virtual ~TextNode() = default;
        virtual std::string ToString() const override { return m_t.IsNotNull() ? *m_t : ""; };
    };

}}}}//qor::data::parser::text

#endif//QOR_PP_H_DATA_TEXT_PARSER_NODES_TEXT
