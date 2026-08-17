// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_BUFFER_CONTEXT
#define QOR_PP_H_PIPELINE_BUFFER_CONTEXT

#include "buffer.h"
#include "../detail/abstractdatacontext.h"

namespace qor{ namespace pipeline{

    //Adapts a pipeline buffer
    template< typename pod_t >
    class BufferContext : public data::AbstractDataContext
    {
    public:
        
        BufferContext(const Buffer* buffer) : m_buffer(const_cast<Buffer*>(buffer)){ }

        bool GetItem(pod_t*& data)
        {            
            if(HasData())
            {
                size_t itemCount = 1;
                data = reinterpret_cast<pod_t*>(m_buffer->ReadRequest(itemCount));
                return itemCount == 1;
            }
            else
            {
                data = nullptr;
                return false;
            }            
        }

        inline bool RejectItem()
        {
            size_t itemCount = 1;
            m_buffer->ReadReject(itemCount);
            return itemCount == 1;
        }

        inline bool ConsumeItem()
        {
            size_t itemCount = 1;
            m_buffer->ReadAcknowledge(itemCount);
            m_position += 1;
            return itemCount == 1;
        }

        inline size_t GetPosition()
        {
            return m_position;
        }

        inline bool HasData()
        {
            return m_buffer->HasData();
        }

        inline bool HasSpace()
        {
            return m_buffer->WriteCapacity() > 0;
        }

        inline void Reset()
        {
            m_buffer->Reset();
            m_position = 0;
        }
        
    private:
    
        Buffer* m_buffer;        
        size_t m_position{0};

    };

}}//qor::pipeline

#endif//QOR_PP_H_PIPELINE_BUFFER_CONTEXT
