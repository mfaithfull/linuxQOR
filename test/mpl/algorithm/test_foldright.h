#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_FOLDRIGHT
#define QOR_PP_H_TEST_MPL_ALGORITHM_FOLDRIGHT

#include <type_traits>

#include "src/qor/essentials/mpl/algorithm/foldright.h"
#include "src/qor/essentials/mpl/functional/bind.h"
#include "src/qor/essentials/mpl/functions/arithmetic/plus.h"
#include "src/qor/essentials/mpl/int.h"
#include "src/qor/essentials/mpl/list.h"

namespace 
{
	using namespace qor::mpl;

	template <typename T1, typename T2>
	using add = uint_<(T1::value + T2::value)>;

	template <typename T, typename U>
	struct push;

	template <typename... Ts, typename U>
	struct push<list<Ts...>, U> 
    {
		using type = list<U, Ts...>;
	};

	template <typename C, typename T>
	struct fold_right_foo 
    {
		template <typename... Ts>
		using f = call<C, Ts..., T>;
	};

	template <typename T>
	struct fold_right_cont_test 
    {
		template <typename... Ts>
		using f = typename dcall<fold_right<cfe<add>>, sizeof...(Ts)>::template f<uint_<1>, Ts...>;
	};

	struct fold_right_test 
    {
		fold_right_test() 
        {
			eager::fold_right<list<uint_<1>, uint_<2>, uint_<3>, uint_<4>>, uint_<0>, add>{} = uint_<10>{};

			call<fold_right<cfl<push>>, list<>, uint_<1>, uint_<2>>{} = list<uint_<1>, uint_<2>>{};
			call<fold_right<cfl<push>>, list<>, uint_<1>, uint_<2>, uint_<3>>{} =
			    list<uint_<1>, uint_<2>, uint_<3>>{};
			call<fold_right<cfl<push>>, list<>, uint_<1>, uint_<2>, uint_<3>, uint_<4>>{} =
			    list<uint_<1>, uint_<2>, uint_<3>, uint_<4>>{};
			call<fold_right<cfl<push>>, list<>, uint_<1>, uint_<2>, uint_<3>, uint_<4>,uint_<5>>{} = 
                list<uint_<1>, uint_<2>, uint_<3>, uint_<4>, uint_<5>>{};
			call<fold_right<cfl<push>>, list<>, uint_<1>, uint_<2>, uint_<3>, uint_<4>, uint_<5>, uint_<6>>{} = 
                list<uint_<1>, uint_<2>, uint_<3>, uint_<4>, uint_<5>, uint_<6>>{};
			call<fold_right<cfl<push>>, list<>, uint_<1>, uint_<2>, uint_<3>, uint_<4>, uint_<5>, uint_<6>, uint_<7>>{} =
			    list<uint_<1>, uint_<2>, uint_<3>, uint_<4>, uint_<5>, uint_<6>, uint_<7>>{};

			call<call<fold_right<cfe<fold_right_foo>>, listify, char, short, int>, void>{} = list<void, char, short, int>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_FOLDRIGHT