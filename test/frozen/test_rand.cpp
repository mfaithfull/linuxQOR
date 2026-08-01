// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <iostream>
#include <string>
#include <vector>

#include "src/qor/tdd/tdd.h"
#include "src/qor/essentials/datastructures/frozen/random.h"
#include <random>
using namespace qor;
struct TestFrozenRand{};

qor_pp_test_suite_case(TestFrozenRand, LlinearCcongruentialEngine) 
{
    frozen::linear_congruential_engine<std::uint32_t, 48271u, 0, 0x7fffffff> dist0;
    std::linear_congruential_engine<std::uint32_t, 48271u, 0, 0x7fffffff> rdist0;
    qor_pp_assert_that(dist0.min() == rdist0.min());
    qor_pp_assert_that(dist0.max() == rdist0.max());
    qor_pp_assert_that(dist0() == rdist0());
    qor_pp_assert_that(dist0() == rdist0());
    qor_pp_assert_that(dist0() == rdist0());

    auto next0 = dist0();
    (void) next0;
    dist0.discard(3);
    rdist0.discard(4);
    qor_pp_assert_that(dist0() == rdist0());

    frozen::linear_congruential_engine<std::uint32_t, 48271u, 0, 0x7fffffff> odist0;
    std::linear_congruential_engine<std::uint32_t, 48271u, 0, 0x7fffffff> ordist0;
    qor_pp_assert_that(rdist0() != ordist0());
    qor_pp_assert_that(dist0() != odist0());
    qor_pp_assert_that(!(dist0() == odist0()));


    frozen::minstd_rand dist1;
    (void)dist1;
    frozen::minstd_rand dist2;
    (void)dist2;
    frozen::linear_congruential_engine<std::size_t, 3, 3, 0> dist3;
    (void)dist3;

}

