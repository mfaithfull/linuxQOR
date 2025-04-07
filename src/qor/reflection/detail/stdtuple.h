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

#ifndef QOR_PP_H_REFLECTION_DETAIL_STDTUPLE
#define QOR_PP_H_REFLECTION_DETAIL_STDTUPLE

#pragma once

#include "config.h"

#ifdef qor_pp_refl_has_std_module
import std;
#else
#include <utility>      // metaprogramming stuff
#include <tuple>
#endif

#include "sequence_tuple.h"

namespace qor_reflection { namespace detail {

    template <class T, std::size_t... I>
    constexpr auto make_stdtuple_from_tietuple(const T& t, std::index_sequence<I...>) 
    {
        (void)t;  // workaround for MSVC 14.1 `warning C4100: 't': unreferenced formal parameter`
        return std::make_tuple(qor_reflection::detail::sequence_tuple::get<I>(t)... );
    }

    template <class T, std::size_t... I>
    constexpr auto make_stdtiedtuple_from_tietuple(const T& t, std::index_sequence<I...>) noexcept 
    {
        (void)t;  // workaround for MSVC 14.1 `warning C4100: 't': unreferenced formal parameter`
        return std::tie(qor_reflection::detail::sequence_tuple::get<I>(t)... );
    }

    template <class T, std::size_t... I>
    constexpr auto make_conststdtiedtuple_from_tietuple(const T& t, std::index_sequence<I...>) noexcept 
    {
        (void)t;  // workaround for MSVC 14.1 `warning C4100: 't': unreferenced formal parameter`
        return std::tuple<
            std::add_lvalue_reference_t<std::add_const_t<
                std::remove_reference_t<decltype(qor_reflection::detail::sequence_tuple::get<I>(t))>
            >>...
        >(qor_reflection::detail::sequence_tuple::get<I>(t)... );
    }

}} // namespace qor_reflection::detail

#endif//QOR_PP_H_REFLECTION_DETAIL_STDTUPLE
