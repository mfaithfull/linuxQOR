// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_PODBUFFER
#define QOR_PP_H_PIPELINE_PODBUFFER

#include<cstring>
#include "buffer.h"

namespace qor{ namespace pipeline{

    template< class pod_t >
    class PODBuffer : public Buffer
    {
    public:

        PODBuffer(size_t itemCount = 0) : Buffer(sizeof(pod_t), itemCount), m_pAllocation(nullptr), m_wrapMask(0)
        {
            SetCapacity(itemCount);
        }

        PODBuffer(const PODBuffer& src)
        {
            *this = src;
        }

        virtual ~PODBuffer()
        {
            SetCapacity((size_t)0);
        }

        PODBuffer& operator = (const PODBuffer& src)
        {
            if(&src != this)
            {
                Buffer::operator = (src);
                memcpy(m_pAllocation, src.m_pAllocation, sizeof(pod_t) * std::min(m_allocationCount, src.m_allocationCount));
            }
            return *this;
        }

        PODBuffer& operator << (const pod_t& item)
        {
            size_t count = 1;
            pod_t* pWrite = reinterpret_cast<pod_t*>(WriteRequest(count));
            if(pWrite && count == 1)
            {
                *pWrite = item;
                WriteAcknowledge(count);
            }
            return *this;
        }

        PODBuffer& operator >> (pod_t& item)
        {
            size_t count = 1;
            pod_t* pRead = reinterpret_cast<pod_t*>(ReadRequest(count));
            if( pRead && count == 1 )
            {
                item = *pRead;
                ReadAcknowledge(count);
            }
            return *this;
        }

        virtual size_t WriteCapacity() const
        {
            //The count available to write is the total count - the reserved count
            size_t result = m_allocationCount - static_cast<size_t>(m_writeEnd - m_readBegin);
            //A single write can't wrap so restrict it to the number of elements left till the wrap
            if(AddressOf(m_writeBegin) + result > EndOfBuffer())
            {
                result = EndOfBuffer() - AddressOf(m_writeBegin);
            }
            return result;
        }

        virtual size_t ReadCapacity() const
        {
            //Read capacity is what has been written but not read yet
            size_t result = static_cast<size_t>(m_writeBegin - m_readEnd);
            //Single read can't wrap so cut it off at the next wrap
            if(AddressOf(m_readEnd) + result > EndOfBuffer())
            {
                result = EndOfBuffer() - AddressOf(m_readEnd);
            }
            return result;
        }

        virtual byte* WriteRequest(size_t& itemCount)
        {
            pod_t* pResult = nullptr;
            size_t writeCapacity = WriteCapacity();
            if (itemCount == 0 || itemCount > writeCapacity)
            {
                itemCount = writeCapacity;
            }

            if (itemCount > 0)
            {
                pResult = AddressOf(m_writeBegin);
                m_writeEnd += itemCount;
            }
            return reinterpret_cast<byte*>(pResult);
        }

        virtual byte* ReadRequest(size_t& itemCount)
        {
            pod_t* pResult = AddressOf(m_readBegin);
            size_t readCapacity = ReadCapacity();
            if (itemCount > readCapacity)
            {
                itemCount = readCapacity;
            }
            m_readEnd += itemCount;
            return reinterpret_cast<byte*>(pResult);
        }

        virtual void SetCapacity(size_t itemCount)
        {
            if(m_allocationCount == itemCount && m_pAllocation != nullptr)
            {
                return;
            }
            //The one and only bit of memory management;
            delete[] m_pAllocation;
            m_pAllocation = (itemCount > 0) ? new pod_t[itemCount] : nullptr;
            memset(reinterpret_cast<void*>(m_pAllocation), 0, sizeof(pod_t) * itemCount);
            Buffer::SetCapacity(itemCount);
            //Let AddressOf mask instead of divide whenever the count is a power of two (0 means "no mask").
            m_wrapMask = (itemCount > 1 && (itemCount & (itemCount - 1)) == 0) ? (itemCount - 1) : 0;
        }

    protected:

        inline pod_t* EndOfBuffer(void) const
        {
            return (m_allocationCount > 0) ? m_pAllocation + m_allocationCount : nullptr;
        }

        inline pod_t* AddressOf(size_t index) const
        {
            if (m_allocationCount == 0)
            {
                return nullptr;
            }
            //The wrap is a modulo when the allocation count is arbitrary, but every pipeline buffer
            //is sized as a power of two, and a modulo on a non-constant divisor is a real 64-bit
            //division on every call. This is called several times per code point, so mask instead.
            return m_pAllocation + ((m_wrapMask != 0) ? (index & m_wrapMask) : (index % m_allocationCount));
        }

        pod_t* m_pAllocation;
        size_t m_wrapMask;
    };

    typedef PODBuffer< byte > ByteBuffer;

}}//qor::pipeline

#endif//QOR_PP_H_PIPELINE_PODBUFFER
