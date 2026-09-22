// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_ALGORITHM_FILTER
#define QOR_PP_H_MPL_ALGORITHM_FILTER

#include "removeif.h"
#include "../functional/identity.h"

namespace qor { namespace mpl {

		// Removes all elements in a list for which a predicate does not hold
		// \effects calls `C` with all elements in the input pack for which the provided predicate
		// `F` holds. \requires Type `F` shall be a `ContinuationPredicate` and C shall be any
		// `Continuation`. example call<filter<same_as<int>,cfe<std::tuple>>,void,int,char>
		// resolves to std::tuple<int>.
		template <typename F = identity, typename C = listify>
		using filter = transform<list_wrap_if<F>, join<C>>;

		namespace eager 
        {
			template <typename List, template <typename...> class F = identity>
			using filter = call<unpack<mpl::filter<cfe<F>>>, List>;
		}

}}//qor::mpl

#endif//QOR_PP_H_MPL_ALGORITHM_FILTER
