// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "heap.h"

#include "src/platform/os/windows/api_layer/kernel/kernel32.h"

namespace qor{ namespace win{

    ThreadHeap::ThreadHeap()
    {
        m_handle = api::Kernel32::HeapCreate(HEAP_NO_SERIALIZE, 0, 0);
    }

    ThreadHeap::~ThreadHeap() noexcept
    {
        api::Kernel32::HeapDestroy(m_handle.Use());
    }

    byte* ThreadHeap::Alloc(unsigned long flags, size_t bytes) const
    {
        return reinterpret_cast<byte*>(api::Kernel32::HeapAlloc(m_handle.Use(), flags, bytes));
    }

    size_t ThreadHeap::Compact(unsigned long flags)
    {
        return api::Kernel32::HeapCompact(m_handle.Use(), flags);
    }

    bool ThreadHeap::Free(unsigned long flags, byte* mem) const
    {
        return api::Kernel32::HeapFree(m_handle.Use(), flags, mem) ? true : false;
    }

    bool ThreadHeap::Lock()
    {
        return api::Kernel32::HeapLock(m_handle.Use()) ? true : false;
    }

    bool ThreadHeap::QueryInformation(HeapInformation informationClass, void* information, size_t informationLength, size_t& returnLength)
    {
        return api::Kernel32::HeapQueryInformation(m_handle.Use(), (HEAP_INFORMATION_CLASS)(informationClass), information, informationLength, &returnLength) ? true : false;
    }

    byte* ThreadHeap::ReAlloc(unsigned long flags, byte* mem, size_t bytes)
    {
        return reinterpret_cast<byte*>(api::Kernel32::HeapReAlloc(m_handle.Use(), flags, mem, bytes));
    }

    bool ThreadHeap::SetInformation(HeapInformation informationClass, void* information, size_t informationLength)
    {
        return api::Kernel32::HeapSetInformation(m_handle.Use(), (HEAP_INFORMATION_CLASS)(informationClass), information, informationLength) ? true : false;
    }

    size_t ThreadHeap::Size(unsigned long flags, const byte* mem)
    {
        return api::Kernel32::HeapSize(m_handle.Use(), flags, mem);
    }

    bool ThreadHeap::Unlock()
    {
        return api::Kernel32::HeapUnlock(m_handle.Use()) ? true : false;
    }

    bool ThreadHeap::Validate(unsigned long flags, const byte* mem)
    {
        return api::Kernel32::HeapValidate(m_handle.Use(), flags, mem) ? true : false;
    }

    bool ThreadHeap::Walk(Entry* entry)
    {
        return api::Kernel32::HeapWalk(m_handle.Use(), reinterpret_cast<LPPROCESS_HEAP_ENTRY>(entry)) ? true : false;
    }

}}//qor::win
