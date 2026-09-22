#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_ZIPWITH
#define QOR_PP_H_TEST_MPL_ALGORITHM_ZIPWITH

#include "src/qor/essentials/mpl/algorithm/zipwith.h"
#include "src/qor/essentials/mpl/functional/bind.h"

namespace 
{
	namespace mpl      = qor::mpl;
	using zip_a        = mpl::list<void, char, short, int>;
	using zip_b        = mpl::list<int, void, char, short>;
	using zip_shoud_be = mpl::list<mpl::list<void, int>, mpl::list<char, void>,mpl::list<short, char>, mpl::list<int, short>>;
	static_assert(std::is_same<mpl::eager::zip_with<mpl::list, zip_a, zip_b>, zip_shoud_be>::value,"");
	static_assert(std::is_same<mpl::call<mpl::zip_with<mpl::cfe<mpl::list>>, zip_a, zip_b>,zip_shoud_be>::value,"");
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_ZIPWITH
