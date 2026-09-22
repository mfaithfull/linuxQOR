// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_FUNCTIONS_BITWISE_COMPLEMENT
#define QOR_PP_H_MPL_FUNCTIONS_BITWISE_COMPLEMENT

#include "../../functional/identity.h"
#include "../../integralconstant.h"

namespace qor { namespace mpl {

		template <typename C = identity>
		struct bitwise_complement 
        {
			template <typename A>
			using f = typename C::template f<integral_constant<decltype(~A::value), (~A::value)>>;
		};

		namespace eager 
        {
			template <typename A>
			using bitwise_complement = integral_constant<decltype(~A::value), (~A::value)>;
		}

}}//qor::mpl

#endif//QOR_PP_H_MPL_FUNCTIONS_BITWISE_COMPLEMENT
