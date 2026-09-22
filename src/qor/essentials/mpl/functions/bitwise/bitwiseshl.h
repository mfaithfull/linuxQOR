// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_FUNCTIONS_BITWISE_SHL
#define QOR_PP_H_MPL_FUNCTIONS_BITWISE_SHL

#include "../../functional/identity.h"
#include "../../integralconstant.h"

namespace qor { namespace mpl {

    template <typename C = identity>
    struct bitwise_shl 
    {
        template <typename A, typename B>
        using f = typename C::template f<integral_constant<decltype(A::value << B::value), (A::value << B::value)>>;
    };

    namespace eager 
    {
        template <typename A, typename B>
        using bitwise_shl = integral_constant<decltype(A::value << B::value), (A::value << B::value)>;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_FUNCTIONS_BITWISE_SHL
