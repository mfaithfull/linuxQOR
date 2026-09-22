#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_FINDIF
#define QOR_PP_H_TEST_MPL_ALGORITHM_FINDIF

#include "src/qor/essentials/mpl/algorithm/findif.h"
#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/sameas.h"

namespace 
{
	struct find_if_test 
    {
		find_if_test() {
			using namespace qor::mpl;

			call<find_if<same_as<char>>, void, char, short, int>{} = list<char, short, int>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_FINDIF