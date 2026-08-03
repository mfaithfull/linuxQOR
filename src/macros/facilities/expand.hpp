# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef QOR_PP_H_MACROS_FACILITIES_EXPAND
# define QOR_PP_H_MACROS_FACILITIES_EXPAND
#
# include "src/macros/config/config.hpp"
#
# if ~qor_pp_config_flags() & qor_pp_config_mwcc() && ~qor_pp_config_flags() & qor_pp_config_dmc()
#    define qor_pp_expand(x) qor_pp_expand_i(x)
# else
#    define qor_pp_expand(x) qor_pp_expand_oo((x))
#    define qor_pp_expand_oo(par) qor_pp_expand_i ## par
# endif
#
# define qor_pp_expand_i(x) x
#
# endif//QOR_PP_H_MACROS_FACILITIES_EXPAND
