#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_PARTITION
#define QOR_PP_H_TEST_MPL_ALGORITHM_PARTITION

#include <type_traits>

#include "src/qor/essentials/mpl/algorithm/partition.h"
#include "src/qor/essentials/mpl/bool.h"
#include "src/qor/essentials/mpl/int.h"

namespace partition_test 
{
	using namespace qor::mpl;

	template <typename T>
	using less_than_5 = bool_<(T::value < 5)>;

	using l           = list<int_<1>, int_<2>, int_<7>, int_<8>>;
	using p           = eager::partition<l, less_than_5>;
	static_assert(std::is_same<p, list<list<int_<7>, int_<8>>, list<int_<1>, int_<2>>>>::value, "partition error");
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_PARTITION
