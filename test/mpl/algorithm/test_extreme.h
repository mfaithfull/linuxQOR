#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_EXTREME
#define QOR_PP_H_TEST_MPL_ALGORITHM_EXTREME

#include "src/qor/essentials/mpl/mpl.h"

namespace 
{
	namespace mpl      = qor::mpl;
	using extreme_pred = mpl::if_<mpl::greater_than<>, mpl::at0<>, mpl::at1<>>;

	struct extreme_test 
    {
		extreme_test() 
        {
			mpl::call<mpl::extreme<extreme_pred>, mpl::uint_<0>, mpl::uint_<5>, mpl::uint_<4>, mpl::uint_<4>>{} = mpl::uint_<5>{};
		}
	};
}

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_EXTREME
