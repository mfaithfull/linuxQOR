
// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "src/configuration/configuration.h"

#include "tokenbuffer.h"

namespace qor { namespace components {

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

}}//qor::components