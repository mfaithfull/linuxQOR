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
# /* Revised by Edward Diener (2011,2020) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef QOR_PP_H_MACROS_CONFIG_LIMITS
# define QOR_PP_H_MACROS_CONFIG_LIMITS
#
#include "src/macros/config/config.hpp"
#
# if defined(qor_pp_limit_dim)
# undef qot_pp_limit_dim
# endif
# if defined(qor_pp_limit_iteration_dim)
# undef qor_pp_limit_iteration_dim
# endif
# if defined(qor_pp_limit_slot_sig)
# undef qor_pp_limit_slot_sig
# endif
# if defined(qor_pp_limit_slot_count)
# undef qor_pp_limit_slot_count
# endif
# if defined(qor_pp_limit_while)
# undef qor_pp_limit_while
# endif
#
# if ~qor_pp_config_flags() & qor_pp_config_strict()
#
# if defined(qor_pp_limit_mag)
# undef qor_pp_limit_mag
# endif
# if defined(qor_pp_limit_variadic)
# undef qor_pp_limit_variadic
# endif
# if defined(qor_pp_limit_tuple)
# undef qor_pp_limit_tuple
# endif
# if defined(qor_pp_limit_for)
# undef qor_pp_limit_for
# endif
# if defined(qor_pp_limit_repeat)
# undef qor_pp_limit_repeat
# endif
# if defined(qor_pp_limit_seq)
# undef qor_pp_limit_seq
# endif
# if defined(qor_pp_limit_iteration)
# undef qor_pp_limit_iteration
# endif
#
# define qor_pp_limit_mag 256
# define qor_pp_limit_while 256
# define qor_pp_limit_variadic 64
# define qor_pp_limit_tuple 64
# define qor_pp_limit_for 256
# define qor_pp_limit_seq 256
# define qor_pp_limit_repeat 256
# define qor_pp_limit_iteration 256
#
#else
#
# if defined(qor_pp_limit_mag)
# if !(qor_pp_limit_mag == 256 || qor_pp_limit_mag == 512 || qor_pp_limit_mag == 1024)
# undef qor_pp_limit_mag
# define qor_pp_limit_mag 256
# define qor_pp_limit_while 256
# else
# define qor_pp_limit_while qor_pp_limit_mag
# if !defined(qor_pp_limit_seq)
# define qor_pp_limit_seq qor_pp_limit_mag
# endif
# endif
# else
# define qor_pp_limit_mag 256
# define qor_pp_limit_while 256
# endif
#
# if defined(qor_pp_limit_variadic)
# if !(qor_pp_limit_variadic == 64 || qor_pp_limit_variadic == 128 || qor_pp_limit_variadic == 256)
# undef qor_pp_limit_variadic
# define qor_pp_limit_variadic 64
# endif
# else
# define qor_pp_limit_variadic 64
# endif
#
# if defined(qor_pp_limit_tuple)
# if !(qor_pp_limit_tuple == 64 || qor_pp_limit_tuple == 128 || qor_pp_limit_tuple == 256)
# undef qor_pp_limit_tuple
# define qor_pp_limit_tuple 64
# elif qor_pp_limit_tuple > qor_pp_limit_variadic
# undef qor_pp_limit_variadic
# define qor_pp_limit_variadic qor_pp_limit_tuple
# endif
# else
# define qor_pp_limit_tuple 64
# endif
#
# if defined(qor_pp_limit_for)
# if !(qor_pp_limit_for == 256 || qor_pp_limit_for == 512 || qor_pp_limit_for == 1024)
# undef qor_pp_limit_for
# define qor_pp_limit_for 256
# elif qor_pp_limit_for > qor_pp_limit_mag
# undef qor_pp_limit_for
# define qor_pp_limit_for qor_pp_limit_mag
# endif
# else
# define qor_pp_limit_for 256
# endif
#
# if defined(qor_pp_limit_repeat)
# if !(qor_pp_limit_repeat == 256 || qor_pp_limit_repeat == 512 || qor_pp_limit_repeat == 1024)
# undef qor_pp_limit_repeat
# define qor_pp_limit_repeat 256
# elif qor_pp_limit_repeat > qor_pp_limit_mag
# undef qor_pp_limit_repeat
# define qor_pp_limit_repeat qor_pp_limit_mag
# endif
# else
# define qor_pp_limit_repeat 256
# endif
#
# if defined(qor_pp_limit_seq)
# if !(qor_pp_limit_seq == 256 || qor_pp_limit_seq == 512 || qor_pp_limit_seq == 1024)
# undef qor_pp_limit_seq
# define qor_pp_limit_seq 256
# elif qor_pp_limit_seq > qor_pp_limit_mag
# undef qor_pp_limit_seq
# define qor_pp_limit_seq qor_pp_limit_mag
# endif
# else
# define qor_pp_limit_seq 256
# endif
#
# if defined(qor_pp_limit_iteration)
# if !(qor_pp_limit_iteration == 256 || qor_pp_limit_iteration == 512 || qor_pp_limit_iteration == 1024)
# undef qor_pp_limit_iteration
# define qor_pp_limit_iteration 256
# elif qor_pp_limit_iteration > qor_pp_limit_mag
# undef qor_pp_limit_iteration
# define qor_pp_limit_iteration qor_pp_limit_mag
# endif
# else
# define qor_pp_limit_iteration 256
# endif
#
# endif
#
# define qor_pp_limit_dim 3
# define qor_pp_limit_iteration_dim 3
# define qor_pp_limit_slot_sig 10
# define qor_pp_limit_slot_count 5
#
# endif//QOR_PP_H_MACROS_CONFIG_LIMITS
