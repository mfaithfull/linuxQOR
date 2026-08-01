// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <iostream>
#include <string>
#include <vector>
#include "src/qor/tdd/tdd.h"
#include "src/qor/essentials/datastructures/frozen/bits/algorithms.h"

using namespace qor;
struct TestFrozenAlgorithms{};

qor_pp_test_suite_case(TestFrozenAlgorithms, Next_highest_power_of_two) 
{
    qor_pp_assert_that(frozen::bits::next_highest_power_of_two(1) == 1);
    qor_pp_assert_that(frozen::bits::next_highest_power_of_two(2) == 2);
    qor_pp_assert_that(frozen::bits::next_highest_power_of_two(3) == 4);
    qor_pp_assert_that(frozen::bits::next_highest_power_of_two(4) == 4);
    qor_pp_assert_that(frozen::bits::next_highest_power_of_two(5) == 8);
    qor_pp_assert_that(frozen::bits::next_highest_power_of_two(6) == 8);
    qor_pp_assert_that(frozen::bits::next_highest_power_of_two(7) == 8);
    qor_pp_assert_that(frozen::bits::next_highest_power_of_two(8) == 8);
    qor_pp_assert_that(frozen::bits::next_highest_power_of_two(16) == 16);
}

qor_pp_test_suite_case(TestFrozenAlgorithms, _log) 
{
    qor_pp_assert_that(frozen::bits::log(1) == 0);
    qor_pp_assert_that(frozen::bits::log(2) == 1);
    qor_pp_assert_that(frozen::bits::log(3) == 1);
    qor_pp_assert_that(frozen::bits::log(4) == 2);
    qor_pp_assert_that(frozen::bits::log(5) == 2);
    qor_pp_assert_that(frozen::bits::log(7) == 2);
    qor_pp_assert_that(frozen::bits::log(8) == 3);
    qor_pp_assert_that(frozen::bits::log(16) == 4);
    qor_pp_assert_that(frozen::bits::log(32) == 5);
}
