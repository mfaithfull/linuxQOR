// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_FUNCTIONS_LOGIC_NOT
#define QOR_PP_H_MPL_FUNCTIONS_LOGIC_NOT

#include "../../functional/identity.h"
#include "../../bool.h"

namespace qor { namespace mpl {

    template <typename C = identity>
    struct logical_not 
    {
        template <typename A>
        using f = typename C::template f<bool_<(!A::value)>>;
    };

    namespace eager 
    {
        template <typename A>
        using logical_not = bool_<(!A::value)>;
    }
    
}}//qor::mpl

#endif//QOR_PP_H_MPL_FUNCTIONS_LOGIC_NOT
