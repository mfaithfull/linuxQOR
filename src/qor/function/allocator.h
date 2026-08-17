// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FUNCTION_TEF_ALLOCATOR
#define QOR_PP_H_FUNCTION_TEF_ALLOCATOR

#include <cstdlib>
#include <utility>

namespace qor { namespace tef {

    // Type for custom memory allocation functions.
    // Allocates the requested number of bytes with the provided alignment.
    //
    // Arguments:
    // - size: the number of bytes to allocate.
    // - alignment: the required alignment for the allocation in bytes.
    // - context: a value provided when setting up the function.
    //
    // Returns: a pointer to the allocated memory. Returning a null pointer will
    // cause a fatal kCustomAllocator error.
    using AllocationFunc = void* (*)(size_t size, size_t alignment, void* context);

    // Type for custom memory deallocation functions.
    // Deallocates memory previously allocated in a provided address.
    //
    // Arguments:
    // - address: the address of the memory to deallocate.
    // - size: the size in bytes requested when allocating the memory.
    // - alignment: the alignment in bytes requested when allocating the memory.
    // - context: a value provided when setting up the function.
    //
    // Returns: true on success, false on failure. Returning false will cause a
    // fatal kCustomAllocator error.
    using DeallocationFunc = bool (*)(void* address, size_t size, size_t alignment, void* context);

    // Allows access to the custom allocator, if any.
    qor_pp_module_interface(QOR_TEF) std::pair<AllocationFunc, void*>& CustomAllocator(); 

    // Allows access to the custom deallocator, if any.
    qor_pp_module_interface(QOR_TEF) std::pair<DeallocationFunc, void*>& CustomDeallocator();;

    // Sets custom allocator functions to use.
    //
    // Should be set only once before starting to use MagicFunc and not changed, as
    // it might lead to deallocations called on mismatching allocator functions.
    // Functions always use the current custom allocation functions if any.
    //
    // @param allocation_func Function used to allocate memory.
    // @param allocation_context Argument provided when invoking the allocator.
    // @param deallocation_func Function used to deallocate memory.
    // @param deallocation_context Argument provided when invoking the deallocator.
    inline void SetCustomAllocator( AllocationFunc allocation_func, void* allocation_context,
        DeallocationFunc deallocation_func, void* deallocation_context) 
    {
        CustomAllocator() = std::make_pair(allocation_func, allocation_context);
        CustomDeallocator() = std::make_pair(deallocation_func, deallocation_context);
    }

}}//qor::tef

#endif  // QOR_PP_H_FUNCTION_TEF_ALLOCATOR
