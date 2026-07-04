// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MEMORY_RELEASEALLOCATOR
#define QOR_PP_H_MEMORY_RELEASEALLOCATOR

#include <cstdint>
#include <stddef.h>

#include "src/platform/compiler/compiler.h"

//The allocator decides how much memory is required and what to put in it.

namespace qor{

    struct ReleaseAllocator final
    {
    private:

        struct Info
        {
            size_t Count;
            size_t Size;
            void* BackRef;
        };
     
        template< class T >
        static byte* RawAllocate(size_t count)
        {
            size_t allocSize = sizeof(Info) + (count * sizeof(T));
            byte* mem = source_of< T >::type::Source(allocSize);
            if(mem)
            {
                Info* info = reinterpret_cast<Info*>(mem);
                info->Count = count;
                info->Size = allocSize;
                info->BackRef = nullptr;
                mem += sizeof(Info);
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
        static T* Allocate(size_t count, _p... p1)
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
        static void Free(T * t)
        {
            if (t != nullptr)
            {
                byte* mem = reinterpret_cast<byte*>(t) - sizeof(Info);
                Info* info = reinterpret_cast<Info*>(mem);
                size_t allocSize = sizeof(Info) + (info->Count * sizeof(T));
                if (allocSize > info->Size)
                {
                    throw memoryexception("Size of item at {0:p} being freed {1} is larger than the byte count allocated {2}.", (void*)mem, allocSize, info->Size);
                }
                for (size_t element = 0; element < info->Count; element++)
                {
                    t->~T();
                    t++;
                }
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

        ReleaseAllocator() = delete;
        ~ReleaseAllocator() = delete;

    };

}//qor

#endif//QOR_PP_H_MEMORY_RELEASEALLOCATOR
