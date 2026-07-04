// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MEMORY
#define QOR_PP_H_MEMORY

#include "src/platform/compiler/compiler.h"
#include "sources/defaultsource.h"

//Trait for type T that determines where it's raw memory comes from
//e.g. from the global OS heap or a special arena

namespace qor{

    template<typename T>
    struct source_of
    {
        typedef DefaultSource type;
    };

}//qor

//Preprocessor macro shorthand for declaring a source_of specialisation
#   define qor_pp_declare_source_of(_CLASS,_SOURCE)\
template<> struct source_of< _CLASS >\
{\
    typedef _SOURCE type;\
};
//Example: qor_pp_declare_source_of(MyClass, DefaultSource);


#include "allocators/debugallocator.h"
#include "allocators/releaseallocator.h"

namespace qor{

    template<typename T>
    struct allocator_of
    {
#   ifndef NDEBUG
        typedef DebugAllocator type;
#   else
        typedef ReleaseAllocator type;
#   endif
    };

}//qor

//Preprocessor macro shorthand for declaring a allocator_of specialisation
#   define qor_pp_declare_allocator_of(_CLASS,_ALLOCATOR)\
template<> struct qor::allocator_of< _CLASS >\
{\
    typedef qor::_ALLOCATOR type;\
};
//Example: qor_pp_declare_allocator_of(Biscuits, SpecialAllocator);

#endif//QOR_PP_H_MEMORY
