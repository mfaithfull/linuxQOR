#ifndef QOR_PP_H_TEST_MPL_FUNCTIONS_BITWISE_COMPLIMENT
#define QOR_PP_H_TEST_MPL_FUNCTIONS_BITWISE_COMPLIMENT

#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/functions/bitwise/bitwisecomplement.h"
#include "src/qor/essentials/mpl/int.h"
#include "src/qor/essentials/mpl/integralconstant.h"

namespace {

	struct bitwise_complement_test 
    {
		bitwise_complement_test() 
        {
			using namespace qor::mpl;

			eager::bitwise_complement<uint_<0>>{} = integral_constant<unsigned long long, (~0ull)>{};
			call<bitwise_complement<>, uint_<0>>{} = integral_constant<unsigned long long, (~0ull)>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_FUNCTIONS_BITWISE_COMPLIMENT
