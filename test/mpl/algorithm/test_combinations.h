#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_COMBINATIONS
#define QOR_PP_H_TEST_MPL_ALGORITHM_COMBINATIONS

#include <type_traits>
#include "src/qor/essentials/mpl/mpl.h"

namespace 
{
	struct combinations_test 
    {
		combinations_test() 
        {
			using namespace qor::mpl;

			call<combinations<uint_<2>, listify, front<>, listify>, uint_<0>, uint_<1>,
			     uint_<2>>{} = list<list<uint_<0>, uint_<1>>, list<uint_<0>, uint_<2>>,
			                        list<uint_<1>, uint_<0>>, list<uint_<1>, uint_<2>>,
			                        list<uint_<2>, uint_<0>>, list<uint_<2>, uint_<1>>>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_COMBINATIONS
