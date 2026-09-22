// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_SAMEAS
#define QOR_PP_H_MPL_SAMEAS

#include <type_traits>
#include "functional/identity.h"

namespace qor { namespace mpl {
		
    template <typename M, typename C = identity>
    struct same_as 
    {
        template <typename T>
        using f = typename C::template f<std::is_same<M, T>>;
    };

    template <typename M>
    struct same_as<M, identity> 
    {
        template <typename T>
        using f = std::is_same<M, T>;
    };

}}//qor::mpl

#endif//QOR_PP_H_MPL_SAMEAS
