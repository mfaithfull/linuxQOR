
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cstring>
#include "tokenbuffer.h"

namespace qor { namespace ui { namespace sercon {

    TokenBuffer::TokenBuffer() : m_tokens(nullptr), m_width(80), m_height(25)
    {
        Reset(m_width, m_height);
    }
        
    TokenBuffer::~TokenBuffer()
    {
        m_width = 0;
        m_height = 0;
        delete m_tokens;
        m_tokens = nullptr;
    }

    void TokenBuffer::SetWidth(int width)
    {
        Reset(width, m_height);
    }

    void TokenBuffer::SetHeight(int height)
    {
        Reset(m_width, height);
    }

    void TokenBuffer::SetSize(int width, int height)
    {
        Reset(width, height);
    }

    void TokenBuffer::Reset(int width, int height)
    {
        m_width = width;
        m_height = height;
        delete m_tokens;
        m_tokens = new int[m_width * m_height];
        Clear();
    }
        
    int TokenBuffer::GetWidth()
    {
        return m_width;
    }

    int TokenBuffer::GetHeight()
    {
        return m_height;
    }

    int TokenBuffer::GetToken(int line, int column)
    {
        if(column >= 0 && column < m_width && line >=0 && line < m_height)
        {
            return m_tokens[(line * m_width) + column];
        }
        return 0;
    }

    void TokenBuffer::SetToken(int line, int column, int token)
    {
        if(column >= 0 && column < m_width && line >= 0 && line < m_height)
        {
            m_tokens[(line * m_width) + column] = token;
        }
    }

    void TokenBuffer::Clear()
    {
        memset(m_tokens, Tokens::BLANK, m_width * m_height * sizeof(int));
    }

    void TokenBuffer::CopyFrom(TokenBuffer* src, Position position, Size size)
    {
        for(int column = position.m_column; column < position.m_column + size.m_width; ++column)
        {
            for(int row = position.m_row; row < position.m_row + size.m_height; ++row)
            {
                SetToken(row, column, src->GetToken(row - position.m_row, column - position.m_column));
            }
        }
    }

}}}//qor::ui::sercon