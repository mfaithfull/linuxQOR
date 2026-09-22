// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEST_MPL_FUNCTIONS_COMPARISON_EQUAL
#define QOR_PP_H_TEST_MPL_FUNCTIONS_COMPARISON_EQUAL

#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/functions/comparison/equal.h"
#include "src/qor/essentials/mpl/bool.h"
#include "src/qor/essentials/mpl/int.h"

namespace {

	struct equal_test 
    {
		equal_test() 
        {
			using namespace qor::mpl;

			eager::equal<uint_<0>, uint_<1>>{} = bool_<false>{};
			call<equal<>, uint_<0>, uint_<1>>{} = bool_<false>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_FUNCTIONS_COMPARISON_EQUAL
