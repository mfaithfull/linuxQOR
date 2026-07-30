// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_APP_THREAD_SOURCE
#define QOR_PP_H_OS_WINDOWS_APP_THREAD_SOURCE

#include "heap.h"

namespace qor{ namespace win{

    class qor_pp_module_interface(QOR_WINDOWSTHREAD) ThreadSource : public ThreadMemorySource
    {
    public:
        ThreadSource(ThreadHeap& heap) : m_heap(heap){}

        virtual inline byte* Source(size_t byteCount) const
        {
            return m_heap.Alloc(0, byteCount);
        }

        virtual inline void Free(byte* memory, size_t /*ignored*/) const
        {
            m_heap.Free(0, memory);
        }

    private:

        ThreadHeap& m_heap;

    };

}}//qor::win

#endif//QOR_PP_H_OS_WINDOWS_APP_THREAD_SOURCE