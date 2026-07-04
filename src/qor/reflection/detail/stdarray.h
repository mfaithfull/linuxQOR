// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_DETAIL_STDARRAY
#define QOR_PP_H_REFLECTION_DETAIL_STDARRAY

#pragma once

#include "config.h"

#ifdef qor_pp_refl_has_std_module
import std;
#else
#include <utility> // metaprogramming stuff
#include <array>
#include <type_traits> // for std::common_type_t
#include <cstddef>
#endif

#include "sequence_tuple.h"

namespace qor_reflection { namespace detail {

    template <class... Types>
    constexpr auto make_stdarray(const Types&... t) noexcept 
    {
        return std::array<std::common_type_t<Types...>, sizeof...(Types)>{t...};
    }

    template <class T, std::size_t... I>
    constexpr auto make_stdarray_from_tietuple(const T& t, std::index_sequence<I...>, int) noexcept 
    {
        return detail::make_stdarray( qor_reflection::detail::sequence_tuple::get<I>(t)... );
    }

    template <class T>
    constexpr auto make_stdarray_from_tietuple(const T&, std::index_sequence<>, long) noexcept 
    {
        return std::array<std::nullptr_t, 0>{};
    }

}} // namespace qor_reflection::detail

#endif//QOR_PP_H_REFLECTION_DETAIL_STDARRAY
