// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_INLINEFILTER
#define QOR_PP_H_PIPELINE_INLINEFILTER

#include <cstring>
#include "podbuffer.h"

namespace qor{ namespace pipeline{

    //A pair of buffers that behave as one buffer and copy data between write source and sink as necessary
    //Override the plain copying filter function to do transformation instead of just copying.

    template< class pod_t >
    class InlineFilter : public Buffer
    {
    public:

        InlineFilter(size_t itemCount = 0) : Buffer(sizeof(pod_t), itemCount),
            m_sourceBuffer(itemCount),
            m_sinkBuffer(itemCount)
        { }

        InlineFilter(const InlineFilter& src)
        {
            *this = src;
        }

        virtual ~InlineFilter()
        {
            m_sourceBuffer.SetCapacity((size_t)0);
            m_sinkBuffer.SetCapacity((size_t)0);
        }

        InlineFilter& operator = (const InlineFilter& src)
        {
            if(&src != this)
            {
                m_sourceBuffer = src.m_sourceBuffer;
                m_sinkBuffer = src.m_sinkBuffer;
            }
            return *this;
        }

        virtual size_t WriteCapacity()
        {
            return m_sourceBuffer.WriteCapacity();
        }

        virtual size_t ReadCapacity()
        {
            return m_sinkBuffer.ReadCapacity();
        }

        virtual byte* WriteRequest(size_t& itemCount)
        {
            return m_sourceBuffer.WriteRequest(itemCount);
        }

        virtual byte* ReadRequest(size_t& itemCount)
        {
            return m_sinkBuffer.ReadRequest(itemCount);
        }

        virtual void SetCapacity(size_t itemCount)
        {   
            m_sourceBuffer.SetCapacity(itemCount);
            m_sinkBuffer.SetCapacity(itemCount);
        }

        virtual size_t ReadAcknowledge(size_t& itemCount)
        {
            return m_sinkBuffer.ReadAcknowledge(itemCount);
        }

        virtual size_t ReadReject(size_t& itemCount)
        {
            return m_sinkBuffer.ReadReject(itemCount);
        }

        virtual size_t WriteAcknowledge(size_t& itemCount)
        {            
            m_sourceBuffer.WriteAcknowledge(itemCount);            
            //Process data to sink buffer
            pod_t* data = m_sourceBuffer.ReadRequest(itemCount);
            size_t writeCount = 0;
            if(DoFilter(data, itemCount, writeCount))
            {
                m_sourceBuffer.ReadAcknowledge(itemCount);
                itemCount = writeCount;
            }

            return m_sourceBuffer.WriteCapacity();
        }

    protected:

        virtual void Filter(pod_t* space, pod_t* data, size_t& itemCount, size_t& writeCount)
        {
            ::memcpy((void*)space, (const void*)data, std::min(itemCount, writeCount) * sizeof(pod_t));
        }

    private:

        virtual bool DoFilter(pod_t* data, size_t& itemCount, size_t& writeCount)
        {
            if(itemCount > 0)
            {
                writeCount = m_sinkBuffer.WriteCapacity();
                pod_t* space = m_sinkBuffer.WriteRequest(writeCount);
                if(writeCount > 0)
                {
                    Filter(space, data, itemCount, writeCount);
                    m_sinkBuffer.WriteAcknowledge(writeCount);
                    if(writeCount > 0)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        PODBuffer<pod_t> m_sourceBuffer;
        PODBuffer<pod_t> m_sinkBuffer;        
    };

}}//qor::pipeline

#endif//QOR_PP_H_PIPELINE_INLINEFILTER