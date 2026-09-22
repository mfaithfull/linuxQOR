#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_FILTER
#define QOR_PP_H_TEST_MPL_ALGORITHM_FILTER

#include "src/qor/essentials/mpl/algorithm/filter.h"
#include "src/qor/essentials/mpl/bool.h"
#include "src/qor/essentials/mpl/int.h"

namespace {

	using namespace qor::mpl;

	template <typename T>
	using less_than_5 = bool_<(T::value < 5)>;

	struct filter_test 
    {
		filter_test() 
        {
			call<filter<cfe<less_than_5>>, int_<1>, int_<2>, int_<7>, int_<8>>{} = list<int_<1>, int_<2>>{};
			call<filter<cfe<less_than_5>>, int_<9>, int_<2>, int_<7>, int_<8>>{} = list<int_<2>>{};
			call<filter<cfe<less_than_5>>>{} = list<>{};
			call<filter<cfe<less_than_5>, cfe<list>>, int_<1>, int_<2>, int_<7>, int_<8>>{} = list<int_<1>, int_<2>>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_FILTER
