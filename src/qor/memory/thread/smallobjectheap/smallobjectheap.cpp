// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <string.h>
#include "smallobjectheap.h"
#include "../fastheap/fastheap.h"

using namespace qor;

thread_local detail::ThreadInstanceHolder<memory::SmallObjectHeap> ThreadInstanceHolderSmallObjectHeap;

qor_pp_export detail::ThreadInstanceHolder<memory::SmallObjectHeap>* GetCurrentSmallObjectHeap()
{
    return &ThreadInstanceHolderSmallObjectHeap;
}

namespace qor { namespace detail {

    template<>
    qor_pp_export ThreadInstanceHolder<memory::SmallObjectHeap>* theThreadInstanceHolder<memory::SmallObjectHeap>()
    {
        return GetCurrentSmallObjectHeap();
    }
}}//qor::detail

namespace qor{ namespace memory{

    SmallObjectHeap::SmallObjectHeap()
    {
        memset(m_buckets, 0, sizeof(char*) * sc_rootBuckets);
        m_totalAlloc = 0;
        m_peakAlloc = 0;

    }

    SmallObjectHeap::~SmallObjectHeap()
    {
        for(size_t bucketIndex = 0; bucketIndex < sc_rootBuckets; ++bucketIndex)
        {
            if(m_buckets[bucketIndex] != nullptr)
            {
                FreeBucket(bucketIndex, m_buckets[bucketIndex]);
            }
        }
    }

    byte* SmallObjectHeap::Allocate(size_t size)
    {
        byte* memory = nullptr;
        if( size > (1024 * 64))
        {
            memory = DeferToFastAllocator(size);
        }
        else
        {
            SmallObjectBucket* bucket = Bucket(BucketIndex(size));
            if(bucket)
            {
                memory = bucket->Allocate();
                if(memory)
                {
                    m_totalAlloc += bucket->UnitSize();
                    if(m_totalAlloc > m_peakAlloc)
                    {
                        m_peakAlloc = m_totalAlloc;
                    }
                }
                else
                {
                    throw memoryexception("The Small Object Heap has run out of space. Total allocation {0} bytes.", m_totalAlloc);
                }
            }
            else
            {
                throw memoryexception("Small Object Heap bucket not fond for request of size {0} bytes.", size);
            }
        }

        return memory;
    }

    void SmallObjectHeap::Free(byte* memory, size_t size)
    {
        if(size <= 1024 * 64)
        {
            size_t bucketIndex = BucketIndex(size);
            SmallObjectBucket* bucket = m_buckets[bucketIndex];
            if(bucket && bucket->Free(memory))
            {
                m_totalAlloc -= bucket->UnitSize();
                if(bucket->IsEmpty() && (bucket->GetCacheCount() == 0))
                {
                    FreeBucket(bucketIndex, bucket);
                }
            }
            else
            {
                throw memoryexception("The Small Object Heap can't free block at {0:p} from bucket at {1:p}.", (void*)memory, (void*)bucket);
            }
        }
        else
        {
            DeferToFastFree(memory, size);
        }
    }

    size_t SmallObjectHeap::TotalAllocation(void) const
    {
        return m_totalAlloc;
    }

    size_t SmallObjectHeap::PeakAllocation() const
    {
        return m_peakAlloc;
    }

    void SmallObjectHeap::SetCacheLimit(byte pages)
    {
        for (size_t bucketIndex = 0; bucketIndex < sc_rootBuckets; ++bucketIndex)
        {
            if (m_buckets[bucketIndex] != nullptr)
            {
                m_buckets[bucketIndex]->SetCacheLimit(pages);
            }
        }
    }

    byte* SmallObjectHeap::DeferToFastAllocator(size_t size)
    {
        return reinterpret_cast<byte*>(new_ref<FastHeap>()->Allocate(size));
    }

    void SmallObjectHeap::DeferToFastFree(void* mem, size_t size)
    {
        new_ref<FastHeap>()->Free(mem, size);
    }

    void SmallObjectHeap::FreeBucket(size_t bucketIndex, SmallObjectBucket* bucket)
    {
        m_buckets[bucketIndex] = nullptr;
        delete bucket;
    }

    size_t SmallObjectHeap::BucketIndex(size_t allocSize)
    {
        size_t bucketIndex = 0;
        SmallObjectBucket* bucket = nullptr;
        do
        {
        } while ((allocSize > (size_t)(4 << bucketIndex)) && (++bucketIndex < sc_rootBuckets));
        bucket = m_buckets[bucketIndex];
        if (bucket == nullptr && (allocSize <= (size_t)(4 << bucketIndex)))
        {
            m_buckets[bucketIndex] = new SmallObjectBucket(4 << bucketIndex);
        }

        return bucketIndex;
    }

    SmallObjectBucket* SmallObjectHeap::Bucket(size_t bucketIndex)          //Get the bucket from the index
    {
        SmallObjectBucket* bucket = nullptr;
        if (bucketIndex < sc_rootBuckets)
        {
            bucket = m_buckets[bucketIndex];
        }
        return bucket;
    }

}}//qor::memory
