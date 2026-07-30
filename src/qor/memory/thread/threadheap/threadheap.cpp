// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/essentials/current/currentthread.h"
#include "threadheap.h"

namespace qor {
    bool qor_pp_module_interface(QOR_THREADMEMORY) ImplementsThreadHeap(){ return true; }
}

using namespace qor;

thread_local detail::ThreadInstanceHolder<memory::ThreadHeap> ThreadInstanceHolderThreadHeap;

qor_pp_export detail::ThreadInstanceHolder<memory::ThreadHeap>* GetCurrentThreadHeap()
{
    return &ThreadInstanceHolderThreadHeap;
}

namespace qor { namespace detail {
    template<>
    qor_pp_export ThreadInstanceHolder<qor::memory::ThreadHeap>* theThreadInstanceHolder<qor::memory::ThreadHeap>()
    {
        return GetCurrentThreadHeap();
    }
}}

namespace qor{ namespace memory{

    ThreadHeap::ThreadHeap() = default;
    ThreadHeap::~ThreadHeap() = default;

    //On GNU Linux the system heap has a per thread arena already so the ThreadHeap is just the heap. Thanks Dave Lee!
    
    byte* ThreadHeap::Allocate(size_t byteCount) const
    {
#if (qor_pp_os_target == qor_pp_os_linux)
        return new byte[byteCount];
#else
        return CurrentThread::GetCurrent().GetMemorySource().value()->Source(byteCount);        
#endif
    }

    void ThreadHeap::Free(byte* allocation) const
    {
#if (qor_pp_os_target == qor_pp_os_linux)
        delete [] allocation;
#else        
        return CurrentThread::GetCurrent().GetMemorySource().value()->Free(allocation, 0);        
#endif
    }

}}//qor::memory