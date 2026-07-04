// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_TUPLESIZE
#define QOR_PP_H_REFLECTION_TUPLESIZE

#pragma once

#include "detail/config.h"

#include <type_traits>
#include <utility>      // metaprogramming stuff
#include "detail/sequence_tuple.h"
#include "detail/fields_count.h"

// Contains tuple-like interfaces to get fields count

namespace qor_reflection {

    qor_pp_refl_begin_module_export

    // Has a static const member variable `value` that contains fields count in a T.
    // Works for any T that satisfies \aggregate.
    //     std::array<int, qor_reflection::tuple_size<my_structure>::value > a;
    template <class T>
    using tuple_size = detail::size_t_< qor_reflection::detail::fields_count<T>() >;


    // `tuple_size_v` is a template variable that contains fields count in a T and
    // works for any T that satisfies \aggregate.
    //     std::array<int, qor_reflection::tuple_size_v<my_structure> > a;
    template <class T>
    constexpr std::size_t tuple_size_v = tuple_size<T>::value;

    qor_pp_refl_end_module_export

}//qor_reflection

#endif//QOR_PP_H_REFLECTION_TUPLESIZE