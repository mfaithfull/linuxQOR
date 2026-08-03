# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2011.                                  *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef QOR_PP_H_MACROS_FACILITIES_OVERLOAD
# define QOR_PP_H_MACROS_FACILITIES_OVERLOAD
#
# include "src/macros/cat.hpp"
# include "src/macros/variadic/size.hpp"
#
# /* qor_pp_overload */
#
# define qor_pp_overload(prefix, ...) qor_pp_cat(prefix, qor_pp_variadic_size(__VA_ARGS__))
#
# endif//QOR_PP_H_MACROS_FACILITIES_OVERLOAD

