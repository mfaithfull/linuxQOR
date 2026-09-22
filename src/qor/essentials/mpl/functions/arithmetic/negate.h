// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_FUNCTIONS_ARITH_NEGATE
#define QOR_PP_H_MPL_FUNCTIONS_ARITH_NEGATE

#include "../../functional/identity.h"
#include "../../integralconstant.h"

namespace qor { namespace mpl {

    template <typename C = identity>
    struct negate 
    {
        template <typename A>
        using f = typename C::template f<integral_constant<decltype(-A::value), (-A::value)>>;
    };

    namespace eager 
    {
        template <typename A>
        using negate = integral_constant<decltype(-A::value), (-A::value)>;
    }
    
}}//qor::mpl

#endif//QOR_PP_H_MPL_FUNCTIONS_ARITH_NEGATE
