// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_ALGORITHM_REMOVEADJACENT
#define QOR_PP_H_MPL_ALGORITHM_REMOVEADJACENT

#include <type_traits>

#include "removeif.h"
#include "rotate.h"
#include "../compatability/dependentcall.h"
#include "../conditional.h"

namespace qor { namespace mpl { namespace detail {

        template <template <typename...> class Pred>
        struct binary_list_if_not 
        {
            template <typename T, typename U>
            using f = typename conditional<Pred<T, U>::value>::template f<list<>, list<T>>;
        };

        template <template <typename...> class Pred, typename T, typename U, typename C = listify>
        struct remove_adjacent;

        template <template <typename...> class Pred, typename... Ts, typename... Us, typename C>
        struct remove_adjacent<Pred, list<Ts...>, list<Us...>, C> 
        {
            using type = typename dcall<join<C>, sizeof...(Ts)>::template f<
                    typename binary_list_if_not<Pred>::template f<Ts, Us>...>;
        };

    }

    //Removes each element in a list which is the same type as the previous element
    template <typename F, typename C = listify>
    struct remove_adjacent 
    {
        template <typename... Ts>
        using f = typename detail::remove_adjacent<
                F::template f, list<Ts...>,
                typename dcall<rotate<uint_<1>>, sizeof...(Ts)>::template f<Ts...>, C>::type;
    };
    
    template <template <typename... Ts> class F, typename C>
    struct remove_adjacent<cfe<F, identity>, C> {
        template <typename... Ts>
        using f = typename detail::remove_adjacent<
                F, list<Ts...>,
                typename dcall<rotate<uint_<1>>, sizeof...(Ts)>::template f<Ts...>, C>::type;
    };

    namespace eager 
    {
        // takes a boolean predicate with two parameters
        // if the predicate return true for any two adjacent elements,
        // then the first of the two elements is removed
        template <typename List, template <typename...> class Pred = std::is_same>
        using remove_adjacent = call<unpack<mpl::remove_adjacent<cfe<Pred>>>, List>;
    }//eager

}}//qor::mpl

#endif//QOR_PP_H_MPL_ALGORITHM_REMOVEADJACENT
