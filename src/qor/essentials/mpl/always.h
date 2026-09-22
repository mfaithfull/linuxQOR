// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_ALWAYS
#define QOR_PP_H_MPL_ALWAYS

#include "functional/identity.h"
#include "bool.h"

namespace qor { namespace mpl {

    //Ignores input and passes a fixed output		
    template <typename T, typename C = identity>
    struct always 
    {
        template <typename...>
        using f = typename C::template f<T>;
    };
    
    template <typename T>
    struct always<T, identity> 
    {
        template <typename...>
        using f = T;
    };

    namespace eager 
    {
        template <typename...>
        struct always_false 
        {
            static constexpr bool value = false;
        };
    }//eager

}}//qor::mpl

#endif//QOR_PP_H_MPL_ALWAYS
