// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FUNCTION_TEF_ERROR
#define QOR_PP_H_FUNCTION_TEF_ERROR

#include <string>
#include "src/qor/flyers/error/error.h"
#include "src/qor/essentials/datastructures/frozen/map.h"

namespace qor { namespace tef {

    // Enumeration of magic func error codes.
    enum class Error 
    {    
        kInvalidFunction,       // The function to call is invalid or not set.
        kInvalidObject,         // The object to call is invalid or not set.
        kInvalidCast,           // Invalid type cast. Actual types do not match.
        kIncompatibleType,      // Actual types behind type erasure are not compatible.
        kNonCopyableObject,     // Object not copyable. Type-erased object is not copy-constructible.
        kCustomAllocator,       // Custom allocator failed to allocate or deallocate the memory.
    };
    
    constexpr frozen::map<const Error, const char*, 6> tefError = {{
        {Error::kInvalidFunction, "The function to call is invalid or not set."},
        {Error::kInvalidObject, "The object to call is invalid or not set."},
        {Error::kInvalidCast, "Invalid type cast. Actual types do not match."},
        {Error::kIncompatibleType, "Actual types behind type erasure are not compatible."},
        {Error::kNonCopyableObject, "Object not copyable. Type-erased object is not copy-constructible."},
        {Error::kCustomAllocator, "Custom allocator failed to allocate or deallocate the memory."}
    }};
    
    inline void check(bool b, Error error) 
    {
        if (!b) 
        {
            serious(tefError.at(error));
        }
    }

#if !defined(NDEBUG)   
    template< typename T>
    inline void debug_check(T b, Error error) 
    {

        if (!b) 
        {
            serious(tefError.at(error));
        }
    }
#else
    #define debug_check(b, error) ((void)0)
#endif

}}//qor::tef

#endif//QOR_PP_H_FUNCTION_TEF_ERROR
