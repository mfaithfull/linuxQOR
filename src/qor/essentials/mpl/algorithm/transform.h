// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_ALGORITHM_TRANSFORM
#define QOR_PP_H_MPL_ALGORITHM_TRANSFORM

#include "../compatability/dependentcall.h"
#include "../functional/bind.h"
#include "../functional/call.h"
#include "../list.h"

namespace qor { namespace mpl {

    //Executes the continuation `F` on every element in the input pack passing the results to the continuation `C`
    template <typename F = identity, typename C = listify>
    struct transform 
    {
        template <typename... Ts>
        using f = typename dcall<C, sizeof...(Ts)>::template f<typename F::template f<Ts>...>;
    };
    
    template <template <typename...> class F, typename C>
    struct transform<cfe<F, identity>, C> 
    {
        template <typename... Ts>
        using f = typename dcall<C, sizeof...(Ts)>::template f<
                typename dcallf<(sizeof...(Ts) < 100000)>::template f1<F, Ts>...>;
    };
    
    template <template <typename...> class F, template <typename...> class C>
    struct transform<cfe<F, identity>, cfe<C, identity>> 
    {
        template <typename... Ts>
        using f = C<F<Ts>...>;
    };
    
    template <typename F, template <typename...> class C>
    struct transform<F, cfe<C, identity>> 
    {
        template <typename... Ts>
        using f = typename dcallf<(sizeof...(Ts) <100000)>::template f1<C, typename F::template f<Ts>...>;
    };

    namespace eager 
    {
        template <typename List, template <typename...> class F>
        using transform = call<unpack<mpl::transform<cfe<F>, listify>>, List>;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_ALGORITHM_TRANSFORM
