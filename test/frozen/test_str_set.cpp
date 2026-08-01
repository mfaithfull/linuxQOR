// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <iostream>
#include <string>
#include <vector>

#include "src/qor/tdd/tdd.h"

#include "src/qor/essentials/datastructures/frozen/set.h"
#include "src/qor/essentials/datastructures/frozen/string.h"
#include <set>
using namespace qor;
struct StrSetSuite{};

qor_pp_test_suite_case(StrSetSuite, TripletonIntFrozenOrderedSet)
{
    constexpr frozen::set<frozen::string, 3> ze_set{"1", "2", "3"};

    constexpr auto empty = ze_set.empty();
    qor_pp_assert_that(!empty);

    constexpr auto size = ze_set.size();
    qor_pp_assert_that(size == 3);

    constexpr auto max_size = ze_set.max_size();
    qor_pp_assert_that(max_size == 3);

    constexpr auto nocount = ze_set.count("4");
    qor_pp_assert_that(nocount == 0);

    constexpr auto count = ze_set.count("1");
    qor_pp_assert_that(count == 1);

    auto notfound = ze_set.find("4");
    qor_pp_assert_that(notfound == ze_set.end());

    auto found = ze_set.find("1");
    qor_pp_assert_that(found == ze_set.begin());

    auto range = ze_set.equal_range("1");
    qor_pp_assert_that(std::get<0>(range) != ze_set.end());

    auto begin = ze_set.begin(), end = ze_set.end();
    qor_pp_assert_that(begin != end);

    auto cbegin = ze_set.cbegin(), cend = ze_set.cend();
    qor_pp_assert_that(cbegin != cend);

    std::for_each(ze_set.begin(), ze_set.end(), [](frozen::string const &) {});
}

qor_pp_test_suite_case(StrSetSuite, FrozenSetStrStdSet) 
{
#define INIT_SEQ                                                               \
  "19", "1", "2", "4", "5", "6", "7", "8", "9", "10",      \
      "11", "111", "1112", "1115", "1118", "1110", "1977",       \
      "177", "277", "477", "577", "677", "777", "877", "977",  \
      "1077", "1177", "11177", "111277", "111577", "111877",       \
      "111077", "1999", "199", "299", "499", "599", "699",       \
      "799", "899", "999", "1099", "1199", "11199", "111299",    \
      "111599", "111899", "111099", "197799", "17799", "27799",    \
      "47799", "57799", "67799", "77799", "87799", "97799",        \
      "107799", "117799", "1117799", "11127799", "11157799",         \
      "11187799", "11107799", "1988", "188", "288", "488",         \
      "588", "688", "788", "888", "988", "1088", "1188",         \
      "11188", "111288", "111588", "111888", "111088", "197788",   \
      "17788", "27788", "47788", "57788", "67788", "77788",        \
      "87788", "97788", "107788", "117788", "1117788", "11127788", \
      "11157788", "11187788", "11107788", "199988", "19988",         \
      "29988", "49988", "59988", "69988", "79988", "89988",        \
      "99988", "109988", "119988", "1119988", "11129988",            \
      "11159988", "11189988", "11109988", "19779988", "1779988",     \
      "2779988", "4779988", "5779988", "6779988", "7779988",         \
      "8779988", "9779988", "10779988", "11779988", "111779988",     \
      "1112779988", "1115779988", "1118779988", "1110779988"

    const std::set<frozen::string> std_set = {INIT_SEQ};
    constexpr frozen::set<frozen::string, 128> frozen_set = {INIT_SEQ};
    //SECTION("checking size and content") 
    {
        qor_pp_assert_that(std_set.size() == frozen_set.size());
        for (auto v : std_set)
            qor_pp_assert_that(frozen_set.count(v));

        for (auto v : frozen_set)
            qor_pp_assert_that(std_set.count(v));
    }
}
