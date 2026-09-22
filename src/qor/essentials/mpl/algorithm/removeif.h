// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_ALGORITHM_REMOVEIF
#define QOR_PP_H_MPL_ALGORITHM_REMOVEIF

#include "transform.h"
#include "../functional/bind.h"
#include "../functional/call.h"
#include "../functional/identity.h"
#include "../sequence/join.h"

namespace qor { namespace mpl { namespace detail {

        template <bool>
        struct list_wrap_if;

        template <>
        struct list_wrap_if<true> 
        {
            template <typename T>
            using f = list<T>;
        };

        template <>
        struct list_wrap_if<false> 
        {
            template <typename>
            using f = list<>;
        };

    }//detail

    template <typename F>
    struct list_wrap_if 
    {
        template <typename T>
        using f = typename detail::list_wrap_if<F::template f<T>::value>::template f<T>;
    };
    
    template <template <typename> class F>
    struct list_wrap_if<cfe<F, identity>> 
    {
        template <typename T>
        using f = typename detail::list_wrap_if<F<T>::value>::template f<T>;
    };

    template <typename F>
    struct list_wrap_if_not 
    {
        template <typename T>
        using f = typename detail::list_wrap_if<(!F::template f<T>::value)>::template f<T>;
    };
    
    template <template <typename> class F>
    struct list_wrap_if_not<cfe<F, identity>> 
    {
        template <typename T>
        using f = typename detail::list_wrap_if<(!F<T>::value)>::template f<T>;
    };

    //Removes all elements for which a predicate holds
    template <typename F = identity, typename C = listify>
    using remove_if = transform<list_wrap_if_not<F>, join<C>>;

    namespace eager 
    {
        template <typename List, template <typename...> class Cond = identity>
        using remove_if = call<unpack<mpl::remove_if<cfe<Cond>>>, List>;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_ALGORITHM_REMOVEIF
