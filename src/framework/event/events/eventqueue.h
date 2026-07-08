// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EVENTS_QUEUE
#define QOR_PP_H_EVENTS_QUEUE

#include "src/platform/compiler/compiler.h"

#include <atomic>

#include "event.h"

namespace qor{ namespace events{
    
    template<typename size_type = size_t>
    class AbstractQueue
    {
    public:

        AbstractQueue(size_type unitSize = 1, size_type itemCount = 0) : m_unitSize(unitSize)
        {
            SetCapacity(itemCount);
        }

        AbstractQueue(const AbstractQueue& src) = delete;
        AbstractQueue& operator = (const AbstractQueue& src) = delete;

        virtual ~AbstractQueue()
        {
            SetCapacity((size_type)0);
        }
        
        virtual size_type WriteCapacity() const
        {
            size_t result = m_allocationCount - static_cast<size_t>((m_write + m_beingRead) - m_read);
            return result;
        }

        virtual size_type ReadCapacity() const
        {
            size_t result = static_cast<size_t>(m_write - (m_read + m_beingWritten));
            return result;
        }

        virtual void SetCapacity(size_type itemCount)
        {
            m_allocationCount = itemCount;
            m_read = 0;            
            m_write = 0;    
            m_beingWritten = 0;
            m_beingRead = 0;
        }

        virtual void Reset(size_type itemCount = 0)
        {
            m_read = 0;            
            m_write = 0;
            if(itemCount != 0 && itemCount != m_allocationCount)//Only reallocate if the size actually changes
            {   
                SetCapacity(itemCount);
            }
        }

        size_type Capacity() const
        {
            return m_allocationCount;
        }

        size_type GetUnitSize() const
        {
            return m_unitSize;
        }

    protected:

        void SetUnitSize(size_type unitSize)
        {
            if(m_allocationCount > 0 )
            {
                SetCapacity((size_t)0);
            }
            m_unitSize = unitSize;
        }        

        size_type m_allocationCount;   //Tracks number of items of m_unitSize we have space for
        size_type m_unitSize;
        std::atomic<size_type> m_read;
        std::atomic<size_type> m_write;
        std::atomic<size_type> m_beingWritten;
        std::atomic<size_type> m_beingRead;

    };

    template<class event_t, typename size_type = size_t>
    class PODQueue : public AbstractQueue<size_type>
    {
    public:
        PODQueue(size_type itemCount = 0) : AbstractQueue<size_type>(sizeof(event_t), itemCount), m_pAllocation(nullptr)
        {
            SetCapacity(itemCount);
        }

        PODQueue(const PODQueue& src) = delete;

        virtual ~PODQueue()
        {
            SetCapacity((size_type)0);
        }

        PODQueue& operator = (const PODQueue& src) = delete;


        virtual size_type WriteCapacity()
        {
            //The count available to write is the total count - the reserved count
            size_type result = this->m_allocationCount - static_cast<size_type>((this->m_write + this->m_beingRead) - this->m_read);
            return result;
        }

        virtual size_type ReadCapacity()
        {
            //Read capacity is what has been written but not read yet
            size_type result = static_cast<size_type>(this->m_write - (this->m_read + this->m_beingWritten));
            if(result > this->m_allocationCount)
            {
                result = 0;
            }
            return result;
        }

        virtual event_t* WriteBegin()
        {            
            if (WriteCapacity() == 0 || this->m_beingWritten > 0)
            {
                return nullptr;
            }

            this->m_beingWritten++;
            event_t* result = AddressOf(this->m_write++);
            return result;
        }

        virtual void WriteEnd()
        {
            this->m_beingWritten--;
        }

        virtual event_t* Peek()
        {            
            if(ReadCapacity() == 0)
            {
                //TODO: warn empty
                return nullptr;
            }
            return AddressOf(this->m_read);
        }

        virtual event_t* ReadBegin()
        {            
            if(ReadCapacity() == 0 || this->m_beingRead > 0)
            {                
                return nullptr;
            }
            this->m_beingRead++;
            return AddressOf(this->m_read++);
        }

        virtual void ReadEnd()
        {
            this->m_beingRead--;
        }

        virtual void SetCapacity(size_type itemCount)
        {
            //The one and only bit of memory management;
            if(m_pAllocation != nullptr)
            {
                delete[] m_pAllocation;
            }
            m_pAllocation = (itemCount > 0) ? new event_t[itemCount] : nullptr;
            AbstractQueue<size_type>::SetCapacity(itemCount);
        }

    protected:
        
        event_t* AddressOf(size_type index)
        {
            event_t* pResult = nullptr;
            if (this->m_allocationCount > 0)
            {
                pResult = m_pAllocation + (index % this->m_allocationCount);
            }
            return pResult;
        }

        event_t* m_pAllocation;        

    };

    typedef PODQueue<struct event, size_t> EventQueue;
    
}}//qor::events

#endif//QOR_PP_H_EVENTS_QUEUE
