// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_ALGORITHM_SPLITIF
#define QOR_PP_H_MPL_ALGORITHM_SPLITIF

#include "foldright.h"
#include "../functional/call.h"
#include "../functional/flow.h"
#include "../sequence/at.h"
#include "../sequence/pushfront.h"
#include "../list.h"
#include "../conditional.h"

namespace qor { namespace mpl { namespace detail {

        template <typename T, typename U>
        struct split_push;
        template <typename... Ls, typename... Ts, typename U>
        struct split_push<list<list<Ts...>, Ls...>, U> 
        {
            using type = list<list<U, Ts...>, Ls...>;
        };

        template <typename F>
        using split_if_pred = if_<at1<F>, at0<unpack<push_front<list<>>>>, cfl<split_push>>;
    }//detail

    //Splits a lits into multiple lists at every point for which the provided predicate holds
    template <typename F = identity, typename C = listify>
    struct split_if 
    {
        template <typename... Ts>
        using f = typename dcall<
                fold_right<detail::split_if_pred<F>, push_front<unpack<C>, cfe<call>>>,
                sizeof...(Ts)>::template f<list<list<>>, Ts...>;
    };

    namespace eager 
    {
        template <typename List, template <typename...> class F = identity>
        using split_if = call<unpack<mpl::split_if<cfe<F>>>, List>;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_ALGORITHM_SPLITIF