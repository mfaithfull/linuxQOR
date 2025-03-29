# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2014.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
#
#ifndef QOR_PREPROCESSOR_DETAIL_IS_BEGIN_PARENS_HPP
#define QOR_PREPROCESSOR_DETAIL_IS_BEGIN_PARENS_HPP

#if qor_pp_variadics_msvc

#include "../../facilities/empty.hpp"

#define qor_pp_detail_vd_ibp_cat(a, b) qor_pp_detail_vd_ibp_cat_i(a, b)
#define qor_pp_detail_vd_ibp_cat_i(a, b) qor_pp_detail_vd_ibp_cat_ii(a ## b)
#define qor_pp_detail_vd_ibp_cat_ii(res) res

#define qor_pp_detail_ibp_split(i, ...) \
    qor_pp_detail_vd_ibp_cat(qor_pp_detail_ibp_primitive_cat(qor_pp_detail_ibp_split_,i)(__VA_ARGS__),qor_pp_empty()) \
/**/

#define qor_pp_detail_ibp_is_variadic_c(...) 1 1

#else

#define qor_pp_detail_ibp_split(i, ...) \
    qor_pp_detail_ibp_primitive_cat(qor_pp_detail_ibp_split_,i)(__VA_ARGS__) \
/**/

#define qor_pp_detail_ibp_is_variadic_c(...) 1

#endif// qor_pp_variadics_msvc

#define qor_pp_detail_ibp_split_0(a, ...) a
#define qor_pp_detail_ibp_split_1(a, ...) __VA_ARGS__

#define qor_pp_detail_ibp_cat(a, ...) qor_pp_detail_ibp_primitive_cat(a,__VA_ARGS__)
#define qor_pp_detail_ibp_primitive_cat(a, ...) a ## __VA_ARGS__

#define qor_pp_detail_ibp_is_variadic_r_1 1,
#define qor_pp_detail_ibp_is_variadic_r_qor_pp_detail_ibp_is_variadic_c 0,

#endif// QOR_PREPROCESSOR_DETAIL_IS_BEGIN_PARENS_HPP
