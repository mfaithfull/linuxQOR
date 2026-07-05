// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_CODEPAGE_CODEPOINT
#define QOR_PP_H_TEXT_CODEPAGE_CODEPOINT

#include <stdint.h>

//NOTE: This shold be in ../unicode

namespace qor{

	class CodePoint
	{
	public:
    
        CodePoint(uint32_t codePoint) noexcept : m_codePoint(codePoint){ }

        CodePoint(const CodePoint& src) noexcept : m_codePoint(src.m_codePoint){ }

        CodePoint(const CodePoint&& src) noexcept : m_codePoint(src.m_codePoint){ }

        CodePoint& operator = (const CodePoint& src)
        {
            m_codePoint = src.m_codePoint;
            return *this;
        }        

        CodePoint& operator = (const CodePoint&& src) noexcept
        {
            m_codePoint = src.m_codePoint;
            return *this;
        }
        
        bool operator == (const CodePoint& cmp) const
        {
            return m_codePoint == cmp.m_codePoint;
        }

        bool operator != (const CodePoint& cmp) const
        {
            return !operator==(cmp);
        }

        bool operator < (const CodePoint& cmp) const
        {
            return m_codePoint < cmp.m_codePoint;
        }

        bool operator <= (const CodePoint& cmp) const
        {
            return m_codePoint <= cmp.m_codePoint;
        }

        bool operator > (const CodePoint& cmp) const
        {
            return m_codePoint > cmp.m_codePoint;
        }

        bool operator >= (const CodePoint& cmp) const
        {
            return m_codePoint >= cmp.m_codePoint;
        }

        ~CodePoint() = default;

        //const char* Name(void);

        uint32_t inline Value(void) const
        {
            return m_codePoint;
        }

        char32_t inline UChar() const
        {
            return static_cast<char32_t>(m_codePoint);
        }

	private:
        CodePoint() = delete;
        uint32_t m_codePoint;
    };

}//qor

#endif//QOR_PP_H_TEXT_CODEPAGE_CODEPOINT