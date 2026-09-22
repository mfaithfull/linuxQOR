#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_LOOKUP
#define QOR_PP_H_TEST_MPL_ALGORITHM_LOOKUP

#include <type_traits>
#include "src/qor/essentials/mpl/sequence/lookup.h"

namespace 
{
	namespace mpl = qor::mpl;

	template <int>
	struct x;

	using xs0  = mpl::list<x<1>>;
	using xs1  = mpl::list<x<1>, x<2>>;
	using xs15 = mpl::list<x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>, x<10>, x<11>, x<12>, x<13>, x<14>, x<15>, x<16>>;
	using xs16 = mpl::list<x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>, x<10>, x<11>, x<12>, x<13>, x<14>, x<15>, x<16>, x<17>>;
	using xs17 = mpl::list<x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>, x<10>, x<11>, x<12>, x<13>, x<14>, x<15>, x<16>, x<17>, x<18>>;

	static_assert(std::is_same<mpl::eager::lookup<xs0, 0>, x<1>>::value, "");
	static_assert(std::is_same<mpl::eager::lookup<xs1, 1>, x<2>>::value, "");
	static_assert(std::is_same<mpl::eager::lookup<xs15, 15>, x<16>>::value, "");
	static_assert(std::is_same<mpl::eager::lookup<xs16, 16>, x<17>>::value, "");
	static_assert(std::is_same<mpl::eager::lookup<xs17, 17>, x<18>>::value, "");
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_LOOKUP
