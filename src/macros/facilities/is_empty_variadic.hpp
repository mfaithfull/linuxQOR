# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2014.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef QOR_PREPROCESSOR_FACILITIES_IS_EMPTY_VARIADIC_HPP
# define QOR_PREPROCESSOR_FACILITIES_IS_EMPTY_VARIADIC_HPP
#
# include "../config/config.hpp"
#
# if qor_pp_variadics
# include "../punctuation/is_begin_parens.hpp"
# include "detail/is_empty.hpp"
#
#if qor_pp_variadics_msvc && _MSC_VER <= 1400
#
#define qor_pp_is_empty(param) \
    qor_pp_detail_is_empty_iif \
      ( \
      qor_pp_is_begin_parens \
        ( \
        param \
        ) \
      ) \
      ( \
      qor_pp_is_empty_zero, \
      qor_pp_detail_is_empty_process \
      ) \
    (param) \
/**/
#define qor_pp_is_empty_zero(param) 0
# else
#define qor_pp_is_empty(...) \
    qor_pp_detail_is_empty_iif \
      ( \
        qor_pp_is_begin_parens \
        ( \
        __VA_ARGS__ \
        ) \
      ) \
      ( \
        qor_pp_is_empty_zero, \
        qor_pp_detail_is_empty_process \
      ) \
    (__VA_ARGS__) \
/**/
#define qor_pp_is_empty_zero(...) 0
# endif // qor_pp_variadics_msvc && _MSC_VER <= 1400
# endif // qor_pp_variadics
# endif // QOR_PREPROCESSOR_FACILITIES_IS_EMPTY_VARIADIC_HPP
