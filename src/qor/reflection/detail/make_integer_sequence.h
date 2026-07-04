// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_DETAIL_MAKEINTEGERSEQUENCE
#define QOR_PP_H_REFLECTION_DETAIL_MAKEINTEGERSEQUENCE

#pragma once

#include "config.h"

#ifdef qor_pp_refl_has_std_module
import std;
#else
#include <type_traits>
#include <utility>
#include <cstddef>
#endif

namespace qor_reflection { namespace detail {

#if qor_pp_refl_use_std_make_integral_sequence == 0

#ifdef __has_builtin
#   if __has_builtin(__make_integer_seq)
#       define qor_pp_refl_use_make_int_seq_builtin
#   endif
#endif

#ifdef qor_pp_refl_use_make_int_seq_builtin

    using std::integer_sequence;

    // Clang unable to use namespace qualified std::integer_sequence in __make_integer_seq.
    template <typename T, T N>
    using make_integer_sequence = __make_integer_seq<integer_sequence, T, N>;

#   undef qor_pp_refl_use_make_int_seq_builtin

#else

    template <typename T, typename U>
    struct join_sequences;

    template <typename T, T... A, T... B>
    struct join_sequences<std::integer_sequence<T, A...>, std::integer_sequence<T, B...>> 
    {
        using type = std::integer_sequence<T, A..., B...>;
    };

    template <typename T, T Min, T Max>
    struct build_sequence_impl 
    {
        static_assert(Min < Max, "Start of range must be less than its end");
        static constexpr T size = Max - Min;
        using type = typename join_sequences<
                typename build_sequence_impl<T, Min, Min + size / 2>::type,
                typename build_sequence_impl<T, Min + size / 2 + 1, Max>::type
            >::type;
    };

    template <typename T, T V>
    struct build_sequence_impl<T, V, V> 
    {
        using type = std::integer_sequence<T, V>;
    };

    template <typename T, std::size_t N>
    struct make_integer_sequence_impl : build_sequence_impl<T, 0, N - 1> {};

    template <typename T>
    struct make_integer_sequence_impl<T, 0> 
    {
        using type = std::integer_sequence<T>;
    };

    template <typename T, T N>
    using make_integer_sequence = typename make_integer_sequence_impl<T, N>::type;

#endif // !defined qor_pp_refl_use_make_int_seq_builtin
    
#else // qor_pp_refl_use_std_make_integral_sequence == 1

    template <typename T, T N>
    using make_integer_sequence = std::make_integer_sequence<T, N>;

#endif // qor_pp_refl_use_std_make_integral_sequence == 1

    template <std::size_t N>
    using make_index_sequence = make_integer_sequence<std::size_t, N>;

    template <typename... T>
    using index_sequence_for = make_index_sequence<sizeof...(T)>;

}}//qor_reflection::detail

#endif//QOR_PP_H_REFLECTION_DETAIL_MAKEINTEGERSEQUENCE