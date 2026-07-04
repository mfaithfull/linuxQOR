// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MEMORY_DEFAULTSOURCE
#define QOR_PP_H_MEMORY_DEFAULTSOURCE

#include <stddef.h>
#include "src/platform/compiler/compiler.h"

namespace qor{

    struct DefaultSource final
    {
    public:
    
        static inline byte* Source(size_t byteCount)
        {
            return new byte[byteCount];
        }

        static inline void Free(byte* memory, size_t /*ignored*/)
        {
            delete[] memory;
        }

    private:

        DefaultSource() = delete;
        ~DefaultSource() = delete;
        DefaultSource(const DefaultSource & src) = delete;
        DefaultSource& operator = (const DefaultSource & src) = delete;

    };

}//qor

#endif//QOR_PP_H_MEMORY_DEFAULTSOURCE
