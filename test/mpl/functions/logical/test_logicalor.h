#ifndef QOR_PP_H_TEST_MPL_FUNCTIONS_LOGICAL_OR
#define QOR_PP_H_TEST_MPL_FUNCTIONS_LOGICAL_OR

#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/functions/logical/logicalor.h"
#include "src/qor/essentials/mpl/bool.h"
#include "src/qor/essentials/mpl/int.h"

namespace 
{
	struct logical_or_test 
    {
		logical_or_test() 
        {
			using namespace qor::mpl;

			eager::logical_or<uint_<0>, uint_<1>>{} = bool_<true>{};
			call<logical_or<>, uint_<0>, uint_<1>>{} = bool_<true>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_FUNCTIONS_LOGICAL_OR
