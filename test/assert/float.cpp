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
#include "../../src/qor/assert/assertcc.h"

namespace test_float {

    using namespace qor;
    using namespace qor::test;

    struct Float {};

    qor_pp_test_suite_case(Float, IsNan) 
    {
        float number0 = std::numeric_limits<float>::quiet_NaN();
        qor_pp_assert_that(number0).isNan();
        float number1 = std::numeric_limits<float>::signaling_NaN();
        qor_pp_assert_that(number1).isNan();
        //TODO: EXPECT_FATAL_FAILURE(qor_pp_assert_that(2.2).isNan(), "");
    }

    qor_pp_test_suite_case(Float, IsNotNan) 
    {
        float number0 = 1.2f;
        qor_pp_assert_that(number0).isNotNan();
        //TODO: EXPECT_FATAL_FAILURE(qor_pp_assert_that(std::numeric_limits<float>::signaling_NaN()).isNotNan(), "");
    }

    qor_pp_test_suite_case(Float, IsFinite) 
    {
        float number0 = 2.3f;
        qor_pp_assert_that(number0).isFinite();
        //TODO: EXPECT_FATAL_FAILURE(qor_pp_assert_that(std::numeric_limits<float>::signaling_NaN()).isFinite(), "");
    }

    qor_pp_test_suite_case(Float, IsNotFinite) 
    {
        float number0 = std::numeric_limits<float>::signaling_NaN();
        qor_pp_assert_that(number0).isNotFinite();
        //TODO: EXPECT_FATAL_FAILURE(qor_pp_assert_that(1.2).isNotFinite(), "");
    }

    qor_pp_test_suite_case(Float, IsPositiveInfinity) 
    {
        volatile float x = 0;
        float number0 = 1.0f / x;
        qor_pp_assert_that(number0).isPositiveInfinity();
    }

    qor_pp_test_suite_case(Float, isNegativeInfinity) 
    {
        volatile float x = 0;
        float number0 = (-1.0f) / x;
        qor_pp_assert_that(number0).isNegativeInfinity();
    }

    qor_pp_test_suite_case(Float, isZero) 
    {
        float number0 = 0.0f;
        qor_pp_assert_that(number0).isZero();
    }

    qor_pp_test_suite_case(Float, isNotZero) 
    {
        float number0 = 0.3f;
        qor_pp_assert_that(number0).isNotZero();
    }

    qor_pp_test_suite_case(Float, EqualTo) 
    {
        float a = 1.0f;
        float b = 1.0f;
        qor_pp_assert_that(a).isEqualTo(1).isEqualTo(b);
    }

    qor_pp_test_suite_case(Float, ExpectEqualTo) 
    {
        float a = 1.0f;
        float b = 1.0f;
        qor_pp_expect_that(a).isEqualTo(1).isEqualTo(b);
    }

    qor_pp_test_suite_case(Float, IsWithIn) 
    {
        float a = 1.0f;
        float b = 2.0f;
        qor_pp_assert_that(a).isWithin(1.0).isEqualTo(b);
    }

    qor_pp_test_suite_case(Float, NotEqualTo) 
    {
        float a = 1.0f;
        float b = 2.0f;
        qor_pp_assert_that(a).isNotEqualTo(3).isNotEqualTo(b);
        //TODO: EXPECT_FATAL_FAILURE(qor_pp_assert_that(1).isNotEqualTo(1), "");
    }

    qor_pp_test_suite_case(Float, IsIn) 
    {
        float a = 1.0f;
        qor_pp_assert_that(a).isIn({1.0, 2.0, 3.0});
        std::vector<float> v{1.0, 2.0, 3.0};
        qor_pp_assert_that(a).isIn(v);
    }

    qor_pp_test_suite_case(Float, IsNotIn) 
    {
        float a = 12.0;
        qor_pp_assert_that(a).isNotIn({1.0, 2.0, 3.0});
        std::vector<float> v{1.0, 2.0, 3.0};
        qor_pp_assert_that(a).isNotIn<std::vector>(v);
    }

    qor_pp_test_suite_case(Float, GreaterThan) 
    {
        float a = 10;
        qor_pp_assert_that(a).isGreaterThan(5.0);
        qor_pp_assert_that(11.0).isGreaterThan(5.0);
    }

    qor_pp_test_suite_case(Float, LessThan) 
    {
        float a = 1;
        qor_pp_assert_that(a).isLessThan(5);
        qor_pp_assert_that(2).isLessThan(5);
    }

    qor_pp_test_suite_case(Float, IsAtLeast) 
    {
        int a = 10;
        qor_pp_assert_that(a).isAtLeast(5).isAtLeast(10);
        qor_pp_assert_that(11).isAtLeast(5).isAtLeast(11);
    }

    qor_pp_test_suite_case(Float, isAtMost) 
    {
        float a = 1.2f;
        qor_pp_assert_that(a).isAtMost(10.0f).isAtMost(1.3f);
        qor_pp_assert_that(3.0f).isAtMost(5.0f).isAtMost(3.5f);
    }

}