// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_DETAIL_CONFIG
#define QOR_PP_H_REFLECTION_DETAIL_CONFIG

#pragma once

#include "../config.h"

#if !qor_pp_refl_enabled

#error QOR Reflection library is not supported in your environment.             \
       Try one of the possible solutions:                                  \
       1. try to take away an '-Dqor_pp_refl_enabled=0', if it exists        \
       2. enable C++14;                                                    \
       3. enable C++17;                                                    \
       4. update your compiler;                                            \
       or disable this error by '-Dqor_pp_refl_enabled=1' if you really know what are you doing.

#endif // !qor_pp_refl_enabled

#endif//QOR_PP_H_REFLECTION_DETAIL_CONFIG