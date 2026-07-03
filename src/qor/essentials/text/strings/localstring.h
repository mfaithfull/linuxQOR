// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_LOCALSTRING
#define QOR_PP_H_TEXT_LOCALSTRING

#ifndef QOR_PP_H_TEXT_STRINGS_STRINGS
#   error "don't include localstring.h directly, include src/qor/text/strings/strings.h"
#endif

#include <stdexcept>
#include <string>
#include "src/qor/essentials/text/abstractstring.h"
#include "src/qor/essentials/text/buffers/encodedbuffer.h"
#include "src/qor/essentials/iterators/iterators.h"
#include "src/qor/essentials/text/codepage/codepageregistry.h"

namespace qor{

    //LocalStrings have fixed character size but variable encoding
    //Set the Encoding to the CodePage of the locale in use
    //but remember to use an appropriate character width for your 
    //locale. A European LocalString might be LocalString< char8_t >
    //whereas a CJK LocalString might be LocalString< char16_t >
    template< typename C, Mib defaultMib = Mib::C >
    class LocalString : public AbstractString< 
        LocalString< C, defaultMib >, 
        EncodedBuffer< C, defaultMib >,
        rawiterator< C >,
        rawiterator< const C >,
        rawreverseiterator< C >,
        rawreverseiterator< const C > >
    {
    public:
        
        typedef AbstractString< 
            LocalString< C, defaultMib >, 
            EncodedBuffer< C, defaultMib >,
            rawiterator< C >,
            rawiterator< const C >,
            rawreverseiterator< C >,
            rawreverseiterator< const C > > base;        

        typedef EncodedBuffer< C, defaultMib > BufferT;
        typedef rawiterator< C > iterator;
        typedef rawiterator< const C > const_iterator;
        typedef rawreverseiterator< C > reverse_iterator;
        typedef rawreverseiterator< const C > const_reverse_iterator;

        inline LocalString() noexcept : m_buffer((size_t)1){ }

        template<size_t N>
        inline LocalString(const C(&str)[N], Mib charSet = defaultMib) : m_buffer(str)
        {
            SetEncoding(charSet);
        }

        LocalString(const BufferT& buffer) : m_buffer(buffer){ }

        LocalString(const LocalString& src) : m_buffer(src.m_buffer){ }

        LocalString(LocalString&& src) noexcept : m_buffer(std::move(src.m_buffer)){ }

        LocalString( const C* pBuffer, size_t stCount, Mib charSet = defaultMib) : m_buffer( pBuffer, stCount )
        { 
            SetEncoding(charSet);
        }

        LocalString(iterator from, iterator to, Mib charSet = defaultMib) : m_buffer(from, to - from)
        { 
            SetEncoding(charSet);
        }

        virtual ~LocalString() = default;

        LocalString& operator = (const LocalString& src)
        {
            if (this != &src)
            {
                m_buffer = src.m_buffer;                
            }
            return *this;
        }

        inline size_t Length() const override
        {
            return m_buffer.Length();
        }

        inline bool IsEmpty() const override
        {
            return m_buffer.IsEmpty();
        }

        inline void Reset(void) override
        {
            m_buffer.Reset();
        }

        inline typename BufferT::View view() const override
        {
            return m_buffer.view();
        }

        inline C operator[](size_t index) const
        {
            return m_buffer[index];
        }

        inline C At(size_t index) const override
        {
            return m_buffer.At(index);
        }

        inline LocalString< C, defaultMib > Clone() const override
        {
            return LocalString< C, defaultMib >(m_buffer);
        }

        static inline LocalString< C, defaultMib > EmptyString()
        {
            return LocalString< C, defaultMib >();
        }

        inline const std::basic_string< typename std_string_char_for< C >::type > ToStdString() const override
        {
            return m_buffer.template ToStdString< typename std_string_char_for< C >::type >();
        }

        operator std::basic_string<C>() const
        {
            return ToStdString();
        }

        inline size_t size() const override
        {
            return m_buffer.Length();
        }

        inline iterator begin() const override
        {
            return iterator(m_buffer.begin());
        }

        inline const_iterator cbegin() const override
        {
            return const_iterator(m_buffer.cbegin());
        }

        inline iterator end() const override
        {
            return iterator(m_buffer.end());
        }

        inline const_iterator cend() const override
        {
            return const_iterator(m_buffer.cend());
        }

        inline reverse_iterator rbegin() const override
        {
            return reverse_iterator(m_buffer.rbegin());
        }

        inline const_reverse_iterator crbegin() const override
        {
            return const_reverse_iterator(m_buffer.crbegin());
        }

        inline reverse_iterator rend() const override
        {
            return reverse_iterator(m_buffer.rend());
        }

        inline const_reverse_iterator crend() const override
        {
            return const_reverse_iterator(m_buffer.crend());
        }

        virtual inline Mib GetEncoding() const override
        {
            return m_buffer.GetEncoding();
        }

        inline void SetEncoding(Mib charSet)
        {
            m_buffer.SetEncoding(charSet);
        }

    protected:

        virtual inline BufferT CloneBuffer() const
        {
            return BufferT(m_buffer);
        }

        virtual inline BufferT* GetModifiableBufferObject()
        {
            return &m_buffer;
        }

        BufferT m_buffer;        
    };
}//qor

#endif//QOR_PP_H_TEXT_LOCALSTRING
