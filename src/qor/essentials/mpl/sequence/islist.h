// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_SEQUENCE_ISLIST
#define QOR_PP_H_MPL_SEQUENCE_ISLIST

#include "../bool.h"
#include "../list.h"

namespace qor { namespace mpl { namespace eager { namespace impl {

            template <typename Elem>
            struct is_list 
            {
                using f = bool_<false>;
            };

            template <typename... Ts>
            struct is_list<mpl::list<Ts...>> 
            {
                using f = bool_<true>;
            };
		} // namespace impl

        template <typename Elem>
        using is_list = typename impl::is_list<Elem>::f;

	} // namespace eager
}}//qor::mpl

#endif//QOR_PP_H_MPL_SEQUENCE_ISLIST
