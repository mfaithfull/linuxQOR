#ifndef QOR_PP_H_TEST_MPL_SEQUENCE_MAKESEQUENCE
#define QOR_PP_H_TEST_MPL_SEQUENCE_MAKESEQUENCE

#include "src/qor/essentials/mpl/sequence/makesequence.h"
#include "src/qor/essentials/mpl/int.h"
#include "src/qor/essentials/mpl/list.h"

namespace sequence_test {

	using namespace qor::mpl;

	list<> t1         = call<make_int_sequence<>, uint_<0>>{};
	list<uint_<0>> t2 = call<make_int_sequence<>, uint_<1>>{};
	list<uint_<0>, uint_<1>> t3 = call<make_int_sequence<>, uint_<2>>{};
	list<uint_<0>, uint_<1>, uint_<2>> t4 = call<make_int_sequence<>, uint_<3>>{};
	list<uint_<0>, uint_<1>, uint_<2>, uint_<3>> t5 = call<make_int_sequence<>, uint_<4>>{};
	list<uint_<0>, uint_<1>, uint_<2>, uint_<3>, uint_<4>> t6 =
	        call<make_int_sequence<>, uint_<5>>{};
	list<uint_<0>, uint_<1>, uint_<2>, uint_<3>, uint_<4>, uint_<5>> t7 =
	        call<make_int_sequence<>, uint_<6>>{};
	list<uint_<0>, uint_<1>, uint_<2>, uint_<3>, uint_<4>, uint_<5>, uint_<6>> t8 =
	        call<make_int_sequence<>, uint_<7>>{};
	list<uint_<0>, uint_<1>, uint_<2>, uint_<3>, uint_<4>, uint_<5>, uint_<6>, uint_<7>> t9 =
	        call<make_int_sequence<>, uint_<8>>{};
	list<uint_<0>, uint_<1>, uint_<2>, uint_<3>, uint_<4>, uint_<5>, uint_<6>, uint_<7>, uint_<8>>
	        t10 = call<make_int_sequence<>, uint_<9>>{};
	list<uint_<0>, uint_<1>, uint_<2>, uint_<3>, uint_<4>, uint_<5>, uint_<6>, uint_<7>, uint_<8>,
	     uint_<9>>
	        t11 = call<make_int_sequence<>, uint_<10>>{};
}

#endif//QOR_PP_H_TEST_MPL_SEQUENCE_MAKESEQUENCE
