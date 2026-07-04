// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_FUNCTORS
#define QOR_PP_H_REFLECTION_FUNCTORS

#pragma once

#include "detail/config.h"
#include "ops.h"
#include "detail/functional.h"

// Contains functors that are close to the Standard Library ones.
// Each functor calls corresponding QOR Reflection function from ops.h
//
//     #include <pfr/functors.hpp>
//     struct my_struct {      // No operators defined for that structure
//         int i; short s; char data[7]; bool bl; int a,b,c,d,e,f;
//     };
//     // ...
//
//     std::unordered_set<
//         my_struct,
//         qor_reflection::hash<>,
//         qor_reflection::equal_to<>
//     > my_set;

namespace qor_reflection {

    qor_pp_refl_begin_module_export

    ///////////////////// Comparisons

    // std::equal_to like comparator that returns eq(x, y)
    template <class T = void> struct equal_to 
    {
        // return true if each field of x equals the field with same index of y.
        bool operator()(const T& x, const T& y) const 
        {
            return qor_reflection::eq(x, y);
        }
    };

    template <> struct equal_to<void> 
    {
        template <class T, class U>
        bool operator()(const T& x, const U& y) const 
        {
            return qor_reflection::eq(x, y);
        }

        typedef std::true_type is_transparent;
    };


    // std::not_equal like comparator that returns ne(x, y)
    template <class T = void> struct not_equal 
    {
        // return true if at least one field x not equals the field with same index of y.
        bool operator()(const T& x, const T& y) const 
        {
            return qor_reflection::ne(x, y);
        }
    };

    template <> struct not_equal<void> 
    {
        template <class T, class U>
        bool operator()(const T& x, const U& y) const 
        {
            return qor_reflection::ne(x, y);
        }

        typedef std::true_type is_transparent;
    };

    // std::greater like comparator that returns gt(x, y)
    template <class T = void> struct greater 
    {
        // return true if field of x greater than the field with same index of y and all previous fields of x equal to the same fields of y.
        bool operator()(const T& x, const T& y) const 
        {
            return qor_reflection::gt(x, y);
        }
    };

    template <> struct greater<void> 
    {
        template <class T, class U>
        bool operator()(const T& x, const U& y) const 
        {
            return qor_reflection::gt(x, y);
        }

        typedef std::true_type is_transparent;
    };

    // std::less like comparator that returns lt(x, y)
    template <class T = void> struct less 
    {
        // return true if field of x less than the field with same index of y and all previous fields of x equal to the same fields of y.
        bool operator()(const T& x, const T& y) const 
        {
            return qor_reflection::lt(x, y);
        }

    #ifdef qor_pp_doxygen_invoked
        /// This typedef exists only if T \b is void
        typedef std::true_type is_transparent;

        /// This operator allows comparison of \b x and \b y that have different type.
        /// \pre Exists only if T \b is void.
        template <class V, class U> bool operator()(const V& x, const U& y) const;
    #endif
    };


    template <> struct less<void> 
    {
        template <class T, class U>
        bool operator()(const T& x, const U& y) const 
        {
            return qor_reflection::lt(x, y);
        }
        typedef std::true_type is_transparent;
    };


    // std::greater_equal like comparator that returns ge(x, y)
    template <class T = void> struct greater_equal 
    {
        // return true if field of x greater than the field with same index of y and all previous fields of x equal to the same fields of y;
        // or if each field of x equals the field with same index of y.
        bool operator()(const T& x, const T& y) const 
        {
            return qor_reflection::ge(x, y);
        }
    };

    template <> struct greater_equal<void> 
    {
        template <class T, class U>
        bool operator()(const T& x, const U& y) const 
        {
            return qor_reflection::ge(x, y);
        }
        typedef std::true_type is_transparent;
    };


    // std::less_equal like comparator that returns le(x, y)
    template <class T = void> struct less_equal 
    {
        // return true if field of x less than the field with same index of y and all previous fields of x equal to the same fields of y;
        // or if each field of x equals the field with same index of y.
        bool operator()(const T& x, const T& y) const 
        {
            return qor_reflection::le(x, y);
        }
    };

    template <> struct less_equal<void> 
    {
        template <class T, class U>
        bool operator()(const T& x, const U& y) const 
        {
            return qor_reflection::le(x, y);
        }
        typedef std::true_type is_transparent;
    };

    // std::hash like functor that returns hash_value(x)
    template <class T> struct hash 
    {
        // return hash value of x.
        std::size_t operator()(const T& x) const 
        {
            return qor_reflection::hash_value(x);
        }
    };

    qor_pp_refl_end_module_export

}//qor_reflection

#endif//QOR_PP_H_REFLECTION_FUNCTORS
