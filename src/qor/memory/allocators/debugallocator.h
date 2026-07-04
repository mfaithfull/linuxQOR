// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MEMORY_DEBUGALLOCATOR
#define QOR_PP_H_MEMORY_DEBUGALLOCATOR

#include <cstdint>
#include <stddef.h>
#include <cstring>

#include "src/platform/compiler/compiler.h"
#include "../exceptions/memoryexception.h"

//The allocator decides how much memory is required and what to put in it.

namespace qor{

    struct DebugAllocator final
    {
    private:

        struct dbgInfo
        {
            size_t Count;
            size_t Size;
            uint32_t Line;
            const char* File;
            void* BackRef;
        };

        template< class T >
        static byte* RawAllocate(size_t count)
        {
            size_t allocSize = sizeof(dbgInfo) + (count * sizeof(T));
            byte* mem = source_of< T >::type::Source(allocSize);
            if(mem)
            {
                dbgInfo* info = reinterpret_cast<dbgInfo*>(mem);
                info->Count = count;
                info->Size = allocSize;
                info->Line = __LINE__;//TODO: Get these from Function Context if possible
                info->File = __FILE__;
                info->BackRef = nullptr;
                mem += sizeof(dbgInfo);
            }
            return mem;
        }

    public:

        template< class T >
        static T* Allocate(size_t count = 1)
        {
            byte* mem = RawAllocate<T>(count);
            T* result = reinterpret_cast<T*>(mem);
            for (size_t element = 0; mem && element < count; element++)
            {
                new(mem)T();
                mem += sizeof(T);
            }
            return result;
        }
        
        template< class T, typename... _p >
        static T* Allocate(size_t count, _p&&... p1)
        {
            byte* mem = RawAllocate<T>(count);
            T* result = reinterpret_cast<T*>(mem);
            for (size_t element = 0; mem && element < count; element++)
            {
                new(mem)T(std::forward<_p&&>(p1)...);
                mem += sizeof(T);
            }
            return result;
        }
        
        template< class T >
        static void Free(T* t)
        {
            if (t != nullptr)
            {
                byte* mem = reinterpret_cast<byte*>(t) - sizeof(dbgInfo);
                dbgInfo* info = reinterpret_cast<dbgInfo*>(mem);
                size_t allocSize = sizeof(dbgInfo) + (info->Count * sizeof(T));
                if (allocSize > info->Size)
                {
                    throw memoryexception("Size of item at {0:p} being freed {1} is larger than the byte count allocated {2}.", (void*)mem, allocSize, info->Size);
                }
                for (size_t element = 0; element < info->Count; element++)
                {
                    t->~T();
                    t++;
                }
                std::memset(mem, 0xFD, allocSize);
                source_of< T >::type::Free(mem, allocSize);
            }
        }
        
        template< class T >
        static T* Reallocate(T* t, size_t oldCount, size_t newCount, bool preserve = true)
        {
            if (!preserve)
            {
                Free(t);
                return Allocate< T >(newCount);
            }

            T* newT = Allocate< T >(newCount);
            size_t element = 0;
            while (element < oldCount && element < newCount)
            {
                newT[element] = t[element];
                element++;
            }
            Free(t);
            return newT;
        }

    private:

        DebugAllocator() = delete;
        ~DebugAllocator() = delete;

    };

}//qor

#endif//QOR_PP_H_MEMORY_DEBUGALLOCATOR
