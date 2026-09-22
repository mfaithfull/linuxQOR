#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_ALL
#define QOR_PP_H_TEST_MPL_ALGORITHM_ALL

#include "src/qor/essentials/mpl/algorithm/all.h"
#include "src/qor/essentials/mpl/list.h"
#include "src/qor/essentials/mpl/sameas.h"

namespace 
{
	struct all_test 
    {
		template <typename T>
		using comp = std::is_same<int, T>;

		all_test() 
        {
			using namespace qor;
			using namespace mpl;

			call<all<cfe<comp>>, int, int, int, int>{}  = true_{};
			call<all<cfe<comp>>, int, int, bool, int>{} = false_{};

			call<all<cfe<comp>>, int, int, int, int>{}  = true_{};
			call<all<cfe<comp>>, int, int, bool, int>{} = false_{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_ALL
