// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MEMORY_THREAD_HEAP
#define QOR_PP_H_MEMORY_THREAD_HEAP

#include "src/qor/essentials/objectcontext/anyobject.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/instance/instance.h"
#include "src/qor/memory/reference/ref.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/memory/instance/threadsingleton.h"

namespace qor
{
    bool qor_pp_module_interface(QOR_THREADMEMORY) ImplementsThreadHeap();

    namespace memory{

    class qor_pp_module_interface(QOR_THREADMEMORY) ThreadHeap
    {
    public:

        ThreadHeap();
        virtual ~ThreadHeap() = default;

        byte* Allocate(size_t byteCount);
        void Free(byte* allocation);

    };

}}//qor::memory

namespace qor{ qor_pp_declare_instancer_of(memory::ThreadHeap, ThreadSingletonInstancer);}

qor_pp_module_interface(QOR_THREADMEMORY) qor::detail::ThreadInstanceHolder<qor::memory::ThreadHeap>* GetCurrentThreadHeap();    

#endif//QOR_PP_H_MEMORY_THREAD_HEAP
