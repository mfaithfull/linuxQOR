// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EVENTS_SINK
#define QOR_PP_H_EVENTS_SINK

#include "eventqueue.h"

namespace qor{ namespace events{

    class Sink final
    {
    public:

        Sink(EventQueue* queue) : m_queue(queue) { }

        ~Sink() noexcept = default;

        bool Peek()
        {
            struct event* evt = m_queue->Peek();
            return evt != nullptr ? true : false;
        }

        struct event GetMessage()
        {
            struct event e;
            struct event* evt = nullptr;
            while(evt == nullptr)
            {
                evt = m_queue->ReadBegin();
            }
            e.category = evt->category;
            e.evt = evt->evt;
            e.data = evt->data;
            e.release = evt->release;            
            m_queue->ReadEnd();
            return e;
        }

    protected:

        EventQueue* m_queue;        

    };

}}//qor::events

#endif//QOR_PP_H_EVENTS_SINK
