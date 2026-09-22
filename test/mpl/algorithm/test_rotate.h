#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_ROTATE
#define QOR_PP_H_TEST_MPL_ALGORITHM_ROTATE

#include "src/qor/essentials/mpl/algorithm/rotate.h"
#include "src/qor/essentials/mpl/sequence/drop.h"
#include "src/qor/essentials/mpl/sequence/join.h"
#include "src/qor/essentials/mpl/sequence/makesequence.h"

namespace 
{
	struct rotate_test 
    {
		template <int n>
		void rotate_test_impl() 
        {
			using namespace qor::mpl;
			constexpr int len = 522;
			using seq         = eager::make_int_sequence<int_<len>>;
			using left_seq    = eager::make_int_sequence<int_<n>>;
			using right_seq   = eager::drop<seq, n>;
			eager::join<right_seq, left_seq>{} = eager::rotate<seq, n>{};
		}

		rotate_test() 
        {
			rotate_test_impl<0>();
			rotate_test_impl<1>();
			rotate_test_impl<2>();
			rotate_test_impl<3>();
			rotate_test_impl<4>();
			rotate_test_impl<5>();
			rotate_test_impl<6>();
			rotate_test_impl<7>();
			rotate_test_impl<8>();
			rotate_test_impl<9>();
			rotate_test_impl<15>();
			rotate_test_impl<16>();
			rotate_test_impl<17>();
			rotate_test_impl<31>();
			rotate_test_impl<32>();
			rotate_test_impl<33>();
			rotate_test_impl<63>();
			rotate_test_impl<64>();
			rotate_test_impl<65>();
			rotate_test_impl<127>();
			rotate_test_impl<128>();
			rotate_test_impl<129>();
			rotate_test_impl<255>();
			rotate_test_impl<256>();
			rotate_test_impl<257>();
			rotate_test_impl<511>();
			rotate_test_impl<512>();
			rotate_test_impl<513>();
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_ROTATE
