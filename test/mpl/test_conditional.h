// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/bool.h"
#include "src/qor/essentials/mpl/conditional.h"

namespace {

	struct conditional_test 
    {
		conditional_test() 
        {
			using namespace qor::mpl;

			call<conditional<true>, bool_<true>, bool_<false>>{}  = bool_<true>{};
			call<conditional<false>, bool_<true>, bool_<false>>{} = bool_<false>{};
		}
	};
}
