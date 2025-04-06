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

#ifndef QOR_PP_H_REFLECTION_DETAIL_CORE14_LOOPHOLE
#define QOR_PP_H_REFLECTION_DETAIL_CORE14_LOOPHOLE

#pragma once

#include "config.h"

#ifdef PFR_HAS_STD_MODULE
import std;
#else
#include <type_traits>
#include <utility>
#endif

#include "offset_based_getter.h"
#include "fields_count.h"
#include "make_flat_tuple_of_references.h"
#include "make_integer_sequence.h"
#include "sequence_tuple.h"
#include "rvalue_t.h"
#include "unsafe_declval.h"


#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wmissing-braces"
#   pragma clang diagnostic ignored "-Wundefined-inline"
#   pragma clang diagnostic ignored "-Wundefined-internal"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#elif defined(__GNUC__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wnon-template-friend"
#endif


namespace qor_reflection { namespace detail {

// tag<T,N> generates friend declarations and helps with overload resolution.
// There are two types: one with the auto return type, which is the way we read types later.
// The second one is used in the detection of instantiations without which we'd get multiple
// definitions.

template <class T, std::size_t N>
struct tag {
    friend auto loophole(tag<T,N>);
};

// The definitions of friend functions.
template <class T, class U, std::size_t N, bool B>
struct fn_def_lref {
    friend auto loophole(tag<T,N>) {
        // Standard Library containers do not SFINAE on invalid copy constructor. Because of that std::vector<std::unique_ptr<int>> reports that it is copyable,
        // which leads to an instantiation error at this place.
        //
        // To workaround the issue, we check that the type U is movable, and move it in that case.
        using no_extents_t = std::remove_all_extents_t<U>;
        return static_cast< std::conditional_t<std::is_move_constructible<no_extents_t>::value, no_extents_t&&, no_extents_t&> >(
            qor_reflection::detail::unsafe_declval<no_extents_t&>()
        );
    }
};
template <class T, class U, std::size_t N, bool B>
struct fn_def_rref {
    friend auto loophole(tag<T,N>) { return std::move(qor_reflection::detail::unsafe_declval< std::remove_all_extents_t<U>& >()); }
};


// Those specializations are to avoid multiple definition errors.
template <class T, class U, std::size_t N>
struct fn_def_lref<T, U, N, true> {};

template <class T, class U, std::size_t N>
struct fn_def_rref<T, U, N, true> {};


// This has a templated conversion operator which in turn triggers instantiations.
// Important point, using sizeof seems to be more reliable. Also default template
// arguments are "cached" (I think). To fix that I provide a U template parameter to
// the ins functions which do the detection using constexpr friend functions and SFINAE.
template <class T, std::size_t N>
struct loophole_ubiq_lref {
    template<class U, std::size_t M> static std::size_t ins(...);
    template<class U, std::size_t M, std::size_t = sizeof(loophole(tag<T,M>{})) > static char ins(int);

    template<class U, std::size_t = sizeof(fn_def_lref<T, U, N, sizeof(ins<U, N>(0)) == sizeof(char)>)>
    constexpr operator U&() const&& noexcept; // `const&&` here helps to avoid ambiguity in loophole instantiations. optional_like test validate that behavior.
};

template <class T, std::size_t N>
struct loophole_ubiq_rref {
    template<class U, std::size_t M> static std::size_t ins(...);
    template<class U, std::size_t M, std::size_t = sizeof(loophole(tag<T,M>{})) > static char ins(int);

    template<class U, std::size_t = sizeof(fn_def_rref<T, U, N, sizeof(ins<U, N>(0)) == sizeof(char)>)>
    constexpr operator U&&() const&& noexcept; // `const&&` here helps to avoid ambiguity in loophole instantiations. optional_like test validate that behavior.
};


// This is a helper to turn a data structure into a tuple.
template <class T, class U>
struct loophole_type_list_lref;

template <typename T, std::size_t... I>
struct loophole_type_list_lref< T, std::index_sequence<I...> >
     // Instantiating loopholes:
    : sequence_tuple::tuple< decltype(T{ loophole_ubiq_lref<T, I>{}... }, 0) >
{
    using type = sequence_tuple::tuple< decltype(loophole(tag<T, I>{}))... >;
};


template <class T, class U>
struct loophole_type_list_rref;

template <typename T, std::size_t... I>
struct loophole_type_list_rref< T, std::index_sequence<I...> >
     // Instantiating loopholes:
    : sequence_tuple::tuple< decltype(T{ loophole_ubiq_rref<T, I>{}... }, 0) >
{
    using type = sequence_tuple::tuple< decltype(loophole(tag<T, I>{}))... >;
};


// Lazily returns loophole_type_list_{lr}ref.
template <bool IsCopyConstructible /*= true*/, class T, class U>
struct loophole_type_list_selector {
    using type = loophole_type_list_lref<T, U>;
};

template <class T, class U>
struct loophole_type_list_selector<false /*IsCopyConstructible*/, T, U> {
    using type = loophole_type_list_rref<T, U>;
};

template <class T>
auto tie_as_tuple_loophole_impl(T& lvalue) noexcept {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
    using indexes = detail::make_index_sequence<fields_count<type>()>;
    using loophole_type_list = typename detail::loophole_type_list_selector<
        std::is_copy_constructible<std::remove_all_extents_t<type>>::value, type, indexes
    >::type;
    using tuple_type = typename loophole_type_list::type;

    return qor_reflection::detail::make_flat_tuple_of_references(
        lvalue,
        offset_based_getter<type, tuple_type>{},
        size_t_<0>{},
        size_t_<tuple_type::size_v>{}
    );
}

template <class T>
auto tie_as_tuple(T& val) noexcept {
    static_assert(
        !std::is_union<T>::value,
        "====================> Boost.PFR: For safety reasons it is forbidden to reflect unions. See `Reflection of unions` section in the docs for more info."
    );
    return qor_reflection::detail::tie_as_tuple_loophole_impl(
        val
    );
}

template <class T, class F, std::size_t... I>
void for_each_field_dispatcher(T& t, F&& f, std::index_sequence<I...>) {
    static_assert(
        !std::is_union<T>::value,
        "====================> Boost.PFR: For safety reasons it is forbidden to reflect unions. See `Reflection of unions` section in the docs for more info."
    );
    std::forward<F>(f)(
        qor_reflection::detail::tie_as_tuple_loophole_impl(t)
    );
}

}} // namespace qor_reflection::detail


#ifdef __clang__
#   pragma clang diagnostic pop
#elif defined(__GNUC__)
#   pragma GCC diagnostic pop
#endif


#endif//QOR_PP_H_REFLECTION_DETAIL_CORE14_LOOPHOLE