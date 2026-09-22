// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_ALGORITHM_COUNTIF
#define QOR_PP_H_MPL_ALGORITHM_COUNTIF

#include "filter.h"
#include "../functional/bind.h"
#include "../sequence/size.h"
#include "../conditional.h"

namespace qor { namespace mpl { namespace detail {

        template <typename F>
        struct list_wrap_void_if 
        {
            template <typename T>
            using f = typename conditional<F::template f<T>::value>::template f<list<void>,
                                                                                list<>>;
        };

        template <template <typename...> class F>
        struct list_wrap_void_if<cfe<F, identity>> 
        {
            template <typename T>
            using f = typename conditional<F<T>::value>::template f<list<void>, list<>>;
        };

    }//detail

    //Counts elements for which the predicate holds
    // \effects resolves to uint_<X> where X is the number of elements in the input pack which
    // fulfill the provided predicate. \requires Type `F` shall be a `continuation predicate`
    // and `C` shall be any `continuation`. example call<count_if<same_as<void>>,void,int,char>
    // resolves to uint_<1>.
    template <typename F = identity, typename C = identity>
    using count_if = transform<detail::list_wrap_void_if<F>, join<size<C>>>;

    namespace eager 
    {
        template <typename List, template <typename...> class Cond = identity>
        using count_if = call<unpack<mpl::count_if<cfe<Cond>>>, List>;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_ALGORITHM_COUNTIF
