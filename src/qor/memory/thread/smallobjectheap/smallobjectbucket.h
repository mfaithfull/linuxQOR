// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MEMORY_THREAD_SMALLOBJECTHEAP_BUCKET
#define QOR_PP_H_MEMORY_THREAD_SMALLOBJECTHEAP_BUCKET

#include "smallobjectpage.h"

namespace qor{ namespace memory{

    class qor_pp_module_interface(QOR_THREADMEMORY) SmallObjectBucket final
    {
    public:

        void* operator new(size_t sz);
        void operator delete(void*);

        SmallObjectBucket(size_t unitSize);
        ~SmallObjectBucket();

        byte* Allocate();
        
        inline bool IsEmpty()
        {
            return m_allocCount == 0 ? true : false;
        }

        bool Free(byte* element);

        inline size_t UnitSize() const
        {
            return m_unitSize;
        }

        inline byte GetCacheLimit() const
        {
            return m_cacheLimit;
        }

        inline void SetCacheLimit(byte pages)
        {
            m_cacheLimit = pages;
        }

        inline byte GetCacheCount() const
        {
            return m_cacheCount;
        }

        inline size_t AllocatedItems(void) const
        {
            return m_allocCount;
        }

    private:

        void AddOnePageToCache(SmallObjectPage* page);
        void FlushOnePageFromCache();
        void FreePage(SmallObjectPage* checkPage);

        size_t m_unitSize;
        SmallObjectPage* m_lastPage;
        size_t m_allocCount;
        SmallObjectPage* m_pageCache;
        byte m_cacheCount;
        byte m_cacheLimit;
    };

}}//qor::memory

#endif//QOR_PP_H_MEMORY_THREAD_SMALLOBJECTHEAP_BUCKET
