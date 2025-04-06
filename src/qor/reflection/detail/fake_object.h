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

#ifndef QOR_PP_H_REFLECTION_DETAIL_FAKE_OBJECT
#define QOR_PP_H_REFLECTION_DETAIL_FAKE_OBJECT

#pragma once

#include "config.h"

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wundefined-internal"
#   pragma clang diagnostic ignored "-Wundefined-var-template"
#endif

namespace pfr { namespace detail {

// This class has external linkage while T has not sure.
template <class T>
struct wrapper {
    const T value;
};

// This variable servers as a link-time assert.
// If linker requires it, then `fake_object()` is used at runtime.
template <class T>
extern const wrapper<T> do_not_use_PFR_with_local_types;

// For returning non default constructible types, it's exclusively used in member name retrieval.
//
// Neither std::declval nor pfr::detail::unsafe_declval are usable there.
// This takes advantage of C++20 features, while pfr::detail::unsafe_declval works
// with the former standards.
template <class T>
constexpr const T& fake_object() noexcept {
    return do_not_use_PFR_with_local_types<T>.value;
}

}} // namespace pfr::detail

#ifdef __clang__
#   pragma clang diagnostic pop
#endif


#endif//QOR_PP_H_REFLECTION_DETAIL_FAKE_OBJECT