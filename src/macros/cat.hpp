// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PREPROCESSOR_CAT
#define QOR_PP_H_PREPROCESSOR_CAT

#include "config/config.hpp"

//qor_pp_cat

# if ~qor_pp_config_flags() & qor_pp_config_mwcc()
#    define qor_pp_cat(a, b) qor_pp_cat_i(a, b)
# else
#    define qor_pp_cat(a, b) qor_pp_cat_oo((a, b))
#    define qor_pp_cat_oo(par) qor_pp_cat_i ## par
# endif
#
# if (~qor_pp_config_flags() & qor_pp_config_msvc()) || (defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 1700)
#    define qor_pp_cat_i(a, b) a##b
# else
#    define qor_pp_cat_i(a, b) qor_pp_cat_ii(~, a ## b)
#    define qor_pp_cat_ii(p, res) res
# endif

#endif//QOR_PP_H_PREPROCESSOR_CAT
