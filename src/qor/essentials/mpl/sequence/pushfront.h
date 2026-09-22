// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_SEQUENCE_PUSHFRONT
#define QOR_PP_H_MPL_SEQUENCE_PUSHFRONT

#include "../compatability/dependentcall.h"
#include "../list.h"

namespace qor { namespace mpl {

    template <typename Input, typename C = listify>
    struct push_front 
    {
        template <typename... Ts>
        using f = typename dcall<C, sizeof...(Ts)>::template f<Input, Ts...>;
    };

    namespace eager { namespace impl {

            template <typename Elem, typename List>
            struct push_front_impl;

            // qor::mpl::list implementation
            template <typename Elem, typename... Ts>
            struct push_front_impl<Elem, mpl::list<Ts...>> 
            {
                using f = mpl::list<Elem, Ts...>;
            };

        }//impl

        // push an element to the front of a list
        template <typename Elem, typename List>
        using push_front = typename impl::push_front_impl<Elem, List>::f;
    }//eager

}}//qor::mpl

#endif//QOR_PP_H_MPL_SEQUENCE_PUSHFRONT
