// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <string.h>
#include "fastheap.h"
#include "../threadheap/threadheap.h"

using namespace qor;


thread_local detail::ThreadInstanceHolder<memory::FastHeap> ThreadInstanceHolderFastHeap;

qor_pp_export detail::ThreadInstanceHolder<memory::FastHeap>* GetFastHeapHolder()
{
    return &ThreadInstanceHolderFastHeap;
}

namespace qor { namespace detail {

    template<>
    ThreadInstanceHolder<memory::FastHeap>* theThreadInstanceHolder<memory::FastHeap>()
    {
        return GetFastHeapHolder();
    }
}}

namespace qor{ namespace memory{

    FastHeap::FastHeap()
    {
        m_allocIndex = 0;
        m_totalAlloc = 0;
        m_peakAlloc = 0;

        size_t BucketSize = 1;
        for (size_t Bucket = 0; Bucket < sc_rootBuckets; Bucket++)
        {
            m_initialPages[Bucket].SetSize(BucketSize);
            BucketSize *= sc_powerScale;
        }
    }

    void* FastHeap::Allocate(size_t byteCount)
    {
        FastBucket* bucket = Bucket(byteCount);
        void* memory = bucket ? bucket->Allocate(byteCount) : nullptr;

        if(memory)
        {
            m_totalAlloc += (byteCount);
            if (m_totalAlloc > m_peakAlloc)
            {
                m_peakAlloc = m_totalAlloc;
            }
        }
        return memory;
    }	

    void FastHeap::Free(void* allocation, size_t byteCount)
    {
        FastBucket* bucket = Bucket(byteCount);
        if (bucket && bucket->Free(allocation, byteCount))
        {
            m_totalAlloc -= (byteCount);
        }
        else
        {
            throw memoryexception("Memory to free at {0:p} not found in Fast Heap.", (void*)allocation);
        }
    }

    size_t FastHeap::TotalBytesAllocated() const
    {
        return m_totalAlloc;
    }

    FastBucket* FastHeap::Bucket(size_t byteCount)
    {
        size_t basePage = 0;
        FastBucket* bucket = &m_initialPages[basePage];
        while ((basePage < sc_rootBuckets) && ((byteCount * 7) > bucket->PageSize()))
        {
            bucket = &m_initialPages[++basePage];
        }
        if (basePage < sc_rootBuckets)
        {
            bucket->Initialise();
        }
        else
        {
            bucket = nullptr;
        }
        return bucket;
    }

}}//qor::memory