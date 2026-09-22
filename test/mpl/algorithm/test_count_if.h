#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_COUNTIF
#define QOR_PP_H_TEST_MPL_ALGORITHM_COUNTIF

#include <type_traits>
#include "src/qor/essentials/mpl/algorithm/countif.h"

namespace {
	struct count_if_test 
    {
		template <typename t>
		using comp = std::is_same<int, t>;

		count_if_test() 
        {
			using namespace qor::mpl;
			call<count_if<cfe<comp>>, void, char, bool, float>{} = uint_<0>{};
			call<count_if<cfe<comp>>, void, char, int, float>{}  = uint_<1>{};
			call<count_if<cfe<comp>>, int, char, int, float>{}   = uint_<2>{};
			call<count_if<cfe<comp>>>{} = uint_<0>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_COUNTIF