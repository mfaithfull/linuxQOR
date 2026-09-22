#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_PRODUCT
#define QOR_PP_H_TEST_MPL_ALGORITHM_PRODUCT

#include <type_traits>
#include "src/qor/essentials/mpl/algorithm/product.h"
#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/list.h"

namespace product_test 
{
	namespace mpl = qor::mpl;

	template <typename...>
	struct foo_list {};
	
    struct foo_listify 
    {
		template <typename... Ts>
		using f = foo_list<Ts...>;
	};

	template <typename...>
	struct bar_list {};

	struct bar_listify 
    {
		template <typename... Ts>
		using f = bar_list<Ts...>;
	};

	static_assert(std::is_same<mpl::call<mpl::product<foo_listify, bar_listify>,mpl::list<void, char>, mpl::list<void, char>>, bar_list<foo_list<void, void>, foo_list<void, char>, foo_list<char, void>, foo_list<char, char>>>::value, "");
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_PRODUCT
