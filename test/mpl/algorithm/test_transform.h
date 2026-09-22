#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_TRANSFORM
#define QOR_PP_H_TEST_MPL_ALGORITHM_TRANSFORM

#include <type_traits>
#include "src/qor/essentials/mpl/algorithm/transform.h"

namespace 
{
	namespace mpl = qor::mpl;
	static_assert(std::is_same<mpl::eager::transform<mpl::list<void, char, short, int>, mpl::list>, mpl::list<mpl::list<void>, mpl::list<char>, mpl::list<short>, mpl::list<int>>>::value, "");
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_TRANSFORM
