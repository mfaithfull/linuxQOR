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
# ifndef QOR_PREPROCESSOR_CONTROL_IF_HPP
# define QOR_PREPROCESSOR_CONTROL_IF_HPP
#
# include "../config/config.hpp"
# include "iif.hpp"
# include "../logical/bool.hpp"
#
# /* qor_pp_if */
#
# if ~qor_pp_config_flags() & qor_pp_config_edg()
#    define qor_pp_if(cond, t, f) qor_pp_iif(qor_pp_bool(cond), t, f)
# else
#    define qor_pp_if(cond, t, f) qor_pp_if_i(cond, t, f)
#    define qor_pp_if_i(cond, t, f) qor_pp_iif(qor_pp_bool(cond), t, f)
# endif
#
# endif//QOR_PREPROCESSOR_CONTROL_IF_HPP
