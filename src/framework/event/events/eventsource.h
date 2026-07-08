// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EVENTS_SOURCE
#define QOR_PP_H_EVENTS_SOURCE

#include "eventqueue.h"
#include "src/framework/event/delegate/bindmember.h"

namespace qor{ namespace events{

    template<class event_type>
    class Source
    {
    public:

        Source(uint32_t categoryId, size_t stockSize, EventQueue* queue) : m_category(categoryId), m_queue(queue), m_stock(stockSize){}
        ~Source() noexcept = default;

        event_type* GetMessage(uint32_t eventId)
        {
            event_type* eventData = nullptr;
            while(eventData == nullptr)				//Get an event from stock
            {
                eventData = m_stock.WriteBegin();				
            }
			eventData->Reset();						//Reset it to defaults
            m_stock.WriteEnd();						//Allow the stock item to be released later
            struct event* eventMessage = nullptr;
            while(eventMessage == nullptr)
            {
                eventMessage = m_queue->WriteBegin();//Grab a queue slot;
            }
            eventMessage->category = m_category;	//Write the event cat, id and link the dat
            eventMessage->evt = eventId;
            eventMessage->data = reinterpret_cast<void*>(eventData);//Link the slot to the stock;
            eventMessage->release = bindMemberFunction(&Source<event_type>::Release, this);//make the slot remember how to free the stock      
            return eventData;						//Return the event data so the client can set it as required
        }

        void Send()
        {
            m_queue->WriteEnd();					//This makes the queue slot available to be read
        }

        void Release()
        {
            event_type* eventData = m_stock.ReadBegin();
            eventData->Reset();
            m_stock.ReadEnd();
        }

    protected:

        uint32_t m_category;
        EventQueue* m_queue;
        PODQueue<event_type, size_t> m_stock;

    };

}}//qor::events

#endif//QOR_PP_H_EVENTS_SOURCE
