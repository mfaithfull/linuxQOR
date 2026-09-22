#ifndef QOR_PP_H_TEST_MPL_FUNCTIONS_LOGICAL_NOT
#define QOR_PP_H_TEST_MPL_FUNCTIONS_LOGICAL_NOT

#include <type_traits>
#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/functions/logical/logicalnot.h"
#include "src/qor/essentials/mpl/bool.h"
#include "src/qor/essentials/mpl/int.h"

namespace 
{
	struct logical_not_test 
    {
		logical_not_test() 
        {
			using namespace qor::mpl;

			eager::logical_not<uint_<0>>{} = bool_<true>{};
			call<logical_not<>, uint_<0>>{} = bool_<true>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_FUNCTIONS_LOGICAL_NOT
