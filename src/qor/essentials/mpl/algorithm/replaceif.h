// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_ALGORITHM_REPLACEIF
#define QOR_PP_H_MPL_ALGORITHM_REPLACEIF

#include "transform.h"
#include "../functional/call.h"
#include "../functional/flow.h"
#include "../functional/identity.h"
#include "../sequence/join.h"
#include "../always.h"
#include "../conditional.h"

namespace qor { namespace mpl {

    //Output is equivalent to the input parameter pack with every element
    // replaced with Input where the predicate F holds.
    template <typename Input, typename F = identity, typename C = listify>
    using replace_if = transform<if_<F, always<Input>, identity>, C>;

    namespace eager 
    {
        template <typename List, typename Input, template <typename...> class Cond = identity>
        using replace_if = call<unpack<mpl::replace_if<Input, cfe<Cond>>>, List>;
    }

}}//qor::mpl

#endif//QOR_PP_H_MPL_ALGORITHM_REPLACEIF
