// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_FUNCTIONS_ARITH_MIN
#define QOR_PP_H_MPL_FUNCTIONS_ARITH_MIN

#include "../../functional/identity.h"
#include "../../integralconstant.h"
#include "../../conditional.h"

namespace qor { namespace mpl {

    template <typename C = identity>
    struct min 
    {
        template <typename A, typename B>
        using f = typename C::template f<typename conditional<(A::value < B::value)>::template f<A, B>>;
    };

    namespace eager 
    {
        template <typename A, typename B>
        using min = typename conditional<(A::value < B::value)>::template f<A, B>;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_FUNCTIONS_ARITH_MIN
