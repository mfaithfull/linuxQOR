# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2019.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef QOR_PP_H_MACROS_FACILITIES_CHECK_EMPTY_HPP
# define QOR_PP_H_MACROS_FACILITIES_CHECK_EMPTY_HPP

# include "src/macros/variadic/has_opt.hpp"
# if qor_pp_variadic_has_opt()
# include "src/macros/facilities/is_empty_variadic.hpp"
# define qor_pp_check_empty(...) qor_pp_is_empty_opt(__VA_ARGS__)
# endif//qor_pp_variadic_has_opt()

# endif//QOR_PP_H_MACROS_FACILITIES_CHECK_EMPTY_HPP
