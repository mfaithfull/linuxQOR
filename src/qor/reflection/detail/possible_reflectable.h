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
