// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEST_MPL_ALWAYS
#define QOR_PP_H_TEST_MPL_ALWAYS

#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/bool.h"
#include "src/qor/essentials/mpl/always.h"

namespace {

    namespace mpl = qor::mpl;

	struct always_test 
    {
		always_test() 
        {
			mpl::call<mpl::always<mpl::bool_<false>>, void>{} = mpl::bool_<false>{};
		}
	};

	template <typename T>
	struct foo 
    {
		static_assert(mpl::eager::always_false<T>::value, "always_false should not have been instantiated");
	};
}

#endif//QOR_PP_H_TEST_MPL_ALWAYSG
