// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MEMORY_THREAD_FASTHEAP_BUCKET
#define QOR_PP_H_MEMORY_THREAD_FASTHEAP_BUCKET

#include "src/platform/compiler/compiler.h"
#include "stackpage.h"
#include "../threadheap/threadheap.h"

namespace qor{ namespace memory{

    class qor_pp_module_interface(QOR_THREADMEMORY) FastBucket final
    {
    public:

        inline void* operator new(size_t sz)
        {
            return new_ref<ThreadHeap>()->Allocate(sz);
        }

        inline void operator delete(void* allocation)
        {
            new_ref<ThreadHeap>()->Free(reinterpret_cast<byte*>(allocation));
        }

        FastBucket(size_t pageUnits = 1);
        ~FastBucket();

        inline size_t PageSize(void) const
        {
            return (m_pageUnits * StackPage::c_pageSize);
        }

        inline size_t AllocatedItems(void) const
        {
            return m_items;
        }

        inline size_t AllocatedPages(void) const
        {
            return m_pages;
        }

        size_t AllocatedSpace(void) const;

        void SetSize(size_t pageUnits)
        {
            m_pageUnits = pageUnits;
        }

        void Initialise();
        void* Allocate(size_t byteCount);
        bool Free(void* memory, size_t byteCount);

    private:

        void PushPage(void);
        void PopPage(void);

        size_t m_pageUnits;
        StackPage* m_basePage;
        StackPage* m_ToSPage;
        size_t m_items;
        size_t m_pages;

    };

}}//qor::memory

#endif//QOR_PP_H_MEMORY_THREAD_FASTHEAP_BUCKET
