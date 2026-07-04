// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MEMORY_THREAD_SMALLOBJECTHEAP
#define QOR_PP_H_MEMORY_THREAD_SMALLOBJECTHEAP

#include "src/platform/compiler/compiler.h"
#include "src/qor/essentials/objectcontext/anyobject.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/instance/instance.h"
#include "src/qor/memory/reference/ref.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/memory/instance/threadsingleton.h"
#include "smallobjectbucket.h"

namespace qor{ namespace memory{

    class qor_pp_module_interface(QOR_THREADMEMORY) SmallObjectHeap final
    {
    public:

        static constexpr size_t sc_rootBuckets = (sizeof(size_t) * 8) / 4;// power of 4 buckets up to the memory space size

        SmallObjectHeap();
        ~SmallObjectHeap();

        byte* Allocate(size_t size);
        void Free(byte* p, size_t size);
        size_t TotalAllocation(void) const;
        size_t PeakAllocation() const;
        void SetCacheLimit(byte Pages);

    private:

        byte* DeferToFastAllocator(size_t size);
        void DeferToFastFree(void* mem, size_t size);
        void FreeBucket(size_t bucketIndex, SmallObjectBucket* bucket);
        size_t BucketIndex(size_t allocSize);					        //Get the bucket index from the size of allocation
        SmallObjectBucket* Bucket(size_t bucketIndex);                  //Get the bucket from the index

        SmallObjectBucket* m_buckets[sc_rootBuckets];		            //Array of buckets to contain allocations        
        size_t m_totalAlloc;							                //The total allocated bytes
        size_t m_peakAlloc;								                //The highest total allocated bytes
    };

}}//qor::memory

namespace qor{ qor_pp_declare_instancer_of(memory::SmallObjectHeap, ThreadSingletonInstancer);}

qor_pp_module_interface(QOR_THREADMEMORY) qor::detail::ThreadInstanceHolder<qor::memory::SmallObjectHeap>* GetCurrentSmallObjectHeap();

#endif//QOR_PP_H_MEMORY_THREAD_SMALLOBJECTHEAP
