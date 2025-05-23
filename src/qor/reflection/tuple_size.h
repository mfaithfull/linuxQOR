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