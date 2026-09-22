#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_FLATTEN
#define QOR_PP_H_TEST_MPL_ALGORITHM_FLATTEN

#include <type_traits>

#include "src/qor/essentials/mpl/algorithm/flatten.h"
#include "src/qor/essentials/mpl/functional/call.h"
#include "src/qor/essentials/mpl/list.h"

namespace 
{
	using namespace qor::mpl;
	list<> t1 = call<flatten<>, list<list<list<>, list<>>, list<>>, list<>>{};
	
	template<typename...>
	struct foo_list 
    {
	};

	list<> t2 = call<flatten<cfe<foo_list>>, foo_list<foo_list<>, foo_list<>>, foo_list<>>{};
	foo_list<> t3 = call<flatten<cfe<foo_list>, cfe<foo_list>>, foo_list<foo_list<>, foo_list<>>, foo_list<>>{};
	list<int, bool> t4 = call<flatten<>, list<int, bool>>{};
	foo_list<int, bool> t5 = call<flatten<cfe<foo_list>, cfe<foo_list>>, foo_list<int, bool>>{};
	foo_list<int, bool> t6 = call<flatten<cfe<list>, cfe<foo_list>>, list<int, list<bool>>>{};
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_FLATTEN