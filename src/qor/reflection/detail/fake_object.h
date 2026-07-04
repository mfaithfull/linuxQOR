// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_DETAIL_FAKE_OBJECT
#define QOR_PP_H_REFLECTION_DETAIL_FAKE_OBJECT

#pragma once

#include "config.h"

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wundefined-internal"
#   pragma clang diagnostic ignored "-Wundefined-var-template"
#endif

namespace qor_reflection { namespace detail {

    // This class has external linkage while T has not sure.
    template <class T>
    struct wrapper 
    {
        const T value;
    };

    // This variable serves as a link-time assert.
    // If linker requires it, then `fake_object()` is used at runtime.
    template <class T>
    extern const wrapper<T> do_not_use_qor_reflection_with_local_types;

    // For returning non default constructible types, it's exclusively used in member name retrieval.
    //
    // Neither std::declval nor qor_reflection::detail::unsafe_declval are usable there.
    // This takes advantage of C++20 features, while qor_reflection::detail::unsafe_declval works
    // with the former standards.
    template <class T>
    constexpr const T& fake_object() noexcept 
    {
        return do_not_use_qor_reflection_with_local_types<T>.value;
    }

}}//qor_reflection::detail

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif//QOR_PP_H_REFLECTION_DETAIL_FAKE_OBJECT
