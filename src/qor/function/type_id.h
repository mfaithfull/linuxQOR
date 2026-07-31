// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FUNCTION_TEF_TYPE_ID
#define QOR_PP_H_FUNCTION_TEF_TYPE_ID

#include <cstdint>

#include "port.h"

namespace qor { namespace tef {

using TypeId = intptr_t;

// Visual Studio applies the linker flags /OPT:ICF by default in Release builds,
// which merges identical functions into the same address. This makes the usual
// get_type_id approach to fail because all type ids collapse to the same value.
//
// Instead, for Visual Studio Release builds, or if the macro MSC_OPT_NOICF is
// manually defined by the user, we use an alternative approach where the id
// comes from the address of a static variable in a template function.
//
// Note that neither of these approaches produce ids that should:
// 1. Be serialized.
// 2. Be shared across processes.
// 3. Be used across Windows DLL boundaries.
//
#if defined(_MSC_VER) && !defined(_DEBUG) && !defined(MSC_OPT_NOICF)
    template <typename... T>
    TypeId get_type_id() noexcept {
    static uint8_t id;
    return reinterpret_cast<TypeId>(&id);
    }
#else
    template <typename... T>
    constexpr TypeId get_type_id() noexcept 
    {
    // The double reinterpret_cast is to workaround a MSVC compiler warning.
    return reinterpret_cast<TypeId>(reinterpret_cast<void*>(&get_type_id<T...>));
    }
#endif

}}//qor::tef

#endif//QOR_PP_H_FUNCTION_TEF_TYPE_ID
