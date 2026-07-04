// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MEMORY_FASTSOURCE
#define QOR_PP_H_MEMORY_FASTSOURCE

#include <cstdint>
#include <stddef.h>

#include "src/platform/compiler/compiler.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/memory/thread/fastheap/fastheap.h"

namespace qor{ namespace memory{

    class qor_pp_module_interface(QOR_THREADMEMORY) FastSource final
    {
    public:
    
        static inline byte* Source(size_t byteCount)
        {
            return reinterpret_cast<byte*>(new_ref<FastHeap>()->Allocate(byteCount));
        }

        static inline void Free(byte* memory, size_t byteCount)
        {
            new_ref<FastHeap>()->Free(memory, byteCount);
        }

    private:

        FastSource() = delete;
        ~FastSource() = delete;
        FastSource(const FastSource & src) = delete;
        FastSource& operator = (const FastSource & src) = delete;

    };

}}//qor::memory

#endif//QOR_PP_H_MEMORY_FASTSOURCE
