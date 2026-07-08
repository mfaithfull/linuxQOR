// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EVENTS_EVENT
#define QOR_PP_H_EVENTS_EVENT

#include <cstdint>
#include <functional>

namespace qor{ namespace events{
    
    struct event
    {
        uint32_t category;
        uint32_t evt;
        void* data;
        std::function<void(void)> release;        
    };

}}//qor::events

#endif//QOR_PP_H_EVENTS_EVENT
