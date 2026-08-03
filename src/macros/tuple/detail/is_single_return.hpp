# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2014.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef QOR_PP_H_MACROS_TUPLE_DETAIL_IS_SINGLE_RETURN
# define QOR_PP_H_MACROS_TUPLE_DETAIL_IS_SINGLE_RETURN
#
# include "src/macros/config/config.hpp"
#
# /* qor_pp_tuple_is_single_return */
#
# if qor_pp_variadics_msvc
# include "src/macros/control/iif.hpp"
# include "src/macros/facilities/is_1.hpp"
# include "src/macros/tuple/size.hpp"
# define qor_pp_tuple_is_single_return(sr,nsr,tuple)  \
    qor_pp_iff(qor_pp_is_1(qor_pp_tuple_size(tuple)),sr,nsr) \
    /**/
# endif//qor_pp_variadics_msvc
#
# endif//QOR_PP_H_MACROS_TUPLE_DETAIL_IS_SINGLE_RETURN
