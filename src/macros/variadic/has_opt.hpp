# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2019.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef QOR_PP_H_MACROS_VARIADIC_HAS_OPT
# define QOR_PP_H_MACROS_VARIADIC_HAS_OPT
#
#include "src/macros/config/config.hpp"
#
# /* qor_pp_variadic_has_opt */
#
# if defined(__cplusplus) && __cplusplus > 201703L
#  if defined(__GNUC__) && !defined(__clang__) && __GNUC__ >= 8 && __GNUC__ < 10
#   define qor_pp_variadic_has_opt() 0
#  elif defined(__clang__) && __clang_major__ < 9
#   define qor_pp_variadic_has_opt() 0
#  else
#   include "src/macros/variadic/detail/has_opt.hpp"
#   define qor_pp_variadic_has_opt() \
  qor_pp_variadic_has_opt_elem2(qor_pp_variadic_has_opt_function(?),) \
/**/
#  endif
# else
# define qor_pp_variadic_has_opt() 0
# endif

# endif//QOR_PP_H_MACROS_VARIADIC_HAS_OPT
