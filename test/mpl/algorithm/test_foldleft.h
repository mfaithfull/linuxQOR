#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_FOLDLEFT
#define QOR_PP_H_TEST_MPL_ALGORITHM_FOLDLEFT

#include <type_traits>

#include "src/qor/essentials/mpl/algorithm/foldleft.h"
#include "src/qor/essentials/mpl/int.h"

namespace 
{
	using namespace qor;
	using mpl::uint_;

	template <typename T1, typename T2>
	using add = uint_<(T1::value + T2::value)>;

	static_assert(std::is_same<mpl::eager::fold_left<mpl::list<uint_<1>, uint_<2>, uint_<3>, uint_<4>>, uint_<0>, add>, uint_<10>>::value, "");
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_FOLDLEFT
