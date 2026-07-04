// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MEMORY_THREAD_FASTHEAP_STACKPAGE
#define QOR_PP_H_MEMORY_THREAD_FASTHEAP_STACKPAGE

#include "src/platform/compiler/compiler.h"
#include "../threadheap/threadheap.h"

namespace qor{ namespace memory{

    class qor_pp_module_interface(QOR_THREADMEMORY) StackPage final
    {
    public:

        static constexpr size_t c_pageSize = 1024 * 64; //64K pages

        inline void* operator new(size_t sz)
        {
            return new_ref<ThreadHeap>()->Allocate(sz);
        }

        inline void operator delete(void* allocation)
        {
            new_ref<ThreadHeap>()->Free(reinterpret_cast<byte*>(allocation));
        }

        StackPage(size_t PageUnits = 1);

        inline ~StackPage()
        {
            new_ref<ThreadHeap>()->Free(m_memoryBase);
        }

        inline size_t TotalSizeBytes() const
        {
            return m_memoryBase ? (m_pageUnits * c_pageSize) : 0;
        }

        inline size_t AllocatedByteCount() const
        {
            return m_ToS - m_memoryBase;
        }

        inline size_t AllocatedItemsCount(void) const
        {
            return m_items;
        }

        inline void SetSize(size_t pageUnits)
        {
            m_pageUnits = pageUnits;
        }

        inline StackPage* Next() const
        {
            return m_next;
        }

        inline StackPage* Previous() const
        {
            return m_prev;
        }

        inline void SetNext(StackPage* next)
        {
            m_next = next;
        }

        inline void SetPrevious(StackPage* prev)
        {
            m_prev = prev;
        }

        void Initialise();
        void* Allocate(size_t byteCount);
        bool Free(void* memory, size_t byteCount);

    private:

        byte* Push(size_t byteCount);
        byte* Pop(size_t byteCount);

        byte* m_memoryBase;
        size_t m_pageUnits;
        byte* m_ToS;
        StackPage* m_next;
        StackPage* m_prev;
        size_t m_items;
    };

}}//qor::memory

#endif//QOR_PP_H_MEMORY_THREAD_FASTHEAP_STACKPAGE
