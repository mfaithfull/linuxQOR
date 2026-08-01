// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from the Frozen library by QuarksLab

/*
 * Frozen
 * Copyright 2016 QuarksLab
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef QOR_PP_H_DATASTRUCTURES_FROZEN_DEFINES
#define QOR_PP_H_DATASTRUCTURES_FROZEN_DEFINES

#include "src/platform/compiler/compiler.h"

//#if (qor_pp_compiler == qor_pp_compiler_msvc)
//  #define FROZEN_LETITGO_IS_MSVC
//#endif

//#define FROZEN_LETITGO_HAS_CXX17  1 //QOR requires C++20
#define qor_pp_has_string_view // We assume Visual Studio always has string_view
#define qor_pp_has_char8_t

#if defined(__cpp_deduction_guides) && __cpp_deduction_guides >= 201703L
  #define qor_pp_has_deduction_guides
#endif

//#if defined(__cpp_lib_constexpr_string) && __cpp_lib_constexpr_string >= 201907L
//  #define qor_pp_has_constexpr_string
//#endif

#endif//QOR_PP_H_DATASTRUCTURES_FROZEN_DEFINES
