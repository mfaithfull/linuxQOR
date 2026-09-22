// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_SEQUENCE_FRONT
#define QOR_PP_H_MPL_SEQUENCE_FRONT

#include "../functional/identity.h"
#include "../list.h"

namespace qor { namespace mpl {

    template <typename C = identity>
    struct front 
    {
        template <typename T, typename... Ts>
        using f = typename C::template f<T>;
    };

    template <>
    struct front<identity> 
    {
        template <typename T, typename... Ts>
        using f = T;
    };

}}//qor::mpl

#endif//QOR_PP_H_MPL_SEQUENCE_FRONT