// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_FUNCTIONSFOR
#define QOR_PP_H_REFLECTION_FUNCTIONSFOR

#pragma once

#include "detail/config.h"
#include "ops_fields.h"
#include "io_fields.h"

// Contains qor_pp_refl_functions_for macro that defined comparison and stream operators for T along with hash_value function.

//     #include <pfr/functions_for.hpp>
//
//     namespace my_namespace {
//         struct my_struct {      // No operators defined for that structure
//             int i; short s; char data[7]; bool bl; int a,b,c,d,e,f;
//         };
//         qor_pp_refl_functions_for(my_struct)
//     }


// qor_pp_refl_functions_for(T)
// Defines comparison and stream operators for T along with hash_value function.
//
//     #include <pfr/functions_for.hpp>
//     struct comparable_struct {      // No operators defined for that structure
//         int i; short s; char data[7]; bool bl; int a,b,c,d,e,f;
//     };
//     qor_pp_refl_functions_for(comparable_struct)
//     // ...
//
//     comparable_struct s1 {0, 1, "Hello", false, 6,7,8,9,10,11};
//     comparable_struct s2 {0, 1, "Hello", false, 6,7,8,9,10,11111};
//     assert(s1 < s2);
//     std::cout << s1 << std::endl; // Outputs: {0, 1, H, e, l, l, o, , , 0, 6, 7, 8, 9, 10, 11}
//
//
// Defines following for T:
//
// bool operator==(const T& lhs, const T& rhs);
// bool operator!=(const T& lhs, const T& rhs);
// bool operator< (const T& lhs, const T& rhs);
// bool operator> (const T& lhs, const T& rhs);
// bool operator<=(const T& lhs, const T& rhs);
// bool operator>=(const T& lhs, const T& rhs);
//
// template <class Char, class Traits>
// std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& out, const T& value);
//
// template <class Char, class Traits>
// std::basic_istream<Char, Traits>& operator>>(std::basic_istream<Char, Traits>& in, T& value);
//
// // helper function for Boost unordered containers and boost::hash<>.
// std::size_t hash_value(const T& value);

#define qor_pp_refl_functions_for(T)                                                                                                            \
    qor_pp_refl_maybe_unused inline bool operator==(const T& lhs, const T& rhs) { return ::qor_reflection::eq_fields(lhs, rhs); }               \
    qor_pp_refl_maybe_unused inline bool operator!=(const T& lhs, const T& rhs) { return ::qor_reflection::ne_fields(lhs, rhs); }               \
    qor_pp_refl_maybe_unused inline bool operator< (const T& lhs, const T& rhs) { return ::qor_reflection::lt_fields(lhs, rhs); }               \
    qor_pp_refl_maybe_unused inline bool operator> (const T& lhs, const T& rhs) { return ::qor_reflection::gt_fields(lhs, rhs); }               \
    qor_pp_refl_maybe_unused inline bool operator<=(const T& lhs, const T& rhs) { return ::qor_reflection::le_fields(lhs, rhs); }               \
    qor_pp_refl_maybe_unused inline bool operator>=(const T& lhs, const T& rhs) { return ::qor_reflection::ge_fields(lhs, rhs); }               \
    template <class Char, class Traits>                                                                                                         \
    qor_pp_refl_maybe_unused inline ::std::basic_ostream<Char, Traits>& operator<<(::std::basic_ostream<Char, Traits>& out, const T& value) {   \
        return out << ::qor_reflection::io_fields(value);                                                                                       \
    }                                                                                                                                           \
    template <class Char, class Traits>                                                                                                         \
    qor_pp_refl_maybe_unused inline ::std::basic_istream<Char, Traits>& operator>>(::std::basic_istream<Char, Traits>& in, T& value) {          \
        return in >> ::qor_reflection::io_fields(value);                                                                                        \
    }                                                                                                                                           \
    qor_pp_refl_maybe_unused inline std::size_t hash_value(const T& v) {                                                                        \
        return ::qor_reflection::hash_fields(v);                                                                                                \
    }                                                                                                                                           \
/**/

#endif//QOR_PP_H_REFLECTION_FUNCTIONSFOR