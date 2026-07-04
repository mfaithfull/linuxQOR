// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_DETAIL_SIZET
#define QOR_PP_H_REFLECTION_DETAIL_SIZET
#pragma once

namespace qor_reflection { namespace detail {

    ///////////////////// General utility stuff
    template <std::size_t Index>
    using size_t_ = std::integral_constant<std::size_t, Index >;

}} //qor_reflection::detail

#endif//QOR_PP_H_REFLECTION_DETAIL_SIZET