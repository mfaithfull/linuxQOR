// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_FUNCTIONS_ARITH_MODULO
#define QOR_PP_H_MPL_FUNCTIONS_ARITH_MODULO

namespace qor { namespace mpl {

    template <typename C = identity>
    struct modulo 
    {
        template <typename A, typename B>
        using f = typename C::template f<integral_constant<decltype(A::value % B::value), (A::value % B::value)>>;
    };

    namespace eager 
    {
        template <typename A, typename B>
        using modulo = integral_constant<decltype(A::value % B::value), (A::value % B::value)>;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_FUNCTIONS_ARITH_MODULO
