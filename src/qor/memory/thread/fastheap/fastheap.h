// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MEMORY_THREAD_FASTHEAP
#define QOR_PP_H_MEMORY_THREAD_FASTHEAP

#include "src/platform/compiler/compiler.h"
#include "src/qor/essentials/objectcontext/anyobject.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/instance/instance.h"
#include "src/qor/memory/reference/ref.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/memory/instance/threadsingleton.h"
#include "fastbucket.h"

namespace qor { namespace memory {

    class qor_pp_module_interface(QOR_THREADMEMORY) FastHeap final
    {
    public:

        static constexpr size_t sc_rootBuckets = 8;
        static constexpr size_t sc_powerScale = 2;

        FastHeap();
        ~FastHeap() = default;
        void* Allocate(size_t byteCount);
        void Free(void* allocation, size_t byteCount);
        size_t TotalBytesAllocated() const;

    private:

        FastBucket* Bucket(size_t byteCount);

        FastBucket m_initialPages[sc_rootBuckets];
        size_t m_totalAlloc;
        size_t m_peakAlloc;
        size_t m_allocIndex;

    };

    }//qor::memory

    qor_pp_declare_instancer_of(memory::FastHeap, ThreadSingletonInstancer);    
}//qor

qor_pp_module_interface(QOR_THREADMEMORY) qor::detail::ThreadInstanceHolder<qor::memory::FastHeap>* GetFastHeapHolder();

#endif//QOR_PP_H_MEMORY_THREAD_FASTHEAP
