// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "allocator.h"

namespace qor { namespace tef {

    // Allows access to the custom allocator, if any.
    std::pair<AllocationFunc, void*>& CustomAllocator() 
    {
        static std::pair<AllocationFunc, void*> allocator(nullptr, nullptr);
        return allocator;
    }

    // Allows access to the custom deallocator, if any.
    std::pair<DeallocationFunc, void*>& CustomDeallocator() 
    {
        static std::pair<DeallocationFunc, void*> deallocator(nullptr, nullptr);
        return deallocator;
    }

}}//qor::tef

