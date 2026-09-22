// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_ALGORITHM_GROUP
#define QOR_PP_H_MPL_ALGORITHM_GROUP

#include "foldleft.h"
#include "rotate.h"
#include "zipwith.h"
#include "../functional/bind.h"
#include "../functional/call.h"
#include "../sequence/pushfront.h"
#include "../sequence/take.h"
#include "../int.h"
#include "../list.h"
#include "../../traits/typetraits.h"
#include "../conditional.h"

namespace qor { namespace mpl { namespace detail {

        template <typename T, typename U>
        struct group_push;

        template <typename... Ls, typename... Ts, typename U1, typename U2>
        struct group_push<list<list<Ts...>, Ls...>, list<U1, U2>> 
        {
            using type = list<list<Ts..., U2>, Ls...>;
        };

        template <typename T, typename U>
        struct group_next;

        template <typename L, typename... Ls, typename U1, typename U2>
        struct group_next<list<L, Ls...>, list<U1, U2>> 
        {
            using type = list<list<U2>, Ls..., L>;
        };

        template <typename Pred>
        struct group_pred 
        {
            template <typename In, typename Pair>
            using f = typename conditional<call<unpack<Pred>, Pair>::value>::template f<
                    group_push<In, Pair>, group_next<In, Pair>>::type;
        };

        template <typename C, bool>
        struct group_impl;

        template <typename C>
        struct group_impl<C, 0> 
        {
            template <typename Pred>
            using f = list<>;
        };

        template <typename C>
        struct group_impl<C, 1> 
        {
            template <typename Pred, typename T0, typename... Ts>
            using f = typename dcall<
                    zip_with<listify, push_front<list<list<T0>>,
                                                    fold_left<group_pred<Pred>,
                                                            unpack<rotate<mpl::uint_<1>, C>>>>>,
                    sizeof...(Ts)>::template f<call<take<uint_<sizeof...(Ts)>>, T0, Ts...>,
                                                list<Ts...>>;
        };

    }//detail

    //Group adjacent elements that respect a predicate in a sublist
    // \requires Type `Pred` shall be a `continuation binary predicate` and `C` shall be any
    // `continuation`.
    // \example call<group<is_same<>>,void,void,int,void>
    // resolves to list<list<void,void>,list<int>,list<void>>
    template <typename Pred = is_same<>, typename C = listify>
    struct group 
    {
        template <typename... Ts>
        using f = typename detail::group_impl<C, sizeof...(Ts) != 0>::template f<Pred, Ts...>;
    };

    namespace eager 
    {
        template <typename List, template <typename...> class Pred = std::is_same>
        using group = call<unpack<mpl::group<cfe<Pred>>>, List>;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_ALGORITHM_GROUP
