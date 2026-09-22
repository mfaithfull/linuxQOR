// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEST_MPL_FUNCTIONS_COMPARISON_GREATERTHANOREQUAL
#define QOR_PP_H_TEST_MPL_FUNCTIONS_COMPARISON_GREATERTHANOREQUAL

#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/functions/comparison/greaterthanorequal.h"
#include "src/qor/essentials/mpl/int.h"

namespace {

	struct greater_than_or_equal_test 
    {
		greater_than_or_equal_test() 
        {
			using namespace qor::mpl;

			eager::greater_than_or_equal<int_<1>, int_<1>>{} = bool_<true>{};
			call<greater_than_or_equal<>, int_<1>, int_<1>>{} = bool_<true>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_FUNCTIONS_COMPARISON_GREATERTHANOREQUAL
