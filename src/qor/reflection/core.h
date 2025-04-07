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

#ifndef QOR_PP_H_REFLECTION_CORE
#define QOR_PP_H_REFLECTION_CORE

#pragma once

#include "detail/config.h"
#include "detail/core.h"
#include "detail/sequence_tuple.h"
#include "detail/stdtuple.h"
#include "detail/for_each_field.h"
#include "detail/make_integer_sequence.h"
#include "detail/tie_from_structure_tuple.h"

#include <type_traits>
#include <utility>      // metaprogramming stuff

#include "tuple_size.h"

// Contains all the basic tuple-like interfaces get, tuple_size, tuple_element_t, and others.

namespace qor_reflection {

    qor_pp_refl_begin_module_export

    // Returns reference or const reference to a field with index `I` in aggregate `val`.
    // Overload taking the type `U` returns reference or const reference to a field
    // with provided type `U` in aggregate `val` if there's only one field of such type in `val`.
    //
    //     struct my_struct { int i, short s; };
    //     my_struct s {10, 11};
    //
    //     assert(qor_reflection::get<0>(s) == 10);
    //     qor_reflection::get<1>(s) = 0;
    //
    //     assert(qor_reflection::get<int>(s) == 10);
    //     qor_reflection::get<short>(s) = 11;

    template <std::size_t I, class T>
    constexpr decltype(auto) get(const T& val) noexcept 
    {
        return detail::sequence_tuple::get<I>( detail::tie_as_tuple(val) );
    }

    template <std::size_t I, class T>
    constexpr decltype(auto) get(T& val
#if !qor_pp_refl_use_cpp17
        , std::enable_if_t<std::is_assignable<T, T>::value>* = nullptr
#endif
    ) noexcept 
    {
        return detail::sequence_tuple::get<I>( detail::tie_as_tuple(val) );
    }

#if !qor_pp_refl_use_cpp17
    template <std::size_t I, class T>
    constexpr auto get(T&, std::enable_if_t<!std::is_assignable<T, T>::value>* = nullptr) noexcept 
    {
        static_assert(sizeof(T) && false, "====================> QOR Reflection: Calling qor_reflection::get on non const non assignable type is allowed only in C++17");
        return 0;
    }
#endif


    template <std::size_t I, class T>
    constexpr auto get(T&& val, std::enable_if_t< std::is_rvalue_reference<T&&>::value>* = nullptr) noexcept 
    {
        return std::move(detail::sequence_tuple::get<I>( detail::tie_as_tuple(val) ));
    }


    template <class U, class T>
    constexpr const U& get(const T& val) noexcept 
    {
        return detail::sequence_tuple::get_by_type_impl<const U&>( detail::tie_as_tuple(val) );
    }


    template <class U, class T>
    constexpr U& get(T& val
#if !qor_pp_refl_use_cpp17
        , std::enable_if_t<std::is_assignable<T, T>::value>* = nullptr
#endif
    ) noexcept 
    {
        return detail::sequence_tuple::get_by_type_impl<U&>( detail::tie_as_tuple(val) );
    }

#if !qor_pp_refl_use_cpp17

    template <class U, class T>
    constexpr U& get(T&, std::enable_if_t<!std::is_assignable<T, T>::value>* = nullptr) noexcept 
    {
        static_assert(sizeof(T) && false, "====================> QOR Reflection: Calling qor_reflection::get on non const non assignable type is allowed only in C++17");
        return 0;
    }
#endif


    template <class U, class T>
    constexpr U&& get(T&& val, std::enable_if_t< std::is_rvalue_reference<T&&>::value>* = nullptr) noexcept 
    {
        return std::move(detail::sequence_tuple::get_by_type_impl<U&>( detail::tie_as_tuple(val) ));
    }


    // `tuple_element` has a member typedef `type` that returns the type of a field with index I in aggregate T.
    //
    //     std::vector< qor_reflection::tuple_element<0, my_structure>::type > v;

    template <std::size_t I, class T>
    using tuple_element = detail::sequence_tuple::tuple_element<I, decltype( ::qor_reflection::detail::tie_as_tuple(std::declval<T&>()) ) >;


    // Type of a field with index `I` in aggregate `T`.
    //
    //     std::vector< qor_reflection::tuple_element_t<0, my_structure> > v;

    template <std::size_t I, class T>
    using tuple_element_t = typename tuple_element<I, T>::type;

    // Creates a `std::tuple` from fields of an aggregate `val`.
    //
    //     struct my_struct { int i, short s; };
    //     my_struct s {10, 11};
    //     std::tuple<int, short> t = qor_reflection::structure_to_tuple(s);
    //     assert(get<0>(t) == 10);

    template <class T>
    constexpr auto structure_to_tuple(const T& val) 
    {
        return detail::make_stdtuple_from_tietuple(
            detail::tie_as_tuple(val),
            detail::make_index_sequence< tuple_size_v<T> >()
        );
    }

    // std::tie` like function that ties fields of a structure.
    //
    // returns a `std::tuple` with lvalue and const lvalue references to fields of an aggregate `val`.
    //
    //     void foo(const int&, const short&);
    //     struct my_struct { int i, short s; };
    //
    //     const my_struct const_s{1, 2};
    //     std::apply(foo, qor_reflection::structure_tie(const_s));
    //
    //     my_struct s;
    //     qor_reflection::structure_tie(s) = std::tuple<int, short>{10, 11};
    //     assert(s.s == 11);

    template <class T>
    constexpr auto structure_tie(const T& val) noexcept 
    {
        return detail::make_conststdtiedtuple_from_tietuple(
            detail::tie_as_tuple(const_cast<T&>(val)),
            detail::make_index_sequence< tuple_size_v<T> >()
        );
    }


    template <class T>
    constexpr auto structure_tie(T& val
#if !qor_pp_refl_use_cpp17
        , std::enable_if_t<std::is_assignable<T, T>::value>* = nullptr
#endif
    ) noexcept 
    {
        return detail::make_stdtiedtuple_from_tietuple(
            detail::tie_as_tuple(val),
            detail::make_index_sequence< tuple_size_v<T> >()
        );
    }

#if !qor_pp_refl_use_cpp17

    template <class T>
    constexpr auto structure_tie(T&, std::enable_if_t<!std::is_assignable<T, T>::value>* = nullptr) noexcept 
    {
        static_assert(sizeof(T) && false, "====================> QOR Reflection: Calling qor_reflection::structure_tie on non const non assignable type is allowed only in C++17");
        return 0;
    }
#endif


    template <class T>
    constexpr auto structure_tie(T&&, std::enable_if_t< std::is_rvalue_reference<T&&>::value>* = nullptr) noexcept 
    {
        static_assert(sizeof(T) && false, "====================> QOR Reflection: Calling qor_reflection::structure_tie on rvalue references is forbidden");
        return 0;
    }

    // Calls `func` for each field of a `value`.
    //
    // func must have one of the following signatures:
    //     * any_return_type func(U&& field)                // field of value is perfect forwarded to function
    //     * any_return_type func(U&& field, std::size_t i)
    //     * any_return_type func(U&& value, I i)           // Here I is an `std::integral_constant<size_t, field_index>`
    //
    // value To each field of this variable will be the `func` applied.
    //
    //     struct my_struct { int i, short s; };
    //     int sum = 0;
    //     qor_reflection::for_each_field(my_struct{20, 22}, [&sum](const auto& field) { sum += field; });
    //     assert(sum == 42);

    template <class T, class F>
    constexpr void for_each_field(T&& value, F&& func) 
    {
        return ::qor_reflection::detail::for_each_field(std::forward<T>(value), std::forward<F>(func));
    }

    // std::tie-like function that allows assigning to tied values from aggregates.
    //
    // returns an object with lvalue references to `args...`; on assignment of an aggregate value to that
    // object each field of an aggregate is assigned to the corresponding `args...` reference.
    //
    //     auto f() {
    //       struct { struct { int x, y } p; short s; } res { { 4, 5 }, 6 };
    //       return res;
    //     }
    //     auto [p, s] = f();
    //     qor_reflection::tie_from_structure(p, s) = f();

    template <typename... Elements>
    constexpr detail::tie_from_structure_tuple<Elements...> tie_from_structure(Elements&... args) noexcept 
    {
        return detail::tie_from_structure_tuple<Elements...>(args...);
    }

    qor_pp_refl_end_module_export

} // namespace qor_reflection

#endif//QOR_PP_H_REFLECTION_CORE
