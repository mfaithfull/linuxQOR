#ifndef QOR_PP_H_TEST_MPL_SEQUENCE_JOIN
#define QOR_PP_H_TEST_MPL_SEQUENCE_JOIN

#include "src/qor/essentials/mpl/algorithm/transform.h"
#include "src/qor/essentials/mpl/sequence/join.h"
#include "src/qor/essentials/mpl/sequence/makesequence.h"

namespace 
{
	struct join_test 
    {
		template <int n>
		void join_test_impl() 
        {
			using namespace qor::mpl;
			using seq = call<make_int_sequence<>, int_<n>>;
			seq{}     = call<unpack<transform<listify, join<>>>, seq>{};
		}

		join_test() 
        {
			join_test_impl<0>();
			join_test_impl<1>();
			join_test_impl<2>();
			join_test_impl<3>();
			join_test_impl<4>();
			join_test_impl<5>();
			join_test_impl<6>();
			join_test_impl<7>();
			join_test_impl<8>();
			join_test_impl<9>();
			join_test_impl<15>();
			join_test_impl<16>();
			join_test_impl<17>();
			join_test_impl<31>();
			join_test_impl<32>();
			join_test_impl<33>();
			join_test_impl<63>();
			join_test_impl<64>();
			join_test_impl<65>();
			join_test_impl<127>();
			join_test_impl<128>();
			join_test_impl<129>();
			join_test_impl<255>();
			join_test_impl<256>();
			join_test_impl<257>();
			join_test_impl<527>();
			join_test_impl<528>();
			join_test_impl<529>();
			join_test_impl<1023>();
			join_test_impl<1024>();
			join_test_impl<1025>();
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_SEQUENCE_JOIN
