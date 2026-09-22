#ifndef QOR_PP_H_TEST_MPL_SEQUENCE_AT
#define QOR_PP_H_TEST_MPL_SEQUENCE_AT

#include "src/qor/essentials/mpl/sequence/at.h"

namespace {
	using namespace qor::mpl;

	char c1  = call<at<int_<1>>, void, char, short>{};
	char c2  = call<at<int_<7>>, int, int, int, int, int, int, int, char, void, short>{};
	char c3  = call<at<int_<8>>, int, int, int, int, int, int, int, void, char, short>{};
	short c4 = call<at<int_<9>>, int, int, int, int, int, int, int, void, char, short>{};
	char c5  = call<at<int_<17>>, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, void, char, short>{};
}

#endif//QOR_PP_H_TEST_MPL_SEQUENCE_AT
