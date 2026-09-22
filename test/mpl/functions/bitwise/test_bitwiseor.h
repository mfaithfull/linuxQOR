#ifndef QOR_PP_H_TEST_MPL_FUNCTIONS_BITWISE_OR
#define QOR_PP_H_TEST_MPL_FUNCTIONS_BITWISE_OR

#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/functions/bitwise/bitwiseor.h"
#include "src/qor/essentials/mpl/int.h"
#include "src/qor/essentials/mpl/integralconstant.h"

namespace {

	struct bitwise_or_test 
    {
		bitwise_or_test() 
        {
			using namespace qor::mpl;

			eager::bitwise_or<uint_<0>, uint_<1>>{} = integral_constant<unsigned long long, 1>{};
			call<bitwise_or<>, uint_<0>, uint_<1>>{} = integral_constant<unsigned long long, 1>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_FUNCTIONS_BITWISE_OR
