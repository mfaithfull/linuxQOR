#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_GROUP
#define QOR_PP_H_TEST_MPL_ALGORITHM_GROUP

#include "src/qor/essentials/mpl/algorithm/group.h"
#include "src/qor/essentials/mpl/sequence/makesequence.h"

namespace 
{
	struct group_test 
    {
		group_test() 
        {
			using namespace qor::mpl;
			call<group<>>{} = list<>{};
			call<group<>, int>{} = list<list<int>>{};
			call<group<>, int, double>{} = list<list<int>, list<double>>{};
			call<group<>, int, double, float>{} = list<list<int>, list<double>, list<float>>{};
			call<group<>, int, int, double, double, double, int, int, int, float>{} =
			    list<list<int, int>, list<double, double, double>, list<int, int, int>, list<float>>{};
			using seq = call<make_int_sequence<>, uint_<5>>;
			call<unpack<group<>>, seq>{} = call<unpack<transform<cfe<list>>>, seq>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_GROUP