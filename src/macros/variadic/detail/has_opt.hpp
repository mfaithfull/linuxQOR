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

# ifndef QOR_PP_H_MACROS_VARIADIC_DETAIL_HAS_OPT
# define QOR_PP_H_MACROS_VARIADIC_DETAIL_HAS_OPT

# include "src/macros/config/config.hpp"

# if defined(__cplusplus) && __cplusplus > 201703L

# if qor_pp_variadics_msvc
# include "src/macros/cat.hpp"
# endif

# define qor_pp_variadic_has_opt_function(...) \
    __VA_OPT__(,) , 1, 0 \
/**/

# if qor_pp_variadics_msvc
# define qor_pp_variadic_has_opt_elem0(e0, ...) qor_pp_cat(qor_pp_variadic_has_opt_elem_0(e0,__VA_ARGS__),)
# define qor_pp_variadic_has_opt_elem2(e0, ...) qor_pp_cat(qor_pp_variadic_has_opt_elem_2(e0,__VA_ARGS__),)
# else
# define qor_pp_variadic_has_opt_elem0(e0, ...) qor_pp_variadic_has_opt_elem_0(e0,__VA_ARGS__)
# define qor_pp_variadic_has_opt_elem2(e0, ...) qor_pp_variadic_has_opt_elem_2(e0,__VA_ARGS__)
# endif
# define qor_pp_variadic_has_opt_elem_0(e0, ...) e0
# define qor_pp_variadic_has_opt_elem_2(e0, e1, e2, ...) e2

# endif

# endif//QOR_PP_H_MACROS_VARIADIC_DETAIL_HAS_OPT
