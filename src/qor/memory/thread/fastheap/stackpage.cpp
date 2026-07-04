// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <string.h>
#include "stackpage.h"

namespace qor{ namespace memory{

    StackPage::StackPage(size_t pageUnits) : 
        m_memoryBase(nullptr),
        m_pageUnits(pageUnits), 
        m_ToS(nullptr), 
        m_next(nullptr), 
        m_prev(nullptr), 
        m_items(0){ }

    void StackPage::Initialise()
    {
        if(m_memoryBase == nullptr && (m_pageUnits > 0))
        {
            m_memoryBase = reinterpret_cast<byte*>(new_ref<ThreadHeap>()->Allocate(m_pageUnits * c_pageSize));
            m_ToS = m_memoryBase;
            m_items = 0;
        }
    }

    void* StackPage::Allocate(size_t byteCount)
    {
        byte* mem = nullptr;
        if((m_memoryBase + (m_pageUnits * c_pageSize)) > (m_ToS + byteCount + sizeof(StackPage*)))
        {
            mem = Push(byteCount + sizeof(StackPage*));
            *(reinterpret_cast<StackPage**>(mem)) = this;
            mem += sizeof(StackPage*);
            m_items++;
        }
        return mem;
    }

    bool StackPage::Free(void* memory, size_t byteCount)
    {
        bool result = false;
        byte* mem = reinterpret_cast<byte*>(memory) - sizeof(StackPage*);
        byteCount += sizeof(StackPage*);
        if((mem >= m_memoryBase) && (mem <= (m_ToS - byteCount)))
        {
            if(m_ToS == (mem + byteCount))
            {
                Pop(byteCount);
            }
            else
            {
#ifndef NDEBUG
                memset(mem, 0xFD, byteCount);
#endif       
            }
            --m_items;
            if(m_items == 0)
            {
                m_ToS = m_memoryBase;
            }
            result = true;
        }
        return result;
    }

    byte* StackPage::Push(size_t byteCount)
    {
        byte* mem = m_ToS;
#ifndef NDEBUG
        memset(mem, 0x3, byteCount);
#endif
        m_ToS += byteCount;
        return mem;
    }

    byte* StackPage::Pop(size_t byteCount)
    {
        m_ToS -= byteCount;
#ifndef NDEBUG
        memset(m_ToS, 0xFD, byteCount);
#endif
        return m_ToS;
    }

}}//qor::memory
