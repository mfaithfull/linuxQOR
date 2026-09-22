#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_SPLITIF
#define QOR_PP_H_TEST_MPL_ALGORITHM_SPLITIF

#include "src/qor/essentials/mpl/algorithm/splitif.h"
#include "src/qor/essentials/mpl/bool.h"
#include "src/qor/essentials/mpl/int.h"
#include "src/qor/essentials/mpl/list.h"

namespace split_if_test 
{
	using namespace qor::mpl;

	template <typename T>
	using is_99 = bool_<T::value == 99>;

	using a = eager::split_if<list<int_<1>, int_<2>, int_<99>, int_<3>, int_<4>, int_<99>, int_<5>>, is_99>;
	using b = list<list<int_<1>, int_<2>>, list<int_<3>, int_<4>>, list<int_<5>>>;
	static_assert(std::is_same<a, b>{}, "");
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_SPLITIF
