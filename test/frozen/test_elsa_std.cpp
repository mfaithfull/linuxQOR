// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include "src/qor/tdd/tdd.h"
#include "src/qor/essentials/datastructures/frozen/bits/defines.h"

#ifdef qor_pp_has_string_view

#include "src/qor/essentials/datastructures/frozen/bits/elsa_std.h"
#include "src/qor/essentials/datastructures/frozen/map.h"
#include "src/qor/essentials/datastructures/frozen/set.h"
#include "src/qor/essentials/datastructures/frozen/unordered_map.h"
#include "src/qor/essentials/datastructures/frozen/unordered_set.h"
using namespace qor;
struct TestFrozenStringTypes{};

qor_pp_test_suite_case(TestFrozenStringTypes, FrozenContainersWorkWithStandardStringView)
{
    constexpr std::string_view str = "string";

    const auto is_found_in = [&str](const auto& container){ return container.count(str) == 1; };

    constexpr frozen::set<std::string_view, 1> set{str};
    constexpr frozen::unordered_set<std::string_view, 1> unordered_set{str};
    constexpr auto map = frozen::make_map<std::string_view, int>({{str, 0}});
    constexpr auto unordered_map = frozen::make_unordered_map<std::string_view, int>({{str, 0}});

    qor_pp_assert_that(is_found_in(set));
    qor_pp_assert_that(is_found_in(unordered_set));
    qor_pp_assert_that(is_found_in(map));
    qor_pp_assert_that(is_found_in(unordered_map));
}


#ifdef qor_pp_has_constexpr_string

qor_pp_test_suite_case(TestFrozenStringTypes, FrozenContainersWorkWithStandardString)
{
    constexpr std::string str = "what?";

    const auto is_found_in = [&str](const auto& container){ return container.count(str) == 1; };

    constexpr frozen::set<std::string, 1> set{str};
    constexpr frozen::unordered_set<std::string, 1> unordered_set{str};
    constexpr auto map = frozen::make_map<std::string, int>({{str, 0}});
    constexpr auto unordered_map = frozen::make_unordered_map<std::string, int>({{str, 0}});

    qor_pp_assert_that(is_found_in(set));
    qor_pp_assert_that(is_found_in(unordered_set));
    qor_pp_assert_that(is_found_in(map));
    qor_pp_assert_that(is_found_in(unordered_map));
}

#endif//qor_pp_has_constexpr_string

#endif // FROZEN_LETITGO_HAS_STRING_VIEW
