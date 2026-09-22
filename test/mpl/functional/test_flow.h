#ifndef QOR_PP_H_TEST_MPL_FUNCTIONAL_FLOW
#define QOR_PP_H_TEST_MPL_FUNCTIONAL_FLOW

#include "src/qor/essentials/mpl/mpl.h"

namespace {
	using namespace qor;
	using namespace mpl;
	static_assert(call<if_<same_as<void>, always<false_>, always<true_>>, int>::value, "");
}

#endif//QOR_PP_H_TEST_MPL_FUNCTIONAL_FLOW
