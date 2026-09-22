// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_SEQUENCE_PUSHBACK
#define QOR_PP_H_MPL_SEQUENCE_PUSHBACK

#include "../compatability/dependentcall.h"
#include "../list.h"

namespace qor { namespace mpl {

    template <typename Input, typename C = listify>
    struct push_back 
    {
        template <typename... Ts>
        using f = typename dcall<C, sizeof...(Ts)>::template f<Ts..., Input>;
    };

    namespace eager 
    {
        namespace impl 
        {
            template <typename Elem, typename List>
            struct push_back_impl 
            {
                // invalid lists get ignored
                using f = Elem;
            };

            /// kvasir::mpl::list implementation
            template <typename Elem, typename... Ts>
            struct push_back_impl<Elem, mpl::list<Ts...>> 
            {
                using f = mpl::list<Ts..., Elem>;
            };
        }//impl

        /// push an element to the back of a list
        /// be wary that pushing to the back of a list can be a linear operation, and is
        /// therefore not recommended as a way of adding to a list instead, it is conventional
        /// to recurse backwards and push to the front instead
        template <typename Elem, typename List>
        using push_back = typename impl::push_back_impl<Elem, List>::f;
    }//eager
}}//qor::mpl

#endif//QOR_PP_H_MPL_SEQUENCE_PUSHBACK