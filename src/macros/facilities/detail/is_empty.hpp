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
#ifndef QOR_PREPROCESSOR_DETAIL_IS_EMPTY_HPP
#define QOR_PREPROCESSOR_DETAIL_IS_EMPTY_HPP

#include "../../punctuation/is_begin_parens.hpp"

#if qor_pp_variadics_msvc

//# pragma warning(once:4002)

#define qor_pp_detail_is_empty_iif_0(t, b) b
#define qor_pp_detail_is_empty_iif_1(t, b) t

#else

#define qor_pp_detail_is_empty_iif_0(t, ...) __VA_ARGS__
#define qor_pp_detail_is_empty_iif_1(t, ...) t

#endif

#if qor_pp_variadics_msvc && _MSC_VER <= 1400

#define qor_pp_detail_is_empty_process(param) \
	qor_pp_is_begin_parens \
    	( \
        qor_pp_detail_is_empty_non_function_c param () \
        ) \
/**/

#else

#define qor_pp_detail_is_empty_process(...) \
    qor_pp_is_begin_parens \
        ( \
            qor_pp_detail_is_empty_non_function_c __VA_ARGS__ () \
        ) \
/**/

#endif

#define qor_pp_detail_is_empty_primitive_cat(a, b) a ## b
#define qor_pp_detail_is_empty_iif(bit) qor_pp_detail_is_empty_primitive_cat(qor_pp_detail_is_empty_iif_,bit)
#define qor_pp_detail_is_empty_non_function_c(...) ()

#endif//QOR_PREPROCESSOR_DETAIL_IS_EMPTY_HPP
