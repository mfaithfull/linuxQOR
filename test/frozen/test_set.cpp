// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <iostream>
#include <string>
#include <vector>

#include "src/qor/tdd/tdd.h"

#include <algorithm>
#include "src/qor/essentials/datastructures/frozen/set.h"
#include <set>
#include <type_traits>
using namespace qor;
struct TestFrozenSet{};

qor_pp_test_suite_case(TestFrozenSet, EmptyFrozenSet) 
{
    constexpr frozen::set<int, 0> ze_set{};

    constexpr auto empty = ze_set.empty();
    qor_pp_assert_that(empty);

    constexpr auto size = ze_set.size();
    qor_pp_assert_that(size == 0);

    constexpr auto max_size = ze_set.max_size();
    qor_pp_assert_that(max_size == 0);

    constexpr auto count = ze_set.count(3);
    qor_pp_assert_that(count == 0);

    constexpr auto find = ze_set.find(5);
    qor_pp_assert_that(find == ze_set.end());

    constexpr auto range = ze_set.equal_range(0);
    qor_pp_assert_that(std::get<0>(range) == ze_set.end());
    qor_pp_assert_that(std::get<1>(range) == ze_set.end());

    constexpr auto lower_bound = ze_set.lower_bound(1);
    qor_pp_assert_that(lower_bound == ze_set.end());

    constexpr auto upper_bound = ze_set.upper_bound(1);
    qor_pp_assert_that(upper_bound == ze_set.end());

    auto constexpr begin = ze_set.begin(), end = ze_set.end();
    qor_pp_assert_that(begin == end);

    auto constexpr key_comp = ze_set.key_comp();
    auto constexpr key_comparison = key_comp(1, 2);
    qor_pp_assert_that(key_comparison);

    auto constexpr value_comp = ze_set.value_comp();
    auto constexpr value_comparison = value_comp(11, 12);
    qor_pp_assert_that(value_comparison);

    auto constexpr cbegin = ze_set.cbegin(), cend = ze_set.cend();
    qor_pp_assert_that(cbegin == cend);

    std::for_each(ze_set.begin(), ze_set.end(), [](int) {});
    qor_pp_assert_that(std::distance(ze_set.rbegin(), ze_set.rend()) == 0);
    qor_pp_assert_that(std::count(ze_set.crbegin(), ze_set.crend(), 3) == 0);
}

qor_pp_test_suite_case(TestFrozenSet, SingletonFrozenSet) 
{
    constexpr frozen::set<short, 1> ze_set{1};

    constexpr auto empty = ze_set.empty();
    qor_pp_assert_that(!empty);

    constexpr auto size = ze_set.size();
    qor_pp_assert_that(size == 1);

    constexpr auto max_size = ze_set.max_size();
    qor_pp_assert_that(max_size == 1);

    constexpr auto count1 = ze_set.count(1);
    qor_pp_assert_that(count1 == 1);

    constexpr auto count2 = ze_set.count(2);
    qor_pp_assert_that(count2 == 0);

    const auto find1 = ze_set.find(1);
    qor_pp_assert_that(find1 == ze_set.begin());

    const auto find5 = ze_set.find(5);
    qor_pp_assert_that(find5 == ze_set.end());

    const auto range0 = ze_set.equal_range(0);
    qor_pp_assert_that(std::get<0>(range0) == ze_set.end());
    qor_pp_assert_that(std::get<1>(range0) == ze_set.end());

    const auto range1 = ze_set.equal_range(1);
    qor_pp_assert_that(std::get<0>(range1) == ze_set.begin());
    qor_pp_assert_that(std::get<1>(range1) == ze_set.end());

    const auto lower_bound0 = ze_set.lower_bound(0);
    qor_pp_assert_that(lower_bound0 == ze_set.end());

    const auto lower_bound1 = ze_set.lower_bound(1);
    qor_pp_assert_that(lower_bound1 == ze_set.find(1));

    const auto lower_bound2 = ze_set.lower_bound(2);
    qor_pp_assert_that(lower_bound2 == ze_set.end());

    const auto upper_bound0 = ze_set.upper_bound(0);
    qor_pp_assert_that(upper_bound0 == ze_set.end());

    const auto upper_bound1 = ze_set.upper_bound(1);
    qor_pp_assert_that(upper_bound1 == ze_set.end());

    const auto upper_bound2 = ze_set.upper_bound(2);
    qor_pp_assert_that(upper_bound2 == ze_set.end());

    auto const begin = ze_set.begin(), end = ze_set.end();
    qor_pp_assert_that((begin + 1) == end);

    auto const key_comp = ze_set.key_comp();
    auto const key_comparison = key_comp(1, 2);
    qor_pp_assert_that(key_comparison);

    auto const value_comp = ze_set.value_comp();
    auto const value_comparison = value_comp(11, 12);
    qor_pp_assert_that(value_comparison);

    auto const cbegin = ze_set.cbegin(), cend = ze_set.cend();
    qor_pp_assert_that(cbegin == (cend - 1));

    std::for_each(ze_set.begin(), ze_set.end(), [](int) {});
    qor_pp_assert_that(std::distance(ze_set.rbegin(), ze_set.rend()) == 1);
    qor_pp_assert_that(std::count(ze_set.crbegin(), ze_set.crend(), 3) == 0);
    qor_pp_assert_that(std::count(ze_set.crbegin(), ze_set.crend(), 1) == 1);
}

qor_pp_test_suite_case(TestFrozenSet, TripleFrozenSet)
{
    constexpr frozen::set<unsigned long, 3> ze_set{10, 20, 30};

    constexpr auto empty = ze_set.empty();
    qor_pp_assert_that(!empty);

    constexpr auto size = ze_set.size();
    qor_pp_assert_that(size == 3);

    constexpr auto max_size = ze_set.max_size();
    qor_pp_assert_that(max_size == 3);

    constexpr auto count1 = ze_set.count(1);
    qor_pp_assert_that(count1 == 0);

    constexpr auto count10 = ze_set.count(10);
    qor_pp_assert_that(count10 == 1);

    const auto find10 = ze_set.find(10);
    qor_pp_assert_that(find10 == ze_set.begin());

    const auto find15 = ze_set.find(15);
    qor_pp_assert_that(find15 == ze_set.end());

    const auto range0 = ze_set.equal_range(0);
    qor_pp_assert_that(std::get<0>(range0) == ze_set.end());
    qor_pp_assert_that(std::get<1>(range0) == ze_set.end());

    const auto range1 = ze_set.equal_range(10);
    qor_pp_assert_that(std::get<0>(range1) == ze_set.begin());
    qor_pp_assert_that(std::get<1>(range1) == ze_set.begin() + 1);

    const auto lower_bound0 = ze_set.lower_bound(0);
    qor_pp_assert_that(lower_bound0 == ze_set.end());

    for (auto val : ze_set) 
    {
        const auto lower_bound = ze_set.lower_bound(val);
        qor_pp_assert_that(lower_bound == ze_set.find(val));
    }

    const auto lower_bound2 = ze_set.lower_bound(40);
    qor_pp_assert_that(lower_bound2 == ze_set.end());

    const auto upper_bound0 = ze_set.upper_bound(0);
    qor_pp_assert_that(upper_bound0 == ze_set.end());

    const auto upper_bound1 = ze_set.upper_bound(10);
    qor_pp_assert_that(upper_bound1 == (ze_set.begin() + 1));

    const auto upper_bound2 = ze_set.upper_bound(40);
    qor_pp_assert_that(upper_bound2 == ze_set.end());

    auto const begin = ze_set.begin(), end = ze_set.end();
    qor_pp_assert_that((begin + ze_set.size()) == end);

    auto const key_comp = ze_set.key_comp();
    auto const key_comparison = key_comp(1, 2);
    qor_pp_assert_that(key_comparison);

    auto const value_comp = ze_set.value_comp();
    auto const value_comparison = value_comp(11, 12);
    qor_pp_assert_that(value_comparison);

    auto const cbegin = ze_set.cbegin(), cend = ze_set.cend();
    qor_pp_assert_that(cbegin == (cend - ze_set.size()));

    std::for_each(ze_set.begin(), ze_set.end(), [](int) {});
    qor_pp_assert_that((std::size_t)std::distance(ze_set.rbegin(), ze_set.rend()) == ze_set.size());
    qor_pp_assert_that(std::count(ze_set.crbegin(), ze_set.crend(), 3) == 0);
    qor_pp_assert_that(std::count(ze_set.crbegin(), ze_set.crend(), 20) == 1);
}

qor_pp_test_suite_case(TestFrozenSet, FrozenSet_StdSet) 
{
#define INIT_SEQ                                                               \
  19, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 111, 1112, 1115, 1118, 1110, 1977, 177,  \
      277, 477, 577, 677, 777, 877, 977, 1077, 1177, 11177, 111277, 111577,    \
      111877, 111077, 1999, 199, 299, 499, 599, 699, 799, 899, 999, 1099,      \
      1199, 11199, 111299, 111599, 111899, 111099, 197799, 17799, 27799,       \
      47799, 57799, 67799, 77799, 87799, 97799, 107799, 117799, 1117799,       \
      11127799, 11157799, 11187799, 11107799, 1988, 188, 288, 488, 588, 688,   \
      788, 888, 988, 1088, 1188, 11188, 111288, 111588, 111888, 111088,        \
      197788, 17788, 27788, 47788, 57788, 67788, 77788, 87788, 97788, 107788,  \
      117788, 1117788, 11127788, 11157788, 11187788, 11107788, 199988, 19988,  \
      29988, 49988, 59988, 69988, 79988, 89988, 99988, 109988, 119988,         \
      1119988, 11129988, 11159988, 11189988, 11109988, 19779988, 1779988,      \
      2779988, 4779988, 5779988, 6779988, 7779988, 8779988, 9779988, 10779988, \
      11779988, 111779988, 1112779988, 1115779988, 1118779988, 1110779988

    const std::set<int> std_set = {INIT_SEQ};
    constexpr frozen::set<int, 128> frozen_set = {INIT_SEQ};

    //SECTION("checking size and content") 
    {
        qor_pp_assert_that(std_set.size() == frozen_set.size());
        for (auto v : std_set)
            qor_pp_assert_that(frozen_set.count(v));
        for (auto v : frozen_set)
            qor_pp_assert_that(std_set.count(v));
    }

}

qor_pp_test_suite_case(TestFrozenSet, FrozenSetFrozenMakeSet) 
{
    constexpr frozen::set<int, 128> from_ctor = { INIT_SEQ };
    constexpr int init_array[]{INIT_SEQ};
    constexpr auto from_c_array = frozen::make_set(init_array);
    constexpr auto from_std_array = frozen::make_set(std::array<int, 128>{{INIT_SEQ}});
    qor_pp_assert_that(std::equal(from_c_array.begin(), from_c_array.end(), from_std_array.begin()));

    //SECTION("checking size and content") 
    {
        qor_pp_assert_that(from_ctor.size() == from_c_array.size());
        for (auto v : from_c_array)
        qor_pp_assert_that(from_ctor.count(v));
        for (auto v : from_ctor)
        qor_pp_assert_that(from_c_array.count(v));
    }

    constexpr frozen::set<short, 0> frozen_empty_set = {};
    constexpr auto frozen_empty_set2 = frozen::make_set<short>();
    constexpr auto frozen_empty_set3 = frozen::make_set<short>({});

    //SECTION("checking empty set") 
    {
        qor_pp_assert_that(frozen_empty_set.empty());
        qor_pp_assert_that(frozen_empty_set.size() == 0);
        qor_pp_assert_that(frozen_empty_set.begin() == frozen_empty_set.end());

        qor_pp_assert_that(frozen_empty_set2.empty());
        qor_pp_assert_that(frozen_empty_set2.size() == 0);
        qor_pp_assert_that(frozen_empty_set2.begin() == frozen_empty_set2.end());

        qor_pp_assert_that(frozen_empty_set3.empty());
        qor_pp_assert_that(frozen_empty_set3.size() == 0);
        qor_pp_assert_that(frozen_empty_set3.begin() == frozen_empty_set3.end());
    }
}

qor_pp_test_suite_case(TestFrozenSet, FrozenSetConstexpr) 
{
    constexpr frozen::set<unsigned, 2> ce = {3, 11};
    static_assert(*ce.begin() == 3, "");
    static_assert(*(ce.begin() + 1) == 11, "");
    static_assert(ce.size() == 2, "");
    static_assert(ce.count(3), "");
    static_assert(!ce.count(0), "");
    static_assert(ce.find(0) == ce.end(), "");
    static_assert(ce.contains(3), "");
    static_assert(!ce.contains(0), "");
}

qor_pp_test_suite_case(TestFrozenSet, FrozenSetOfFrozenSet) 
{
    using s1 = frozen::set<unsigned, 1>;
    constexpr frozen::set<s1, 2> ce = {{3}, {11}};
    static_assert(*ce.begin() == s1({3}), "");
    static_assert(*(ce.begin() + 1) == s1({11}), "");
    static_assert(ce.size() == 2, "");
    static_assert(ce.count(s1({3})), "");
    static_assert(!ce.count(s1({0})), "");
    static_assert(ce.find(s1({0})) == ce.end(), "");
    static_assert(ce.contains(s1({3})), "");
    static_assert(!ce.contains(s1({0})), "");
}

struct Foo {
    int x;
};

constexpr inline bool operator==(Foo const &a, Foo const &b) { return a.x == b.x; }
constexpr inline bool operator<(Foo const &a, Foo const &b) { return a.x < b.x; }
constexpr inline bool operator<(Foo const &a, int b) { return a.x < b; }
constexpr inline bool operator<(int a, Foo const &b) { return a < b.x; }

qor_pp_test_suite_case(TestFrozenSet, FrozenSetHeterogeneousContainer) 
{
    constexpr std::array<Foo, 3> std_array{{{1}, {2}, {3}}};
    constexpr Foo c_array[]{{1}, {2}, {3}};
    constexpr auto from_std_array = frozen::make_set<Foo, std::less<>>(std_array);
    constexpr auto from_c_array = frozen::make_set<Foo, std::less<>>(c_array);

    qor_pp_assert_that(from_std_array == from_c_array);
    for (const auto& set : {from_std_array, from_c_array}) 
    {
        qor_pp_assert_that(set.find(1) != set.end());
        qor_pp_assert_that(set.count(2) == 1);
        qor_pp_assert_that(set.count(42) == 0);
    }
}

#ifdef qor_pp_has_deduction_guides

    qor_pp_test_suite_case(TestFrozenSet, FrozenSetDeductionGuide) 
    {
        constexpr frozen::set integersSet{1,2,3,4,5};
        static_assert(std::is_same<
                std::remove_cv_t<decltype(integersSet)>,
                frozen::set<int, 5>>::value, "wrong type deduced");
    }

#endif//qor_pp_has_deduction_guides
