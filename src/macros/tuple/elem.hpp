# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002-2011) */
# /* Revised by Edward Diener (2011,2014,2020) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef QOR_PP_H_MACROS_TUPLE_ELEM
# define QOR_PP_H_MACROS_TUPLE_ELEM
#
# include "src/macros/cat.hpp"
# include "src/macros/control.hpp"
# include "src/macros/facilities/expand.hpp"
# include "src/macros/facilities/overload.hpp"
# include "rem.hpp"
# include "src/macros/variadic/elem.hpp"
# include "detail/is_single_return.hpp"
#
# if qor_pp_variadics_msvc
#     define qor_pp_tuple_elem(...) qor_pp_tuple_elem_i(qor_pp_overload(qor_pp_tuple_elem_o_, __VA_ARGS__), (__VA_ARGS__))
#     define qor_pp_tuple_elem_i(m, args) qor_pp_tuple_elem_ii(m, args)
#     define qor_pp_tuple_elem_ii(m, args) qor_pp_cat(m ## args,)
/*
  Use qor_pp_rem_cat if it is a single element tuple ( which might be empty )
  else use qor_pp_rem. This fixes a VC++ problem with an empty tuple and BOOST_PP_TUPLE_ELEM
  functionality. See tuple_elem_bug_test.cxx.
*/
#     define qor_pp_tuple_elem_o_2(n, tuple) \
         qor_pp_variadic_elem(n, qor_pp_expand(qor_pp_tuple_is_single_return(qor_pp_rem_cat,qor_pp_rem,tuple) tuple)) \
         /**/
# else
#     define qor_pp_tuple_elem(...) qor_pp_overload(qor_pp_tuple_elem_o_, __VA_ARGS__)(__VA_ARGS__)
#     define qor_pp_tuple_elem_o_2(n, tuple) qor_pp_variadic_elem(n, qor_pp_rem tuple)
# endif
# define qor_pp_tuple_elem_o_3(size, n, tuple) qor_pp_tuple_elem_o_2(n, tuple)
#

#
# define qor_pp_tuple_elem_1_0(a) a
#
# define qor_pp_tuple_elem_2_0(a, b) a
# define qor_pp_tuple_elem_2_1(a, b) b
#
# define qor_pp_tuple_elem_3_0(a, b, c) a
# define qor_pp_tuple_elem_3_1(a, b, c) b
# define qor_pp_tuple_elem_3_2(a, b, c) c
#
# endif//QOR_PP_H_MACROS_TUPLE_ELEM
