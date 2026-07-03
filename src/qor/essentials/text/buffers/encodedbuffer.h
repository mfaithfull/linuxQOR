// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_ENCODEDBUFFER
#define QOR_PP_H_TEXT_ENCODEDBUFFER

#include "src/qor/essentials/text/buffers/mutablebuffer.h"
#include "src/qor/essentials/text/codepage/codepages.h"
#include "../abstractstring.h"

//A mutable text buffer with intrinsic encoding
//Use these for strings encoded by a specific code page - e.g. ISOLatin3

namespace qor{

    struct sEncodedBufferHeader
    {
        Mib Encoding;                               //Encoding of Text
        //This structure must end with a copy of sBufferHeader
        size_t Alloc;								//Byte length of allocation for string, header, footer and all
        size_t RefCount;							//Reference count for CoW
        size_t Len;									//Unit count in use
    };

    struct EncodedBufferLayout
    {
    public:
        static const unsigned short HeaderSize{sizeof(sEncodedBufferHeader)};
        static const unsigned short FooterSize{sizeof(sBufferFooter)};
    };

    template< typename T, Mib defaultMib = Mib::C>
    class EncodedBuffer : public MutableBuffer<T, EncodedBufferLayout>
    {
    public:
        
        typedef MutableBuffer<T, EncodedBufferLayout> base;

        EncodedBuffer() : base()
        {
            SetEncoding(defaultMib);
        }

        EncodedBuffer(const EncodedBuffer& src) : base(src)
        {            
            SetEncoding(src.GetEncoding());
        }

        EncodedBuffer(const ConstBuffer<T>& src) : base(src)
        {
            SetEncoding(defaultMib);
        }

        EncodedBuffer(const ConstBuffer<T>&& src) : base(src)
        {
            SetEncoding(defaultMib);
        }

        template<size_t N>
        EncodedBuffer(T(&str)[N]) : base(str)
        {
            SetEncoding(defaultMib);
        }

        EncodedBuffer( const T* pBuffer, size_t stCount ) : base(pBuffer, stCount)
        {
            SetEncoding(defaultMib);
        }

        EncodedBuffer(EncodedBuffer&& src) noexcept : base(std::move(src))
        {
            SetEncoding(src.GetEncoding());
        }

        EncodedBuffer(size_t initialUnitCapacity) : base(initialUnitCapacity)
        {
            SetEncoding(defaultMib);
        }

        EncodedBuffer& operator = ( EncodedBuffer&& src ) noexcept 
        {
            base::operator = (src);
            SetEncoding(src.GetEncoding());
            return *this;
        }

        EncodedBuffer& operator = ( const EncodedBuffer& src )
        {
            base::operator = (src);
            SetEncoding(src.GetEncoding());
            return *this;
        }

        virtual ~EncodedBuffer() = default;

        virtual void HeaderOnCopy(byte* pOld, byte* pNew) override
        {
            sEncodedBufferHeader* old = reinterpret_cast<sEncodedBufferHeader*>(pOld);
            sEncodedBufferHeader* _new = reinterpret_cast<sEncodedBufferHeader*>(pNew);
            if(_new)
            {
                _new->Encoding = old ? old->Encoding : defaultMib;
            }
        }

        void SetEncoding(Mib charSet)
        {
            sEncodedBufferHeader* header = reinterpret_cast<sEncodedBufferHeader*>(base::GetHeader());
            if(header)
            {
                header->Encoding = charSet;
            }
        }

        Mib GetEncoding() const
        {
            sEncodedBufferHeader* header = reinterpret_cast<sEncodedBufferHeader*>(base::GetHeader());
            if(header)
            {
                return header->Encoding;
            }
            return Mib::C;
        }

    };
}//qor

#endif//QOR_PP_H_TEXT_ENCODEDBUFFER
