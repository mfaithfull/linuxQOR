// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEST_MPL_FUNCTIONS_COMPARISON_LESSTHAN
#define QOR_PP_H_TEST_MPL_FUNCTIONS_COMPARISON_LESSTHAN

#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/functions/comparison/lessthan.h"
#include "src/qor/essentials/mpl/int.h"

namespace 
{
	struct less_than_test 
    {
		less_than_test() 
        {
			using namespace qor::mpl;

			eager::less_than<int_<0>, int_<1>>{} = bool_<true>{};
			call<less_than<>, int_<0>, int_<1>>{} = bool_<true>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_FUNCTIONS_COMPARISON_LESSTHAN
