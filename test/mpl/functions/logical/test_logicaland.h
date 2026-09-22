#ifndef QOR_PP_H_TEST_MPL_FUNCTIONS_LOGICAL_AND
#define QOR_PP_H_TEST_MPL_FUNCTIONS_LOGICAL_AND

#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/functions/logical/logicaland.h"
#include "src/qor/essentials/mpl/bool.h"
#include "src/qor/essentials/mpl/int.h"

namespace 
{
	struct logical_and_test 
    {
		logical_and_test() 
        {
			using namespace qor::mpl;
			eager::logical_and<uint_<2>, uint_<1>>{} = bool_<true>{};
			call<logical_and<>, uint_<2>, uint_<1>>{} = bool_<true>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_FUNCTIONS_LOGICAL_AND
