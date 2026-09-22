#ifndef QOR_PP_H_TEST_MPL_FUNCTIONS_BITWISE_XOR
#define QOR_PP_H_TEST_MPL_FUNCTIONS_BITWISE_XOR

#include <type_traits>
#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/functions/bitwise/bitwisexor.h"
#include "src/qor/essentials/mpl/int.h"
#include "src/qor/essentials/mpl/integralconstant.h"

namespace {

	struct bitwise_xor_test 
    {
		bitwise_xor_test() 
        {
			using namespace qor::mpl;

			eager::bitwise_xor<uint_<0>, uint_<1>>{} = integral_constant<unsigned long long, 1>{};
			call<bitwise_xor<>, uint_<0>, uint_<1>>{} = integral_constant<unsigned long long, 1>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_FUNCTIONS_BITWISE_XOR
