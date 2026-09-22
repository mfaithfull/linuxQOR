// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_FUNCTIONAL_BIND
#define QOR_PP_H_MPL_FUNCTIONAL_BIND

#include "../compatability/dependentcall.h"
#include "identity.h"

namespace qor { namespace mpl {

    //makes a continuation from a lazy metafunction /
    // wraps a template template parameter in such a way that it can be used as a
    // continuation capable meta closure.
    // \requires fixed parameters: a template template parameter which contains a nestesd
    // ::type (the lazy metafunction) dynamic parameters: a variadic pack of type parameters
    // \effects the result will be whatever the ::type of the lazy metafunction maps to when
    // passed the dynamic parameters
    template <template <typename...> class F, typename C = identity>
    struct cfl 
    {
        template <typename... Ts>
        using f = typename dcall<C, sizeof...(Ts)>::template f<
                typename dcallf<bool(sizeof...(Ts) > 0)>::template f1<F, Ts...>::type>;
    };
    
    template <template <typename...> class F>
    struct cfl<F, identity> 
    {
        template <typename... Ts>
        using f = typename dcallf<bool(sizeof...(Ts) > 0)>::template f1<F, Ts...>::type;
    };

    // makes a continuation from an eager metafunction /
    // wraps a template template parameter in such a way that it can be used as a
    // continuation capable meta closure.
    // \requires fixed parameters: a template template parameter (the eager metafunction)
    // dynamic parameters: a variadic pack of type parameters
    // \effects the result will be whatever the eager metafunction maps to when passed the
    // dynamic parameters
    // \notes when passed an alias the result is the same as a traditional call to the at
    // metafunction/ \notes it is perfectly valid to pass a template such as tuple or pair as a
    // 'metafunction'
    template <template <typename...> class F, typename C = identity>
    struct cfe 
    {
        template <typename... Ts>
        using f = typename dcall<C, sizeof...(Ts)>::template f<
                typename dcallf<bool(sizeof...(Ts) > 0)>::template f1<F, Ts...>>;
    };
    
    template <template <typename...> class F>
    struct cfe<F, identity> 
    {
        template <typename... Ts>
        using f = typename dcallf<bool(sizeof...(Ts) > 0)>::template f1<F, Ts...>;
    };

}}//qor::mpl

#endif//QOR_PP_H_MPL_FUNCTIONAL_BIND