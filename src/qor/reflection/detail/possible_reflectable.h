// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_DETAIL_POSSIBLEREFLECTABLE
#define QOR_PP_H_REFLECTION_DETAIL_POSSIBLEREFLECTABLE

#pragma once

#include "config.h"
#include "../traits_fwd.h"

#ifdef qor_pp_refl_has_std_module
import std;
#else
#include <type_traits> // for std::is_aggregate
#endif

namespace qor_reflection { namespace detail {

    ///////////////////// Returns false when the type exactly wasn't be reflectable
    template <class T, class WhatFor>
    constexpr decltype(is_reflectable<T, WhatFor>::value) possible_reflectable(long) noexcept 
    {
        return is_reflectable<T, WhatFor>::value;
    }

#if qor_pp_refl_enable_implicit_reflection

    template <class T, class WhatFor>
    constexpr bool possible_reflectable(int) noexcept 
    {
#   if  defined(__cpp_lib_is_aggregate)
        using type = std::remove_cv_t<T>;
        return std::is_aggregate<type>();
#   else
        return true;
#   endif
    }

#else

    template <class T, class WhatFor>
    constexpr bool possible_reflectable(int) noexcept 
    {
        // negative answer here won't change behaviour in PFR-dependent libraries(like Fusion)
        return false;
    }

#endif

}} // namespace qor_reflection::detail

#endif//QOR_PP_H_REFLECTION_DETAIL_POSSIBLEREFLECTABLE
