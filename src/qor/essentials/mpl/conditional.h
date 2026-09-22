// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_CONDITIONAL
#define QOR_PP_H_MPL_CONDITIONAL

namespace qor { namespace mpl {

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

}}//qor::mpl

#endif//QOR_PP_H_MPL_CONDITIONAL
