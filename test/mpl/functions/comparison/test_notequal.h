// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEST_MPL_FUNCTIONS_COMPARISON_NOTEQUAL
#define QOR_PP_H_TEST_MPL_FUNCTIONS_COMPARISON_NOTEQUAL

#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/functions/comparison/notequal.h"
#include "src/qor/essentials/mpl/int.h"

namespace 
{
	struct not_equal_test 
    {
		not_equal_test() 
        {
			using namespace qor::mpl;

			eager::not_equal<uint_<0>, uint_<1>>{} = bool_<true>{};
			call<not_equal<>, uint_<0>, uint_<1>>{} = bool_<true>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_FUNCTIONS_COMPARISON_NOTEQUAL
