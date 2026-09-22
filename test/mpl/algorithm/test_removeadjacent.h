#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_REMOVEADJACENT
#define QOR_PP_H_TEST_MPL_ALGORITHM_REMOVEADJACENT

#include <type_traits>

#include "src/qor/essentials/mpl/algorithm/removeadjacent.h"
#include "src/qor/essentials/mpl/bool.h"
#include "src/qor/essentials/mpl/list.h"

namespace 
{
	namespace mpl = qor::mpl;
	static_assert(std::is_same<mpl::eager::remove_adjacent<mpl::list<void, char, char, short>>, mpl::list<void, char, short>>{}, "");

	template <typename T1, typename T2>
	struct foo_is_same_impl 
    {
		using f = mpl::bool_<false>;
	};

	template <typename T>
	struct foo_is_same_impl<T, T> 
    {
		using f = mpl::bool_<true>;
	};

	template <typename T1, typename T2>
	using foo_is_same = typename foo_is_same_impl<T1, T2>::f;

	// unoptimised version of remove_adjacent
	static_assert(std::is_same<mpl::eager::remove_adjacent<mpl::list<void, char, char, short>, foo_is_same>, mpl::list<void, char, short>>{}, "");
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_REMOVEADJACENT
