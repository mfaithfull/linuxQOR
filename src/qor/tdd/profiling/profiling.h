// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROFILING
#define QOR_PP_H_PROFILING

#ifdef qor_pp_profiling_enabled
#   define qor_pp_profiling_object FunctionProfiler
#else
#   define qor_pp_profiling_object NullProfiler
#endif

#ifndef qor_pp_coverage_enabled
#   define qor_pp_coverage_enabled false
#endif//

#define qor_pp_profile_enabled false
#define qor_pp_profile_begin "src/qor/tdd/profiling/profile_enable.inl"
#define qor_pp_profile_end "src/qor/tdd/profiling/profile_disable.inl"

#include "nullprofiler.h"
#include "functionprofiler.h"

#endif//QOR_PP_H_PROFILING