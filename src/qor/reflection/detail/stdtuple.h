// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

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
