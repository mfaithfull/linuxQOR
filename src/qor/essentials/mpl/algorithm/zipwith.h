// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_ALGORITHM_ZIPWITH
#define QOR_PP_H_MPL_ALGORITHM_ZIPWITH

#include "transform.h"
#include "../functional/bind.h"
#include "../functional/call.h"
#include "../list.h"
#include "../nothing.h"
#include "../always.h"

#include "../sequence/popfront.h"
#include "../sequence/pushfront.h"
#include "../sequence/size.h"

namespace qor { namespace mpl { namespace detail {

        template <typename>
        struct append;

        template <template <typename...> class Seq, typename... Ts0>
        struct append<Seq<Ts0...>> 
        {
            template <typename... Ts1>
            using f = Seq<Ts0..., Ts1...>;
        };

        template <typename F, typename C, typename Result, typename... Ts>
        struct zip_with_impl 
        {
            using f = call<unpack<transform<unpack<F>, C>>, Result>;
        };

        template <typename F, typename C, template <typename...> class Seq, typename... Result,
                    template <typename...> class Seq0, typename... Ts0, typename... Ts>
        struct zip_with_impl<F, C, Seq<Result...>, Seq0<Ts0...>, Ts...> 
        {
            using f = typename zip_with_impl<
                    F, C, list<typename append<Result>::template f<Ts0>...>, Ts...>::f;
        };

        template <typename F, typename C, template <typename...> class Seq, typename... Result,
                    template <typename...> class Seq0, typename... Ts0,
                    template <typename...> class Seq1, typename... Ts1, typename... Ts>
        struct zip_with_impl<F, C, Seq<Result...>, Seq0<Ts0...>, Seq1<Ts1...>, Ts...> 
        {
            using f = typename zip_with_impl<
                    F, C, list<typename append<Result>::template f<Ts0, Ts1>...>, Ts...>::f;
        };

        template <typename F, typename C, template <typename...> class Seq, typename... Result,
                    template <typename...> class Seq0, typename... Ts0,
                    template <typename...> class Seq1, typename... Ts1,
                    template <typename...> class Seq2, typename... Ts2,
                    template <typename...> class Seq3, typename... Ts3, typename... Ts>
        struct zip_with_impl<F, C, Seq<Result...>, Seq0<Ts0...>, Seq1<Ts1...>, Seq2<Ts2...>,
                                Seq3<Ts3...>, Ts...> 
        {
            using f = typename zip_with_impl<
                    F, C, list<typename append<Result>::template f<Ts0, Ts1, Ts2, Ts3>...>,
                    Ts...>::f;
        };

        template <typename F, typename C, template <typename...> class Seq, typename... Result,
                    template <typename...> class Seq0, typename... Ts0,
                    template <typename...> class Seq1, typename... Ts1,
                    template <typename...> class Seq2, typename... Ts2,
                    template <typename...> class Seq3, typename... Ts3,
                    template <typename...> class Seq4, typename... Ts4,
                    template <typename...> class Seq5, typename... Ts5,
                    template <typename...> class Seq6, typename... Ts6,
                    template <typename...> class Seq7, typename... Ts7, typename... Ts>
        struct zip_with_impl<F, C, Seq<Result...>, Seq0<Ts0...>, Seq1<Ts1...>, Seq2<Ts2...>,
                                Seq3<Ts3...>, Seq4<Ts4...>, Seq5<Ts5...>, Seq6<Ts6...>,
                                Seq7<Ts7...>, Ts...> 
        {
            using f = typename zip_with_impl<
                    F, C, list<typename append<Result>::template f<Ts0, Ts1, Ts2, Ts3>...>,
                    Ts...>::f;
        };

        template <typename F, typename C, typename... Ts>
        struct zip_with_unpack 
        {
            // in the case of no lists just call c
            using f = typename C::template f<>;
        };

        template <typename F, typename C, typename T, typename... Ts>
        struct zip_with_unpack<F, C, T, Ts...> 
        {
            using f =
                    typename zip_with_impl<F, C, call<unpack<transform<listify>>, T>, Ts...>::f;
        };

    }//detail

    // n-ary version of transform
    template <typename F = listify, typename C = listify>
    struct zip_with 
    {
        template <typename... Ts>
        using f = typename detail::zip_with_unpack<F, C, Ts...>::f;
    };

    namespace eager 
    {
        template <template <typename...> class Func, typename... Lists>
        using zip_with = call<zip_with<cfe<Func>>, Lists...>;
    }
    
}}//qor::mpl

#endif//QOR_PP_H_MPL_ALGORITHM_ZIPWITH
