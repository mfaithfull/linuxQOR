// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MEMORY_SMALLOBJECTSOURCE
#define QOR_PP_H_MEMORY_SMALLOBJECTSOURCE

#include <stddef.h>
#include "src/platform/compiler/compiler.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/memory/thread/smallobjectheap/smallobjectheap.h"

namespace qor{ namespace memory{

    class SmallObjectSource final
    {
    public:
    
        static byte* Source(size_t byteCount)
        {
            return reinterpret_cast<byte*>(new_ref<SmallObjectHeap>()->Allocate(byteCount));
        }

        static void Free(byte* memory, size_t byteCount)
        {
            new_ref<SmallObjectHeap>()->Free(memory, byteCount);
        }

    private:

        SmallObjectSource() = delete;
        ~SmallObjectSource() = delete;
        SmallObjectSource(const SmallObjectSource & src) = delete;
        SmallObjectSource& operator = (const SmallObjectSource & src) = delete;

    };

}}//qor::memory

#endif//QOR_PP_H_MEMORY_SMALLOBJECTSOURCE
