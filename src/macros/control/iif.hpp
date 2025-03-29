# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
#
# ifndef QOR_PREPROCESSOR_CONTROL_IIF_HPP
# define QOR_PREPROCESSOR_CONTROL_IIF_HPP
#
# include "../config/config.hpp"
#
# if ~qor_pp_config_flags() & qor_pp_config_mwcc()
#    define qor_pp_iif(bit, t, f) qor_pp_iif_i(bit, t, f)
# else
#    define qor_pp_iif(bit, t, f) qor_pp_iif_oo((bit, t, f))
#    define qor_pp_iif_oo(par) qor_pp_iif_i ## par
# endif
#
# if ~qor_pp_config_flags() & qor_pp_config_msvc()
#    define qor_pp_iif_i(bit, t, f) qor_pp_iif_ ## bit(t, f)
# else
#    define qor_pp_iif_i(bit, t, f) qor_pp_iif_ii(qor_pp_iif_ ## bit(t, f))
#    define qor_pp_iif_ii(id) id
# endif
#
# define qor_pp_iif_0(t, f) f
# define qor_pp_iif_1(t, f) t
#
# endif//QOR_PREPROCESSOR_CONTROL_IIF_HPP
