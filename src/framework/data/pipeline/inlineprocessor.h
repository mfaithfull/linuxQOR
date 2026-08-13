// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_INLINEPROCESSOR
#define QOR_PP_H_PIPELINE_INLINEPROCESSOR

#include <cstring>
#include "podbuffer.h"

namespace qor{ namespace pipeline{

    template< class out_t, class in_t >
    class InlineProcessor : public Buffer
    {
    public:
    
        InlineProcessor(size_t inItemCount = 0, size_t outItemCount = inItemCount) : Buffer(sizeof(in_t), 0),
            m_sourceBuffer(inItemCount),
            m_sinkBuffer(outItemCount)
        { }

        InlineProcessor(const InlineProcessor& src)
        {
            *this = src;
        }

        virtual ~InlineProcessor()
        {
            m_sourceBuffer.SetCapacity((size_t)0);
            m_sinkBuffer.SetCapacity((size_t)0);
        }

        InlineProcessor& operator = (const InlineProcessor& src)
        {
            if(&src != this)
            {
                m_sourceBuffer = src.m_sourceBuffer;
                m_sinkBuffer = src.m_sinkBuffer;
            }
            return *this;
        }

        virtual size_t WriteCapacity() const
        {
            return m_sourceBuffer.WriteCapacity();
        }

        virtual size_t ReadCapacity() const
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
            SetInCapacity(itemCount);
            SetOutCapacity(itemCount);
        }

        virtual void SetInCapacity(size_t itemCount)
        {   
            m_sourceBuffer.SetCapacity(itemCount);            
        }

        virtual void SetOutCapacity(size_t itemCount)
        {   
            m_sinkBuffer.SetCapacity(itemCount);            
        }

        virtual void Reset(size_t inItemCount, size_t outItemCount = 1)
        {
            m_sourceBuffer.Reset(inItemCount);
            m_sinkBuffer.Reset(outItemCount);
        }

        size_t InCapacity() const
        {
            return m_sourceBuffer.Capacity();
        }

        size_t OutCapacity() const
        {
            return m_sinkBuffer.Capacity();
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

            Process(itemCount);

            return m_sourceBuffer.WriteCapacity();
        }

        size_t GetUnitSize() const
        {
            return GetInUnitSize();
        }

        size_t GetInUnitSize() const
        {
            return m_sourceBuffer.GetUnitSize();
        }

        size_t GetOutUnitSize() const
        {
            return m_sinkBuffer.GetUnitSize();
        }

    protected:

        virtual void Process(size_t& itemCount) = 0;

        PODBuffer<in_t> m_sourceBuffer;
        PODBuffer<out_t> m_sinkBuffer;        
    };

}}//qor::pipeline

#endif//QOR_PP_H_PIPELINE_INLINEPROCESSOR