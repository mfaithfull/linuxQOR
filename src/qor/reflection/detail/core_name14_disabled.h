// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_DETAIL_CORENAME14_DISABLED
#define QOR_PP_H_REFLECTION_DETAIL_CORENAME14_DISABLED

#pragma once

#include "config.h"
#include "sequence_tuple.h"

namespace qor_reflection { namespace detail {

    template <class T, std::size_t I>
    constexpr auto get_name() noexcept 
    {
        static_assert(
            sizeof(T) && false,
            "====================> QOR Reflection: Field's names extracting functionality requires C++20."
        );

        return nullptr;
    }

    template <class T>
    constexpr auto tie_as_names_tuple() noexcept 
    {
        static_assert(
            sizeof(T) && false,
            "====================> QOR Reflection: Field's names extracting functionality requires C++20."
        );

        return detail::sequence_tuple::make_sequence_tuple();
    }

    template <class T, class F>
    constexpr void for_each_field_with_name(T&& /* value */, F&& /* func */) 
    {
        static_assert(
            sizeof(T) && false,
            "====================> QOR Reflection: Field's names extracting functionality requires C++20."
        );
    }

}}//qor_reflection::detail

#endif//QOR_PP_H_REFLECTION_DETAIL_CORENAME14_DISABLED
