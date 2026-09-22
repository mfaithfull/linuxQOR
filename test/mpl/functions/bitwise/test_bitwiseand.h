#ifndef QOR_PP_H_TEST_MPL_FUNCTIONS_BITWISE_AND
#define QOR_PP_H_TEST_MPL_FUNCTIONS_BITWISE_AND

#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/functions/bitwise/bitwiseand.h"
#include "src/qor/essentials/mpl/int.h"
#include "src/qor/essentials/mpl/integralconstant.h"

namespace {

	struct bitwise_and_test 
    {
		bitwise_and_test() 
        {
			using namespace qor::mpl;

			eager::bitwise_and<uint_<0>, uint_<1>>{} = integral_constant<unsigned long long, 0>{};
			call<bitwise_and<>, uint_<0>, uint_<1>>{} = integral_constant<unsigned long long, 0>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_FUNCTIONS_BITWISE_AND
