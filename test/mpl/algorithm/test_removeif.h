#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_REMOVEIF
#define QOR_PP_H_TEST_MPL_ALGORITHM_REMOVEIF

#include <type_traits>
#include "src/qor/essentials/mpl/algorithm/removeif.h"
#include "src/qor/essentials/mpl/bool.h"
#include "src/qor/essentials/mpl/int.h"

namespace remove_if_test 
{
	using namespace qor::mpl;

	template <typename T>
	using less_than_5 = bool_<(T::value < 5)>;

	struct less_than_5_c 
    {
		template <typename T>
		using f = less_than_5<T>;
	};

	static_assert(std::is_same<eager::remove_if<list<int_<1>, int_<2>, int_<7>, int_<8>>, less_than_5>, list<int_<7>, int_<8>>>::value,"");

	template <typename... Ts>
	using test = call<remove_if<less_than_5_c>, Ts...>;

	static_assert(std::is_same<test<int_<1>, int_<2>, int_<7>, int_<8>>, list<int_<7>, int_<8>>>::value,"");
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_REMOVEIF
