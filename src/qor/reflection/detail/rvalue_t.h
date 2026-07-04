// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_DETAIL_RVALUET
#define QOR_PP_H_REFLECTION_DETAIL_RVALUET

#pragma once

#ifdef qor_pp_refl_has_std_module
import std;
#else
#include <type_traits>
#include <utility>      // std::enable_if_t
#endif

// This header provides aliases rvalue_t and lvalue_t.
//
// Usage: template <class T> void foo(rvalue<T> rvalue);
//
// Those are useful for
//  * better type safety - you can validate at compile time that only rvalue reference is passed into the function
//  * documentation and readability - rvalue_t<T> is much better than T&&+SFINAE

namespace qor_reflection { namespace detail {

    // Binds to rvalues only, no copying allowed.
    template <class T
#ifdef qor_pp_refl_detail_strict_rvalue_testing
        , class = std::enable_if_t<std::is_rvalue_reference<T&&>::value>
#endif
    >
    using rvalue_t = T&&;

    // Binds to mutable lvalues only

}} //qor_reflection::detail

#endif//QOR_PP_H_REFLECTION_DETAIL_RVALUET
