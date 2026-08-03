# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2003.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef QOR_PP_H_MACROS_FACILITIES_IS_1
# define QOR_PP_H_MACROS_FACILITIES_IS_1
#
# include "src/macros/cat.hpp"
# include "src/macros/facilities/is_empty.hpp"
#
# /* qor_pp_is_1 */
#
# define qor_pp_is_1(x) qor_pp_is_empty(qor_pp_cat(qor_pp_is_1_helper_, x))
# define qor_pp_is_1_helper_1
#
# endif//QOR_PP_H_MACROS_FACILITIES_IS_1
