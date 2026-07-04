// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MEMORY_THREAD_SMALLOBJECTHEAP_PAGE
#define QOR_PP_H_MEMORY_THREAD_SMALLOBJECTHEAP_PAGE

#include "src/platform/compiler/compiler.h"

namespace qor{ namespace memory{

    class qor_pp_module_interface(QOR_THREADMEMORY) SmallObjectPage final
    {
    public:

        static constexpr size_t sc_WordBits = sizeof(size_t) * 8;

        SmallObjectPage(size_t unitSize, size_t mapWordCount = 1);        
        ~SmallObjectPage();

        byte* Allocate();
        bool IsEmpty() const;
        bool Free(byte* element);
        bool IsFull() const;

        SmallObjectPage* m_prev;

    private:
    
        inline byte* Memory() const
        {
            return m_memory;
        }    

        void inline Use(size_t wordIndex, size_t bitIndex)
        {
            m_map[wordIndex] |= ( (size_t)(1) << bitIndex);
        }

        void inline Free(size_t wordIndex, size_t bitIndex)
        {
            m_map[wordIndex] &= ~( (size_t)(1) << bitIndex );
        }

        bool inline IsInUse(size_t wordIndex, size_t bitIndex) const
        {
            return (m_map[wordIndex] & ((size_t)(1) << bitIndex)) != 0 ? true : false;
        }

        size_t* m_map;
        byte* m_memory;
        size_t m_mapWords;
        size_t m_unitSize;
        size_t m_pageByteCount;        

        SmallObjectPage(const SmallObjectPage& src) = delete;
        SmallObjectPage& operator = (const SmallObjectPage& src) = delete;
    };

}}//qor::memory

#endif//QOR_PP_H_MEMORY_THREAD_SMALLOBJECTHEAP_PAGE
