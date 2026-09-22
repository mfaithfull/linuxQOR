// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_ISINSTANCE
#define QOR_PP_H_MPL_ISINSTANCE

#include "bool.h"

namespace qor { namespace mpl { namespace eager { namespace impl {

            template <template <typename...> class Type, typename T>
            struct is_instance_impl 
            {
                using f = bool_<false>;
            };

            template <template <typename...> class Type, typename... Ts>
            struct is_instance_impl<Type, Type<Ts...>> {
                using f = bool_<true>;
            };

        }//impl

        template <template <typename...> class Type, typename T>
        using is_instance = typename impl::is_instance_impl<Type, T>::f;
    }//eager

}}//qor::mpl

#endif//QOR_PP_H_MPL_ISINSTANCE
