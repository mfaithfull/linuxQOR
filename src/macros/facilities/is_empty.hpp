# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2003.
#  *     (C) Copyright Edward Diener 2014.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef qor_preprocessor_facilities_is_empty_hpp
# define qor_preprocessor_facilities_is_empty_hpp
#
# include "../config/config.hpp"
#
# if qor_pp_variadics
#
# include "is_empty_variadic.hpp"
#
# else
#
# if ~qor_pp_config_flags() & qor_pp_config_msvc() && ~qor_pp_config_flags() & qor_pp_config_mwcc()
# include <configuration/preprocessor/tuple/elem.hpp>
# include <configuration/preprocessor/facilities/identity.hpp>
# else
# include <configuration/preprocessor/cat.hpp>
# include <configuration/preprocessor/detail/split.hpp>
# endif
#
# // qor_pp_is_empty */
#
# if ~qor_pp_config_flags() & qor_pp_config_msvc() && ~qor_pp_config_flags() & qor_pp_config_mwcc()
#    define qor_pp_is_empty(x) qor_pp_is_empty_i(x qor_pp_is_empty_helper)
#    define qor_pp_is_empty_i(contents) qor_pp_tuple_elem(2, 1, (qor_pp_is_empty_def_ ## contents()))
#    define qor_pp_is_empty_def_qor_pp_is_empty_helper 1, qor_pp_identity(1)
#    define qor_pp_is_empty_helper() , 0
# else
#    if qor_pp_config_flags() & qor_pp_config_msvc()
#        define QOR_PP_IS_EMPTY(x) qor_pp_is_empty_i(qor_pp_is_empty_helper x ())
#        define qor_pp_is_empty_i(test) qor_pp_is_empty_ii(qor_pp_split(0, qor_pp_cat(qor_pp_is_empty_def_, test)))
#        define qor_pp_is_empty_ii(id) id
#    else
#        define QOR_PP_IS_EMPTY(x) qor_pp_is_empty_i((qor_pp_is_empty_helper x ()))
#        define qor_pp_is_empty_i(par) qor_pp_is_empty_ii ## par
#        define qor_pp_is_empty_ii(test) qor_pp_split(0, qor_pp_cat(qor_pp_is_empty_def_, test))
#    endif
#    define qor_pp_is_empty_helper() 1
#    define qor_pp_is_empty_def_1 1, qor_pp_nil
#    define qor_pp_is_empty_def_qor_pp_is_empty_helper 0, qor_pp_nil
# endif
#
# endif //qor_pp_variadics
#
# endif//qor_preprocessor_facilities_is_empty_hpp
