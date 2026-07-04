// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_DETAIL_CORE17
#define QOR_PP_H_REFLECTION_DETAIL_CORE17

#pragma once

#include "core17_generated.h"
#include "fields_count.h"
#include "for_each_field_impl.h"
#include "rvalue_t.h"

namespace qor_reflection { namespace detail {

#ifndef _MSC_VER // MSVC fails to compile the following code, but compiles the structured bindings in core17_generated.hpp

    struct do_not_define_std_tuple_size_for_me 
    {
        bool test1 = true;
    };

    template <class T>
    constexpr bool do_structured_bindings_work() noexcept 
    { // ******************************************* IN CASE OF ERROR READ THE FOLLOWING LINES IN detail/core17.hpp FILE:
        T val{};
        auto& [a] = val; // ******************************************* IN CASE OF ERROR READ THE FOLLOWING LINES IN pfr/detail/core17.hpp FILE:

        /****************************************************************************
        *
        * It looks like your compiler or Standard Library can not handle C++17
        * structured bindings.
        *
        * Workaround: Define qor_pp_refl_use_cpp17 to 0
        * It will disable the C++17 features for QOR Reflection library.
        *
        * Sorry for the inconvenience caused.
        *
        ****************************************************************************/

        return a;
    }

    static_assert(
        do_structured_bindings_work<do_not_define_std_tuple_size_for_me>(),
        "====================> QOR Reflection: Your compiler can not handle C++17 structured bindings. Read the above comments for workarounds."
    );
#endif // #ifndef _MSC_VER

    template <class T>
    constexpr auto tie_as_tuple(T& val) noexcept 
    {
        static_assert(
            !std::is_union<T>::value,
            "====================> QOR Reflection: For safety reasons it is forbidden to reflect unions. See `Reflection of unions` section in the docs for more info."
        );
        typedef size_t_<qor_reflection::detail::fields_count<T>()> fields_count_tag;
        return qor_reflection::detail::tie_as_tuple(val, fields_count_tag{});
    }

    template <class T, class F, std::size_t... I>
    constexpr void for_each_field_dispatcher(T& t, F&& f, std::index_sequence<I...>) 
    {
        static_assert(
            !std::is_union<T>::value,
            "====================> QOR Reflection: For safety reasons it is forbidden to reflect unions. See `Reflection of unions` section in the docs for more info."
        );
        std::forward<F>(f)(
            detail::tie_as_tuple(t)
        );
    }

}}//qor_reflection::detail

#endif//QOR_PP_H_REFLECTION_DETAIL_CORE17