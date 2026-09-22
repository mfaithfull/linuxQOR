#ifndef QOR_PP_H_TEST_MPL_SEQUENCE_SIZE
#define QOR_PP_H_TEST_MPL_SEQUENCE_SIZE

#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/sequence/size.h"

namespace {
	struct size_test 
    {
		size_test() 
        {
			using namespace qor::mpl;

			call<size<>>{} = uint_<0>{};
			call<size<>, int>{} = uint_<1>{};
			call<size<>, int, int>{} = uint_<2>{};
			call<size<>, int, int, int>{} = uint_<3>{};
			call<size<>, int, int, int, int>{} = uint_<4>{};
			call<size<>, int, int, int, int, int>{} = uint_<5>{};
			call<size<>, int, int, int, int, int, int>{} = uint_<6>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_SEQUENCE_SIZE
