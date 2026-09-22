// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_FUNCTIONAL_FORK
#define QOR_PP_H_MPL_FUNCTIONAL_FORK

#include "../algorithm/rotate.h"
#include "bind.h"
#include "../list.h"

namespace qor { namespace mpl {

	namespace detail {

        template <typename C, typename... Fs>
        struct each_impl 
        {
            template <typename... Ts>
            using f = typename dcall<C, sizeof...(Ts)>::template f<typename Fs::template f<Ts>...>;
        };

        template <typename C, typename... Fs>
        struct fork_impl 
        {
            template <typename... Ts>
            using f = typename dcall<C, sizeof...(Fs)>::template f<typename dcall<Fs, sizeof...(Ts)>::template f<Ts...>...>;
        };
	} // namespace detail

	// execute multiple continuations passing all dynamic parameters to each
    template <typename... Ts>
    struct fork : rotate<uint_<sizeof...(Ts)-1>, cfe<detail::fork_impl>>::template f<Ts...> {};


    template <typename F0, typename C>
    struct fork<F0, C> 
    {
        template <typename... Ts>
        using f = typename C::template f<typename dcall<F0, sizeof...(Ts)>::template f<Ts...>>;
    };

    template <typename F0, typename F1, typename C>
    struct fork<F0, F1, C> 
    {
        template <typename... Ts>
        using f = typename C::template f<typename dcall<F0, sizeof...(Ts)>::template f<Ts...>, typename dcall<F1, sizeof...(Ts)>::template f<Ts...>>;
    };

    //execute multiple continuations each taking the dynamic parameter corresponding to its position
    template <typename... Ts>
    struct each : rotate<uint_<sizeof...(Ts)-1>, cfe<detail::each_impl>>::template f<Ts...> {};

    template <typename F0, typename C>
    struct each<F0, C> 
    {
        template <typename... Ts>
        using f = typename C::template f<typename F0::template f<Ts>...>;
    };
		
    template <typename F0, typename F1, typename C>
    struct each<F0, F1, C> 
    {
        template <typename T0, typename T1, typename... Ts>
        using f = typename C::template f<typename F0::template f<T0>, typename F1::template f<T1>>;
    };
		
    template <typename F, typename C>
    struct fork_front 
    {
        template <typename... Ts>
        using f = typename dcall<C, sizeof...(Ts)>::template f<typename dcall<F, sizeof...(Ts)>::template f<Ts...>, Ts...>;
    };

}}//qor::mpl

#endif//QOR_PP_H_MPL_FUNCTIONAL_FORK
