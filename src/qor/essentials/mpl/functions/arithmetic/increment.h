// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_FUNCTIONS_ARITH_INCREMENT
#define QOR_PP_H_MPL_FUNCTIONS_ARITH_INCREMENT

#include "../../functional/identity.h"
#include "../../integralconstant.h"

namespace qor { namespace mpl {

    template <typename C = identity>
    struct increment 
    {
        template <typename A>
        using f = typename C::template f<integral_constant<decltype(A::value + 1), (A::value + 1)>>;
    };

    namespace eager 
    {
        template <typename A>
        using increment = integral_constant<decltype(A::value + 1), (A::value + 1)>;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_FUNCTIONS_ARITH_INCREMENT
