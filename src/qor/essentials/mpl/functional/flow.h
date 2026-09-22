// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_FUNCTIONAL_FLOW
#define QOR_PP_H_MPL_FUNCTIONAL_FLOW

#include "../compatability/dependentcall.h"

namespace qor { namespace mpl {

    namespace detail 
    {
        template <bool B, typename T, typename U>
        struct if_impl;

        template <typename T, typename U>
        struct if_impl<true, T, U> : T {};

        template <typename T, typename U>
        struct if_impl<false, T, U> : U {};
    }//detail

    template <typename P, typename TC, typename FC>
    struct if_ 
    {
        template <typename... Ts>
        using f = typename detail::if_impl<dcall<P, sizeof...(Ts)>::template f<Ts...>::value, TC, FC>::template f<Ts...>;
    };

}}//qor::mpl

#endif//QOR_PP_H_MPL_FUNCTIONAL_FLOW
