
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_SEQUENCE_POPFRONT
#define QOR_PP_H_MPL_SEQUENCE_POPFRONT

#include <type_traits>
#include "../compatability/dependentcall.h"
#include "../list.h"
#include "../nothing.h"

namespace qor { namespace mpl {

    template <typename C = listify>
    struct pop_front 
    {
        template <typename T, typename... Ts>
        using f = typename dcall<C, sizeof...(Ts)>::template f<Ts...>;
    };

	namespace eager { namespace impl {

            template <typename List>
            struct pop_front_impl 
            {
                // when used with any element, just return the element
                using front = List;
                using rest  = nothing;
            };

            template <typename T, typename... Ts>
            struct pop_front_impl<mpl::list<T, Ts...>> 
            {
                using front = T;
                using rest  = mpl::list<Ts...>;
            };

            template <>
            struct pop_front_impl<mpl::list<>> 
            {
                // when the list is empty return nothing as the element and an empty list as
                // rest
                using front = nothing;
                using rest  = mpl::list<>;
            };

		} // namespace impl

        // pop a single element from the front of a list
        // returns a pair of the first element and the rest of the list, with ::front and
        // ::rest respectively when the input is not a list, ::front will be the element, and
        // ::rest will be nothing when the input is an empty list, ::front will be nothing, and
        // ::rest will be an empty list
        template <typename List>
        using pop_front = impl::pop_front_impl<List>;

	} // namespace eager
}}//qor::mpl

#endif//QOR_PP_H_MPL_SEQUENCE_POPFRONT
