// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_OPSFIELDS
#define QOR_PP_H_REFLECTION_OPSFIELDS

#pragma once

#include "detail/config.h"
#include "core.h"
#include "detail/functional.h"

// Contains field-by-fields comparison and hash functions.
//
//     #include <pfr/ops_fields.hpp>
//     struct comparable_struct {      // No operators defined for that structure
//         int i; short s;
//     };
//     // ...
//
//     comparable_struct s1 {0, 1};
//     comparable_struct s2 {0, 2};
//     assert(qor_reflection::lt_fields(s1, s2));
//
//  ops for other ways to define operators and more details.
//

namespace qor_reflection {

    qor_pp_refl_begin_module_export

    // Does a field-by-field equality comparison.
    //
    // returns `L == R && tuple_size_v<T> == tuple_size_v<U>`, where `L` and
    // `R` are the results of calling `std::tie` on first `N` fields of `lhs` and
    // `rhs` respectively; `N` is `std::min(tuple_size_v<T>, tuple_size_v<U>)`.
    template <class T, class U>
    constexpr bool eq_fields(const T& lhs, const U& rhs) noexcept 
    {
        return detail::binary_visit<detail::equal_impl>(lhs, rhs);
    }

    // Does a field-by-field inequality comparison.
    //
    // returns `L != R || tuple_size_v<T> != tuple_size_v<U>`, where `L` and
    // `R` are the results of calling `std::tie` on first `N` fields of `lhs` and
    // `rhs` respectively; `N` is `std::min(tuple_size_v<T>, tuple_size_v<U>)`.
    template <class T, class U>
    constexpr bool ne_fields(const T& lhs, const U& rhs) noexcept 
    {
        return detail::binary_visit<detail::not_equal_impl>(lhs, rhs);
    }

    // Does a field-by-field greter comparison.
    //
    // returns `L > R || (L == R && tuple_size_v<T> > tuple_size_v<U>)`, where `L` and
    // `R` are the results of calling `std::tie` on first `N` fields of `lhs` and
    // `rhs` respectively; `N` is `std::min(tuple_size_v<T>, tuple_size_v<U>)`.
    template <class T, class U>
    constexpr bool gt_fields(const T& lhs, const U& rhs) noexcept 
    {
        return detail::binary_visit<detail::greater_impl>(lhs, rhs);
    }


    // Does a field-by-field less comparison.
    //
    // returns `L < R || (L == R && tuple_size_v<T> < tuple_size_v<U>)`, where `L` and
    // `R` are the results of calling `std::tie` on first `N` fields of `lhs` and
    // `rhs` respectively; `N` is `std::min(tuple_size_v<T>, tuple_size_v<U>)`.
    template <class T, class U>
    constexpr bool lt_fields(const T& lhs, const U& rhs) noexcept 
    {
        return detail::binary_visit<detail::less_impl>(lhs, rhs);
    }


    // Does a field-by-field greater equal comparison.
    //
    // returns `L > R || (L == R && tuple_size_v<T> >= tuple_size_v<U>)`, where `L` and
    // `R` are the results of calling `std::tie` on first `N` fields of `lhs` and
    // `rhs` respectively; `N` is `std::min(tuple_size_v<T>, tuple_size_v<U>)`.
    template <class T, class U>
    constexpr bool ge_fields(const T& lhs, const U& rhs) noexcept 
    {
        return detail::binary_visit<detail::greater_equal_impl>(lhs, rhs);
    }


    // Does a field-by-field less equal comparison.
    //
    // returns `L < R || (L == R && tuple_size_v<T> <= tuple_size_v<U>)`, where `L` and
    // `R` are the results of calling `std::tie` on first `N` fields of `lhs` and
    // `rhs` respectively; `N` is `std::min(tuple_size_v<T>, tuple_size_v<U>)`.
    template <class T, class U>
    constexpr bool le_fields(const T& lhs, const U& rhs) noexcept 
    {
        return detail::binary_visit<detail::less_equal_impl>(lhs, rhs);
    }


    // Does a field-by-field hashing.
    //
    // returns combined hash of all the fields
    template <class T>
    std::size_t hash_fields(const T& x) 
    {
        constexpr std::size_t fields_count_val = qor_reflection::detail::fields_count<std::remove_reference_t<T>>();
#if qor_pp_refl_use_cpp17 || qor_pp_refl_use_loophole
        return detail::hash_impl<0, fields_count_val>::compute(detail::tie_as_tuple(x));
#else
        std::size_t result = 0;
        ::qor_reflection::detail::for_each_field_dispatcher(
            x,
            [&result](const auto& lhs) {
                // We can not reuse `fields_count_val` in lambda because compilers had issues with
                // passing constexpr variables into lambdas. Computing is again is the most portable solution.
                constexpr std::size_t fields_count_val_lambda = qor_reflection::detail::fields_count<std::remove_reference_t<T>>();
                result = detail::hash_impl<0, fields_count_val_lambda>::compute(lhs);
            },
            detail::make_index_sequence<fields_count_val>{}
        );

        return result;
#endif
    }

    qor_pp_refl_end_module_export

}//qor_reflection

#endif//QOR_PP_H_REFLECTION_OPSFIELDS
