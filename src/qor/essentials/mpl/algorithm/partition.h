// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_ALGORITHM_PARTITION
#define QOR_PP_H_MPL_ALGORITHM_PARTITION

#include "filter.h"
#include "removeif.h"
#include "../functional/fork.h"
#include "../functional/identity.h"

namespace qor { namespace mpl {

    //Returns a list containing two lists, one with all elements for which the provided predicate holds and one with all the rest
    template <typename F = identity, typename C = listify>
    using partition = fork<remove_if<F>, filter<F>, C>;
    namespace eager 
    {
        template <typename List, template <typename...> class Cond = identity>
        using partition = call<unpack<mpl::partition<cfe<Cond>>>, List>;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_ALGORITHM_PARTITION
