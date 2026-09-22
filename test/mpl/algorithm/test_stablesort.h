#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_STABLESORT
#define QOR_PP_H_TEST_MPL_ALGORITHM_STABLESORT

#include <type_traits>
#include "src/qor/essentials/mpl/algorithm/stablesort.h"
#include "src/qor/essentials/mpl/int.h"
#include "src/qor/essentials/mpl/list.h"

namespace stable_sort_test 
{
	namespace mpl = qor::mpl;

	template <typename A, typename B>
	struct equal 
    {
		constexpr static bool value = false;
	};

	template <typename A, typename B>
	struct less 
    {
		constexpr static bool value = A::value < B::value;
	};

	using mpl::int_;
	using ss_test_list = mpl::list<int_<3>, int_<0>, int_<7>, int_<2>, int_<1>, int_<3>, int_<4>, int_<6>, int_<5>>;
	using ss_expect_list = mpl::list<int_<0>, int_<1>, int_<2>, int_<3>, int_<3>, int_<4>, int_<5>, int_<6>, int_<7>>;

	static_assert(std::is_same<ss_test_list, mpl::eager::stable_sort<ss_test_list, equal>>::value, "Stable sort is not stable");
	static_assert(std::is_same<ss_expect_list, mpl::eager::stable_sort<ss_test_list, less>>::value, "Stable sort is not sort");

	using empty_ss_test = mpl::eager::stable_sort<mpl::list<>>;
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_STABLESORT
