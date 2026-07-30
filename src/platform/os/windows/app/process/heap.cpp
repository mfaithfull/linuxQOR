// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "heap.h"

#include "src/platform/os/windows/api_layer/kernel/kernel32.h"

namespace qor{ namespace win{

    ProcessHeap::ProcessHeap()
    {
        m_handle = api::Kernel32::GetProcessHeap();
    }

    ProcessHeap::~ProcessHeap() noexcept = default;

    byte* ProcessHeap::Alloc(unsigned long flags, size_t bytes)
    {
        return reinterpret_cast<byte*>(api::Kernel32::HeapAlloc(m_handle(), flags, bytes));
    }

    size_t ProcessHeap::Compact(unsigned long flags)
    {
        return api::Kernel32::HeapCompact(m_handle(), flags);
    }

    bool ProcessHeap::Free(unsigned long flags, byte* mem)
    {
        return api::Kernel32::HeapFree(m_handle(), flags, mem) ? true : false;
    }

    bool ProcessHeap::Lock()
    {
        return api::Kernel32::HeapLock(m_handle()) ? true : false;
    }

    bool ProcessHeap::QueryInformation(HeapInformation informationClass, void* information, size_t informationLength, size_t& returnLength)
    {
        return api::Kernel32::HeapQueryInformation(m_handle(), (HEAP_INFORMATION_CLASS)(informationClass), information, informationLength, &returnLength) ? true : false;
    }

    byte* ProcessHeap::ReAlloc(unsigned long flags, byte* mem, size_t bytes)
    {
        return reinterpret_cast<byte*>(api::Kernel32::HeapReAlloc(m_handle(), flags, mem, bytes));
    }

    bool ProcessHeap::SetInformation(HeapInformation informationClass, void* information, size_t informationLength)
    {
        return api::Kernel32::HeapSetInformation(m_handle(), (HEAP_INFORMATION_CLASS)(informationClass), information, informationLength) ? true : false;
    }

    size_t ProcessHeap::Size(unsigned long flags, const byte* mem)
    {
        return api::Kernel32::HeapSize(m_handle(), flags, mem);
    }

    bool ProcessHeap::Unlock()
    {
        return api::Kernel32::HeapUnlock(m_handle()) ? true : false;
    }

    bool ProcessHeap::Validate(unsigned long flags, const byte* mem)
    {
        return api::Kernel32::HeapValidate(m_handle(), flags, mem) ? true : false;
    }

    bool ProcessHeap::Walk(Entry* entry)
    {
        return api::Kernel32::HeapWalk(m_handle(), reinterpret_cast<LPPROCESS_HEAP_ENTRY>(entry)) ? true : false;
    }

}}//qor::win
