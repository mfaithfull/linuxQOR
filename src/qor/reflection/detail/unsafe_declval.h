// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_DETAIL_UNSAFEDECLVAL
#define QOR_PP_H_REFLECTION_DETAIL_UNSAFEDECLVAL

#pragma once

#include "config.h"

#ifdef qor_pp_refl_has_std_module
import std;
#else
#include <type_traits>
#endif

namespace qor_reflection { namespace detail {

    // This function serves as a link-time assert. If linker requires it, then
    // `unsafe_declval()` is used at runtime.
    void report_if_you_see_link_error_with_this_function() noexcept;

    // For returning non default constructible types. Do NOT use at runtime!
    //
    // GCCs std::declval may not be used in potentionally evaluated contexts,
    // so we reinvent it.
    template <class T>
    constexpr T unsafe_declval() noexcept 
    {
        report_if_you_see_link_error_with_this_function();

        typename std::remove_reference<T>::type* ptr = nullptr;
        ptr += 42; // suppresses 'null pointer dereference' warnings
        return static_cast<T>(*ptr);
    }

}}//qor_reflection::detail

#endif//QOR_PP_H_REFLECTION_DETAIL_UNSAFEDECLVAL