// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FUNCTION_TEF_ERROR
#define QOR_PP_H_FUNCTION_TEF_ERROR

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

// Macro called in case of error.
//
// By default, throws an error exception if exceptions are enabled, or just
// terminates otherwise.
//
// Can be customized as needed, but continuing to run after an error is
// undefined behavior and can cause crashes or subtle issues.
#if !defined(MAGIC_FUNC_ERROR)
#if defined(__EXCEPTIONS) || defined(_HAS_EXCEPTIONS) || defined(_CPPUNWIND)
#define MAGIC_FUNC_ERROR(error) throw (error);
#else
#define MAGIC_FUNC_ERROR(error) std::terminate();
#endif
#endif

// Auxiliary macro for unconditional assertions.
//
// Used to assert conditions that are severe enough to be verified in all kinds
// of builds, not only debug ones.
//
// Unconditional assertions include runtime type checks in function casts, where
// incorrect casts can have many dangerous and hard to find consequences.
#define MAGIC_FUNC_CHECK(cond, error) \
    if (!(cond)) { MAGIC_FUNC_ERROR((error)); }

// Auxiliary macro for debug-mode assertions.
//
// Used to assert conditions in debug builds that are not critical and might
// otherwise decrease performance.
//
// Debug-mode assertions involve checking for the validity of functions and
// objects, where if not valid a crash involving nullptr is likely to happen.
//
// Debug-mode assertions can be disabled by defining the NDEBUG macro.
#if !defined(NDEBUG)
#define MAGIC_FUNC_DCHECK(cond, error) \
    if (!(cond)) { MAGIC_FUNC_ERROR((error)); }
#else
#define MAGIC_FUNC_DCHECK(cond, error)
#endif

}}//qor::tef

#endif//QOR_PP_H_FUNCTION_TEF_ERROR
