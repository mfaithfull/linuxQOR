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

#ifndef QOR_PP_H_COMPONENT_THREADMEMORY_FASTHEAP_STACKPAGE
#define QOR_PP_H_COMPONENT_THREADMEMORY_FASTHEAP_STACKPAGE

#include "src/platform/compiler/compiler.h"

namespace qor{ namespace components{ namespace threadmemory{

    class qor_pp_module_interface(QOR_THREADMEMORY) StackPage final
    {
    public:

        static constexpr size_t c_pageSize = 1024 * 64; //64K pages

        void* operator new(size_t sz);
        void operator delete(void*);

        StackPage(size_t PageUnits = 1);
        ~StackPage();
        size_t TotalSizeBytes() const;
        size_t AllocatedByteCount() const;
        size_t AllocatedItemsCount(void) const;
        void SetSize(size_t pageUnits);
        StackPage* Next() const;
        StackPage* Previous() const;
        void SetNext(StackPage* next);
        void SetPrevious(StackPage* prev);
        void Initialise();
        void* Allocate(size_t byteCount);
        bool Free(void* memory, size_t byteCount);

    private:

        byte* Push(size_t byteCount);
        byte* Pop(size_t byteCount);
        byte* m_memoryBase;
        size_t m_pageUnits;
        byte* m_ToS;
        StackPage* m_next;
        StackPage* m_prev;
        size_t m_items;
    };

}}}//qor::components::threadmemory

#endif//QOR_PP_H_COMPONENT_THREADMEMORY_FASTHEAP_STACKPAGE
