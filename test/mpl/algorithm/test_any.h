#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_ANY
#define QOR_PP_H_TEST_MPL_ALGORITHM_ANY

#include <type_traits>

#include "src/qor/essentials/mpl/algorithm/any.h"
#include "src/qor/essentials/mpl/functional/bind.h"
#include "src/qor/essentials/mpl/list.h"

namespace 
{
	struct any_test 
    {
		template <typename T>
		using comp = std::is_same<int, T>;

		any_test() 
        {
			using namespace qor::mpl;
			call<any<cfe<comp>>, void, char, int, float>{}  = true_{};
			call<any<cfe<comp>>, void, char, bool, float>{} = false_{};
			call<any<cfe<comp>>>{} = false_{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_ANY