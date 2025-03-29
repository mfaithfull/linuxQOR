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
# ifndef QOR_PREPROCESSOR_IS_BEGIN_PARENS_HPP
# define QOR_PREPROCESSOR_IS_BEGIN_PARENS_HPP

# include "../config/config.hpp"

#if qor_pp_variadics

#include "detail/is_begin_parens.hpp"

#if qor_pp_variadics_msvc && _MSC_VER <= 1400

#define qor_pp_is_begin_parens(param) \
    qor_pp_detail_ibp_split \
      ( \
      0, \
      qor_pp_detail_ibp_cat \
        ( \
        qor_pp_detail_ibp_is_variadic_r_, \
        qor_pp_detail_ibp_is_variadic_c param \
        ) \
      ) \
/**/

#else

#define qor_pp_is_begin_parens(...) \
    qor_pp_detail_ibp_split \
      ( \
      0, \
      qor_pp_detail_ibp_cat \
        ( \
        qor_pp_detail_ibp_is_variadic_r_, \
        qor_pp_detail_ibp_is_variadic_c __VA_ARGS__ \
        ) \
      ) \
/**/

#endif // qor_pp_variadics_msvc && _MSC_VER <= 1400
#endif // qor_pp_variadics
#endif // QOR_PREPROCESSOR_IS_BEGIN_PARENS_HPP
