// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#include "../../src/configuration/configuration.h"

#include "../../src/qor/test/test.h"
#include "../../src/qor/assert/assert.h"

namespace test_int {

    using namespace qor;
    using namespace qor::test;

    struct IntTests {};

    qor_pp_test_suite_case(IntTests, IsEqualTo) 
    {
        int a = 1;
        int b = 1;
        qor_pp_assert_that(a).isEqualTo(1).isEqualTo(b);
    }

    qor_pp_test_suite_case(IntTests, IsNotEqualTo) 
    {
        int a = 1;
        int b = 2;
        qor_pp_assert_that(a).isNotEqualTo(3).isNotEqualTo(b);
        //TODO: EXPECT_FATAL_FAILURE(qor_pp_assert_that(1).isNotEqualTo(1), "");
    }

    qor_pp_test_suite_case(IntTests, IsIn) 
    {
        int a = 1;
        qor_pp_assert_that(a).isIn({1, 2, 3});
        std::vector<int> v{1, 2, 3};
        qor_pp_assert_that(a).isIn(v);
    }

    qor_pp_test_suite_case(IntTests, IsNotIn) 
    {
        int a = 10;
        qor_pp_assert_that(a).isNotIn({1, 2, 3});
        std::vector<int> v{1, 2, 3};
        qor_pp_assert_that(a).isNotIn<std::vector>(v);
    }

    qor_pp_test_suite_case(IntTests, IsGreaterThan) 
    {
        int a = 10;
        qor_pp_assert_that(a).isGreaterThan(5);
        qor_pp_assert_that(11).isGreaterThan(5);
    }

    qor_pp_test_suite_case(IntTests, IsLessThan) 
    {
        int a = 1;
        qor_pp_assert_that(a).isLessThan(5);
        qor_pp_assert_that(2).isLessThan(5);
    }

    qor_pp_test_suite_case(IntTests, IsAtLeast) 
    {
        int a = 10;
        qor_pp_assert_that(a).isAtLeast(5).isAtLeast(10);
        qor_pp_assert_that(11).isAtLeast(5).isAtLeast(11);
    }

    qor_pp_test_suite_case(IntTests, isAtMost) 
    {
        int a = 1;
        qor_pp_assert_that(a).isAtMost(10).isAtMost(1);
        qor_pp_assert_that(3).isAtMost(5).isAtMost(3);
    }
}
