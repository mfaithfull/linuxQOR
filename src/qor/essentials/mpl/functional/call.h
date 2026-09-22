// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_FUNCTIONAL_CALL
#define QOR_PP_H_MPL_FUNCTIONAL_CALL

#include "../compatability/dependentcall.h"
#include "../list.h"

namespace qor { namespace mpl {

	namespace detail 
    {
        template <typename C, typename L, typename... Ts>
        struct unpack_impl;

        template <typename C, template <typename...> class Seq, typename... Ls, typename... Ts>
        struct unpack_impl<C, Seq<Ls...>, Ts...> 
        {
            using type = typename dcall<C, sizeof...(Ls)>::template f<Ts..., Ls...>;
        };
	}//detail

    //Turns a list of types into a variadic pack of those types /
    /// example: call<all<>,ture_,false_,true_> is equivalent to
    /// call<unpack<all<>>,list<true_,false_,true_>>
    template <typename C>
    struct unpack 
    {
        template <typename... Ls>
        using f = typename detail::unpack_impl<C, Ls...>::type;
    };

    //Call a continuation (left parameter) with a variadic pack
    template <typename C, typename... Ts>
    using call = typename dcall<C, sizeof...(Ts)>::template f<Ts...>;

    template <typename C = identity>
    struct call_f 
    {
        template <typename... Ts>
        using f = typename C::template f<dcallf<(sizeof...(Ts) < 10000)>::template f1<call, Ts...>>;
    };

    template <>
    struct call_f<identity> 
    {
        template <typename... Ts>
        using f = typename dcallf<(sizeof...(Ts) < 10000)>::template f1<call, Ts...>;
    };

}}//qor::mpl

#endif//QOR_PP_H_MPL_FUNCTIONAL_CALL