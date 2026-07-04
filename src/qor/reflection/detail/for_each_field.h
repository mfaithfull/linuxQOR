// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_DETAIL_FOREACHFIELD
#define QOR_PP_H_REFLECTION_DETAIL_FOREACHFIELD

#pragma once

#include "config.h"

#ifdef qor_pp_refl_has_std_module
import std;
#else
#include <type_traits>
#endif

#include "core.h"
#include "fields_count.h"
#include "for_each_field_impl.h"
#include "make_integer_sequence.h"

namespace qor_reflection { namespace detail {

    template <class T, class F>
    constexpr void for_each_field(T&& value, F&& func) 
    {
        constexpr std::size_t fields_count_val = qor_reflection::detail::fields_count<std::remove_reference_t<T>>();

        ::qor_reflection::detail::for_each_field_dispatcher(
            value,
            [f = std::forward<F>(func)](auto&& t) mutable 
            {
                // MSVC related workaround. Its lambdas do not capture constexprs.
                constexpr std::size_t fields_count_val_in_lambda
                    = qor_reflection::detail::fields_count<std::remove_reference_t<T>>();

                ::qor_reflection::detail::for_each_field_impl(
                    t,
                    std::forward<F>(f),
                    detail::make_index_sequence<fields_count_val_in_lambda>{},
                    std::is_rvalue_reference<T&&>{}
                );
            },
            detail::make_index_sequence<fields_count_val>{}
        );
    }

}}//qor_reflection::detail

#endif//QOR_PP_H_REFLECTION_DETAIL_FOREACHFIELD
