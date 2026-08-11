// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_TEXT_PARSER_NODES_UTF16CHAR
#define QOR_PP_H_DATA_TEXT_PARSER_NODES_UTF16CHAR

#include <string>
#include "src/components/data/parser/node.h"
#include "../tokens.h"

namespace qor { namespace data{ namespace parser{ namespace text {

    class UTF16CharNode : public NodeAdapter< unsigned short >
    {
    public:
        UTF16CharNode() : NodeAdapter< unsigned short >(static_cast<uint64_t>(textToken::utf16char)){};

        virtual ~UTF16CharNode() = default;

        byte GetLowByte()
        {
            return (byte)(*m_t & 0x00FF);
        }

        byte GetHighByte()
        {
            return (byte)((*m_t << 8) & 0xFF00);
        }

        void SetLowByte(byte b)
        {
            m_t()() = (*m_t & 0xFF00) | b;
        }

        void SetHighByte(byte b)
        {
            unsigned short s = ( b << 8 ) & 0xFF00;
            m_t()() = (*m_t & 0x00FF) | s;
        }
    };

}}}}//qor::data::parser::text

#endif//QOR_PP_H_DATA_TEXT_PARSER_NODES_UTF16CHAR
