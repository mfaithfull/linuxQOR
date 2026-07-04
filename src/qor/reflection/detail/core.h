// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_DETAIL_CORE
#define QOR_PP_H_REFLECTION_DETAIL_CORE

#pragma once

#include "config.h"

// Each core provides `qor_reflection::detail::tie_as_tuple` and
// `qor_reflection::detail::for_each_field_dispatcher` functions.
//
// The whole PFR library is build on top of those two functions.
#if qor_pp_refl_use_cpp17
#   include "core17.h"
#elif qor_pp_refl_use_loophole
#   include "core14_loophole.h"
#else
#   include "core14_classic.h"
#endif

#endif//QOR_PP_H_REFLECTION_DETAIL_CORE
