// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_ALGORITHM_NONE
#define QOR_PP_H_MPL_ALGORITHM_NONE

#include "findif.h"
#include "../functional/identity.h"
#include "../bool.h"
#include "../always.h"

namespace qor { namespace mpl {

    //Resolves to false_ if any element in the input pack fulfills the provided
    // predicate, otherwise true_. \requires Type `F` shall be a `continuation predicate` and
    // C shall be any `continuation`. example call<any<same_as<void>>,void,int,char> resolves
    // to true_.
    template <typename F = identity, typename C = identity>
    using none = find_if<F, always<bool_<false>, C>, always<bool_<true>, C>>;

    namespace eager 
    {
        template <typename List, template <typename...> class Cond = identity>
        using none = call<unpack<mpl::none<cfe<Cond>>>, List>;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_ALGORITHM_NONE

