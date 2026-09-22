#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_NONE
#define QOR_PP_H_TEST_MPL_ALGORITHM_NONE

#include <type_traits>
#include "src/qor/essentials/mpl/algorithm/none.h"
#include "src/qor/essentials/mpl/functional/bind.h"
#include "src/qor/essentials/mpl/list.h"

namespace 
{
	template <typename T>
	using comp = std::is_same<int, T>;

	using namespace qor;
	static_assert(!mpl::eager::none<mpl::list<void, char, int, float>, comp>::value, "");
	static_assert(mpl::eager::none<mpl::list<void, char, bool, float>, comp>::value, "");
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_NONE
