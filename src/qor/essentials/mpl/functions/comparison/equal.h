// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_FUNCTIONS_COMPARISON_EQUAL
#define QOR_PP_H_MPL_FUNCTIONS_COMPARISON_EQUAL

#include "../../functional/identity.h"
#include "../../bool.h"

namespace qor { namespace mpl {

    template <typename C = identity>
    struct equal 
    {
        template <typename A, typename B>
        using f = typename C::template f<bool_<(A::value == B::value)>>;
    };

    namespace eager 
    {
        template <typename A, typename B>
        using equal = bool_<(A::value == B::value)>;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_FUNCTIONS_COMPARISON_EQUAL
