// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_FUNCTIONAL_COMPOSE
#define QOR_PP_H_MPL_FUNCTIONAL_COMPOSE

#include "bind.h"
#include "identity.h"

namespace qor { namespace mpl { namespace eager { namespace impl {

            template <template <typename...> class F1, template <typename...> class F2>
            struct compose_impl 
            {
                template <typename... Ts>
                using f = F2<F1<Ts...>>;
            };

            template <template <typename...> class F, template <typename...> class... Fs>
            struct compose_n_impl 
            {
                template <template <typename...> class Result>
                using f = typename compose_n_impl<Fs...>::template f<compose_impl<Result, F>::f>;
            };

            template <template <typename...> class F>
            struct compose_n_impl<F> 
            {
                template <template <typename...> class Result>
                using f = compose_impl<Result, F>;
            };
        }//impl

        template <template <typename...> class F, template <typename...> class... Fs>
        using compose = typename impl::compose_n_impl<Fs...>::template f<F>;

    }//eager

}}//qor::mpl

#endif//QOR_PP_H_MPL_FUNCTIONAL_COMPOSE