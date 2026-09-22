#ifndef QOR_PP_H_TEST_MPL_ALGORITHM_SORT
#define QOR_PP_H_TEST_MPL_ALGORITHM_SORT

#include <type_traits>

#include "src/qor/essentials/mpl/algorithm/sort.h"
#include "src/qor/essentials/mpl/bool.h"
#include "src/qor/essentials/mpl/int.h"
#include "src/qor/essentials/mpl/list.h"

using namespace qor::mpl;

template <typename E1, typename E2>
using compare_func = bool_<(E1::value < E2::value)>;

using test_list = list<int_<3>, int_<0>, int_<7>, int_<2>, int_<1>, int_<3>, int_<4>, int_<6>, int_<5>>;
using expect_list = list<int_<0>, int_<1>, int_<2>, int_<3>, int_<3>, int_<4>, int_<5>, int_<6>, int_<7>>;

static_assert(std::is_same<eager::sort<test_list>, expect_list>::value, "");

using empty_sort_test = eager::sort<list<>>;

#endif//QOR_PP_H_TEST_MPL_ALGORITHM_SORT
