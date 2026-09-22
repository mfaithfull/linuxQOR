// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_FUNCTIONS_BITWISE_AND
#define QOR_PP_H_MPL_FUNCTIONS_BITWISE_AND

#include "../../functional/identity.h"
#include "../../integralconstant.h"

namespace qor { namespace mpl {

		template <typename C = identity>
		struct bitwise_and 
        {
			template <typename A, typename B>
			using f = typename C::template f<integral_constant<decltype(A::value & B::value), (A::value & B::value)>>;
		};

		namespace eager 
        {
			template <typename A, typename B>
			using bitwise_and = integral_constant<decltype(A::value & B::value), (A::value & B::value)>;
		}
        
}}//qor::mpl

#endif//QOR_PP_H_MPL_FUNCTIONS_BITWISE_AND
