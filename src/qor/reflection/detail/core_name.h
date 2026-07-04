// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_DETAIL_CORENAME
#define QOR_PP_H_REFLECTION_DETAIL_CORENAME

#pragma once

#include "config.h"

// Each core_name provides `qor_reflection::detail::get_name` and
// `qor_reflection::detail::tie_as_names_tuple` functions.
//
// The whole functional of extracting field's names is build on top of those
// two functions.
#if qor_pp_refl_name_enabled
#include "core_name20_static.h"
#else
#include "core_name14_disabled.h"
#endif

#endif//QOR_PP_H_REFLECTION_DETAIL_CORENAME