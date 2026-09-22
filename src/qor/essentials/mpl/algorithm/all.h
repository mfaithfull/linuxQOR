// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_ALGORITHM_ALL
#define QOR_PP_H_MPL_ALGORITHM_ALL

#include "findif.h"
#include "../functional/bind.h"
#include "../functional/call.h"
#include "../functional/identity.h"
#include "../bool.h"

#include "../always.h"

namespace qor { namespace mpl { namespace detail {

        template <typename F>
        struct not_ 
        {
            template <typename T>
            using f = bool_<(!(F::template f<T>::value))>;
        };

        template <template <typename...> class F>
        struct not_<cfe<F, identity>> 
        {
            template <typename T>
            using f = bool_<(!F<T>::value)>;
        };
	}//detail

    //Tests if a predicate holds for all elements in a pack
    // \effects resolves to true_ if all elements in the input pack fulfill the provided
    // predicate, otherwise false_. \requires Type `F` shall be a `continuation predicate` and
    // C shall be any `continuation`. example: call<all<same_as<void>>,void,void,void> resolves
    // to true_.
    template <typename F = identity, typename C = identity>
    using all = find_if<detail::not_<F>, always<bool_<false>, C>, always<bool_<true>, C>>;

    namespace eager 
    {
        // resolves to true_ if all elements in the input list fulfill the provided predicate
        template <typename List, template <typename...> class Cond = identity>
        using all = call<unpack<mpl::all<cfe<Cond>>>, List>;
    }//eager

}}//qor::mpl

#endif//QOR_PP_H_MPL_ALGORITHM_ALL
