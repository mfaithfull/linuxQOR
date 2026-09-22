// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_FUNCTIONS_ARITH_MINUS
#define QOR_PP_H_MPL_FUNCTIONS_ARITH_MINUS

#include "../../functional/identity.h"
#include "../../integralconstant.h"

namespace qor { namespace mpl {

    template <typename C = identity>
    struct minus 
    {
        template <typename A, typename B>
        using f = typename C::template f<integral_constant<decltype(A::value - B::value), (A::value - B::value)>>;
    };

    namespace eager 
    {
        template <typename A, typename B>
        using minus = integral_constant<decltype(A::value - B::value), (A::value - B::value)>;
    }
    
}}//qor::mpl

#endif//QOR_PP_H_MPL_FUNCTIONS_ARITH_MINUS
