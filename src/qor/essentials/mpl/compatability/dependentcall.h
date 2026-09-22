// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_COMPAT_DEPENDENTCALL
#define QOR_PP_H_MPL_COMPAT_DEPENDENTCALL

//#include "compatability.hpp"
#include "../conditional.h"

namespace qor { namespace mpl {

    namespace detail 
    {
        template <bool b, typename C>
        struct dependant_impl;

        template <typename C>
        struct dependant_impl<true, C> : C {};
    }//detail

    template <typename C, unsigned size>
    using dcall = typename detail::dependant_impl<static_cast<bool>(size < 100000), C>;

    template <bool>
    struct dcallf;
    
    template <>
    struct dcallf<true> 
    {
        template <template <typename...> class F1, typename... Ts>
        using f1 = F1<Ts...>;
        
        template <template <typename...> class F1, template <typename...> class F2, typename... Ts>
        using f2 = F1<F2<Ts...>>;
    };

    template <>
    struct dcallf<false> 
    {
        template <template <typename...> class F1, typename... Ts>
        using f1 = F1<>;

        template <template <typename...> class F1, template <typename...> class F2, typename... Ts>
        using f2 = F1<F2<>>;
    };

}}//qor::mpl

#endif//QOR_PP_H_MPL_COMPAT_DEPENDENTCALL
