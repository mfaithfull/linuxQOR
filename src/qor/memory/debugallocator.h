// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef QOR_PP_H_MEMORY_DEBUGALLOCATOR
#define QOR_PP_H_MEMORY_DEBUGALLOCATOR

#include <cstdint>
#include <stddef.h>
#include <cstring>

#include "src/platform/compiler/compiler.h"
#include "memoryexception.h"

//The allocator decides how much memory is required and what to put in it.

namespace qor{

    //TODO: work out how to get the real allocation site info and write this function so it works
    //extern void FillInDbgInfo(uint32_t& uiLine, const char** pszFile);

    class DebugAllocator final
    {
    private:

        struct dbgInfo
        {
            size_t Count;
            size_t Size;
            uint32_t uiLine;
            const char* szFile;
            void* pBackRef;
        };

        template< class T >
        static byte* RawAllocate(size_t count)
        {
            size_t allocSize = sizeof(dbgInfo) + (count * sizeof(T));
            byte* pMem = source_of< T >::type::Source(allocSize);
            dbgInfo* pInfo = reinterpret_cast<dbgInfo*>(pMem);
            pInfo->Count = count;
            pInfo->Size = allocSize;
            pInfo->uiLine = __LINE__;
            pInfo->szFile = __FILE__;
            pInfo->pBackRef = nullptr;
            //FillInDbgInfo
            pMem += sizeof(dbgInfo);
            return pMem;
        }

    public:

        template< class T >
        static T* Allocate(size_t count = 1)
        {
            byte* pMem = RawAllocate<T>(count);
            T* pResult = reinterpret_cast<T*>(pMem);
            for (size_t element = 0; element < count; element++)
            {
                new(pMem)T();
                pMem += sizeof(T);
            }
            return pResult;
        }
        
        template< class T, typename... _p >
        static T* Allocate(size_t count, _p&&... p1)
        {
            byte* pMem = RawAllocate<T>(count);
            T* pResult = reinterpret_cast<T*>(pMem);
            for (size_t element = 0; element < count; element++)
            {
                new(pMem)T(std::forward<_p>(p1)...);
                pMem += sizeof(T);
            }
            return pResult;
        }
        
        template< class T >
        static void Free(T * pT)
        {
            if (pT != nullptr)
            {
                byte* pMem = reinterpret_cast<byte*>(pT) - sizeof(dbgInfo);
                dbgInfo* pInfo = reinterpret_cast<dbgInfo*>(pMem);
                size_t allocSize = sizeof(dbgInfo) + (pInfo->Count * sizeof(T));
                if (allocSize != pInfo->Size)
                {
                    throw memoryexception("Size of item at %X being freed %u does not match byte count allocated %u.");
                }
                for (size_t element = 0; element < pInfo->Count; element++)
                {
                    pT->~T();
                    pT++;
                }
                std::memset(pMem, 0xFD, allocSize);
                source_of< T >::type::Free(pMem, allocSize);
            }
        }
        
        template< class T >
        static T* Reallocate(T * pT, size_t oldCount, size_t newCount, bool bPreserve = true)
        {
            if (!bPreserve)
            {
                Free(pT);
                return Allocate< T >(newCount);
            }

            T* pNewT = Allocate< T >(newCount);
            size_t element = 0;
            while (element < oldCount && element < newCount)
            {
                pNewT[element] = pT[element];
                element++;
            }
            Free(pT);
            return pNewT;
        }

    private:

        DebugAllocator() = delete;
        ~DebugAllocator() = delete;

    };

}//qor

#endif//QOR_PP_H_MEMORY_DEBUGALLOCATOR
