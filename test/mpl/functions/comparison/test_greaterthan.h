// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEST_MPL_FUNCTIONS_COMPARISON_GREATERTHAN
#define QOR_PP_H_TEST_MPL_FUNCTIONS_COMPARISON_GREATERTHAN

#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/functions/comparison/greaterthan.h"
#include "src/qor/essentials/mpl/int.h"

namespace {
	struct greater_than_test 
    {
		greater_than_test() 
        {
			using namespace qor::mpl;

			eager::greater_than<int_<0>, int_<1>>{} = bool_<false>{};
			call<greater_than<>, int_<0>, int_<1>>{} = bool_<false>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_FUNCTIONS_COMPARISON_GREATERTHAN
