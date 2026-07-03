// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_UTF8STRING
#define QOR_PP_H_TEXT_UTF8STRING

#include <stdexcept>
#include <string>
#include "src/qor/essentials/text/abstractstring.h"
#include "src/qor/essentials/text/buffers/mutablebuffer.h"
#include "src/qor/essentials/text/iterators/utf8iterator.h"
#include "src/qor/essentials/text/codepage/codepages/utf8.h"

namespace qor{

    //UTF-8 Strings have UTF-8 encoding and variable width (1-4 bytes per character)
    class UTF8String : public AbstractString< 
        UTF8String, 
        MutableBuffer<char8_t>,
        UTF8Iterator< MutableBuffer<char8_t> >::Iterator,
        UTF8ConstIterator< MutableBuffer<char8_t> >::Iterator,
        UTF8ReverseIterator< MutableBuffer<char8_t> >::Iterator,
        UTF8ConstReverseIterator< MutableBuffer<char8_t> >::Iterator >
    {
    public:

        typedef char8_t CharT;
        typedef AbstractString< 
        UTF8String, 
        MutableBuffer<CharT>,
        UTF8Iterator< MutableBuffer<CharT> >::Iterator,
        UTF8ConstIterator< MutableBuffer<CharT> >::Iterator,
        UTF8ReverseIterator< MutableBuffer<CharT> >::Iterator,
        UTF8ConstReverseIterator< MutableBuffer<CharT> >::Iterator > base;

        typedef MutableBuffer<CharT> BufferT;
        typedef UTF8CodePage defaultEncodingT;
        typedef typename BufferT::View viewT;
        typedef typename UTF8Iterator<BufferT>::Iterator iterator;
        typedef typename UTF8ConstIterator<BufferT>::Iterator const_iterator;
        typedef typename UTF8ReverseIterator<BufferT>::Iterator reverse_iterator;
        typedef typename UTF8ConstReverseIterator<BufferT>::Iterator const_reverse_iterator;

        UTF8String() noexcept : m_buffer(), m_cachedLength(0) { }

        template<size_t N>
        UTF8String(const char(&str)[N]) noexcept : m_buffer((const char8_t*)str, N - 1)
        {
            UpdateLength();
        }

        template<size_t N>
        UTF8String(const char8_t(&str)[N]) noexcept : m_buffer(str)
        {
            UpdateLength();
        }

        UTF8String(const BufferT& buffer) : m_buffer(buffer)
        {
            UpdateLength();
        }

        UTF8String(const UTF8String& src) : m_buffer(src.m_buffer)
        {
            m_cachedLength = src.Length();
        }

        UTF8String(UTF8String&& src) noexcept : m_buffer(std::move(src.m_buffer))
        {
            m_cachedLength = src.Length();
        }

        //NOTE: These assume the sources are valid pointers into valid UTF-8 encoded text
        //If not you will get junk and may triggger a buffer overrun error or snakes or
        //zombies or anything really.

        UTF8String(const char* pBuffer, size_t stCount ) : m_buffer( (const char8_t*)pBuffer, stCount )
        {
            UpdateLength();            
        }

        UTF8String(const char8_t* pBuffer, size_t stCount ) : m_buffer( pBuffer, stCount )
        {
            UpdateLength();            
        }

        UTF8String(iterator from, iterator to) : m_buffer(from, to - from)
        {
            UpdateLength();
        }
        
        virtual ~UTF8String() = default;

        UTF8String& operator = (const UTF8String& src)
        {
            if (this != &src)
            {
                m_buffer = src.m_buffer;
                m_cachedLength = src.Length();
            }
            return *this;
        }

        size_t Length() const override
        {
            return m_cachedLength;
        }

        bool IsEmpty() const override
        {
            return m_buffer.IsEmpty();
        }

        void Reset(void) override
        {
            m_buffer.Reset();
            m_cachedLength = 0;
        }

        virtual viewT view() const override
        {
            return m_buffer.view();
        }

        char32_t operator[](size_t index) const
        {
            iterator it = begin();
            while(it != end() && index-- > 0)
            {
                it++;
            }
            if(it != end())
            {
                AbstractCharacterCodec< CharT >* Codec = GetCodec();
                const CharT* input = it;
                size_t charCount = 1;
                CodePoint cp = Codec->Decode(input, charCount);
                return cp.UChar();
            }
            else
            {
                throw std::out_of_range("Index out of range accessing UTF8String At {index}");
            }
        }

        CharT At(size_t index) const override
        {
            return m_buffer.At(index);
        }

        inline UTF8String Clone() const override
        {
            return UTF8String(*this);
        }

        static inline UTF8String EmptyString()
        {
            return UTF8String();
        }

        inline const std::basic_string<char> ToStdString() const override
        {
            return m_buffer.ToStdString<char>();
        }

        inline size_t size() const override
        {
            return Length();
        }

        inline iterator begin() const override
        {
            iterator it(m_buffer);
            return it.begin();
        }

        inline const_iterator cbegin() const override
        {
            const_iterator it(m_buffer);
            return it.begin();
        }

        inline iterator end() const override
        {
            iterator it(m_buffer);
            return it.end();
        }

        inline const_iterator cend() const override
        {
            const_iterator it(m_buffer);
            return it.end();
        }

        inline reverse_iterator rbegin() const override
        {
            reverse_iterator it(m_buffer);
            return it.begin();
        }

        inline const_reverse_iterator crbegin() const override
        {
            const_reverse_iterator it(m_buffer);
            return it.begin();
        }

        inline reverse_iterator rend() const override
        {
            reverse_iterator it(m_buffer);
            return it.end();
        }

        inline const_reverse_iterator crend() const override
        {
            const_reverse_iterator it(m_buffer);
            return it.end();
        }

        virtual inline Mib GetEncoding() const override
        {
            return defaultEncodingT::GetMib();
        }

    protected:
        
        virtual void UpdateLength() const override
        {            
            //Recalculate character count after buffer change
            m_cachedLength = 0;
            auto it = cbegin();
            while(it++ != cend())
            {
                m_cachedLength++;
            }
        }

        virtual AbstractCharacterCodec< CharT >* GetCodecCache() const override
        { 
            return m_cachedCodec; 
        }

        virtual void SetCodecCache(AbstractCharacterCodec< CharT >* codec) const override
        {
            m_cachedCodec = codec;
        }

        virtual BufferT CloneBuffer() const override
        {
            return BufferT(m_buffer);
        }

        virtual BufferT* GetModifiableBufferObject()
        {
            return &m_buffer;
        }

        BufferT m_buffer;
        mutable size_t m_cachedLength{0};
        mutable AbstractCharacterCodec< CharT >* m_cachedCodec{nullptr};
    };

}//qor

#endif//QOR_PP_H_TEXT_UTF8STRING
