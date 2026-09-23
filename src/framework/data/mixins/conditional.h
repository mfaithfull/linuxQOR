// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_MIXINS_CONDITIONAL
#define QOR_PP_H_FRAMEWORK_MIXINS_CONDITIONAL

#include <tuple>
#include "foreach.h"

namespace qor{ namespace mix{

    template <bool>
    struct conditional;

    template <>
    struct conditional<true> 
    {
        template <typename A, typename B>
        using f = A;
    };

    template <>
    struct conditional<false> 
    {
        template <typename A, typename B>
        using f = B;
    };

}}//qor::mix

#endif//QOR_PP_H_FRAMEWORK_MIXINS_CONDITIONAL