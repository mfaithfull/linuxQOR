// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_FUNCTIONS_COMPARISON_LESSTHANOREQUAL
#define QOR_PP_H_MPL_FUNCTIONS_COMPARISON_LESSTHANOREQUAL

#include "../../functional/identity.h"
#include "../../bool.h"

namespace qor { namespace mpl {

    template <typename C = identity>
    struct less_than_or_equal 
    {
        template <typename A, typename B>
        using f = typename C::template f<bool_<(A::value <= B::value)>>;
    };

    namespace eager 
    {
        template <typename A, typename B>
        using less_than_or_equal = bool_<(A::value <= B::value)>;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_FUNCTIONS_COMPARISON_LESSTHANOREQUAL
