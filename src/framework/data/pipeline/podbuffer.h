// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_PODBUFFER
#define QOR_PP_H_PIPELINE_PODBUFFER

#include "buffer.h"

namespace qor{ namespace pipeline{

    template< class pod_t >
    class PODBuffer : public Buffer
    {
    public:

        PODBuffer(size_t itemCount = 0) : Buffer(sizeof(pod_t), itemCount), m_pAllocation(nullptr)
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

        virtual size_t WriteCapacity()
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

        virtual size_t ReadCapacity()
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
            pod_t* pResult = 0;
            if (itemCount == 0 || itemCount > WriteCapacity())
            {
                itemCount = WriteCapacity();
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
            if (itemCount > ReadCapacity())
            {
                itemCount = static_cast<size_t>(ReadCapacity());
            }
            m_readEnd += itemCount;
            return reinterpret_cast<byte*>(pResult);
        }

        virtual void SetCapacity(size_t itemCount)
        {
            if(m_allocationCount == itemCount)
            {
                return;
            }
            //The one and only bit of memory management;
            delete[] m_pAllocation;
            m_pAllocation = (itemCount > 0) ? new pod_t[itemCount] : nullptr;
            memset(m_pAllocation, 0, sizeof(pod_t) * itemCount);
            Buffer::SetCapacity(itemCount);
        }

    protected:

        pod_t* EndOfBuffer(void)
        {
            pod_t* pResult = nullptr;
            if (m_allocationCount > 0)
            {
                pResult = m_pAllocation + m_allocationCount;
            }
            return pResult;
        }

        pod_t* AddressOf(size_t index)
        {
            pod_t* pResult = nullptr;
            if (m_allocationCount > 0)
            {
                pResult = m_pAllocation + (index % m_allocationCount);
            }
            return pResult;
        }

        pod_t* m_pAllocation;
    };

    typedef PODBuffer< byte > ByteBuffer;

}}//qor::pipeline

#endif//QOR_PP_H_PIPELINE_PODBUFFER
