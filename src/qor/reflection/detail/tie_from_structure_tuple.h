// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_DETAIL_TIEFROMSTRUCTTUPLE
#define QOR_PP_H_REFLECTION_DETAIL_TIEFROMSTRUCTTUPLE

#pragma once

#include "config.h"

#include "core.h"

#include "stdtuple.h"
#include "../tuple_size.h"
#include "make_integer_sequence.h"

#ifdef qor_pp_refl_has_std_module
import std;
#else
#include <tuple>
#endif

namespace qor_reflection { namespace detail {

    // A `std::tuple` capable of de-structuring assignment used to support
    // a tie of multiple lvalue references to fields of an aggregate T.

    template <typename... Elements>
    struct tie_from_structure_tuple : std::tuple<Elements&...> 
    {
        using base = std::tuple<Elements&...>;
        using base::base;

        template <typename T>
        constexpr tie_from_structure_tuple& operator= (T const& t) 
        {
            base::operator=(
                detail::make_stdtiedtuple_from_tietuple(
                    detail::tie_as_tuple(t),
                    detail::make_index_sequence<tuple_size_v<T>>()));
            return *this;
        }
    };

}}//qor_reflection::detail

#endif//QOR_PP_H_REFLECTION_DETAIL_TIEFROMSTRUCTTUPLE