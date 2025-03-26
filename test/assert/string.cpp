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

#include <string>

#include "../../src/qor/test/test.h"
#include "../../src/qor/assert/assertcc.h"

namespace test_string
{
    using namespace qor;
    using namespace qor::test;

    struct StringTests {};

    qor_pp_test_suite_case(StringTests, isEmpty) 
    {
        std::string x{""};
        qor_pp_assert_that(x).isEmpty();
    }

    qor_pp_test_suite_case(StringTests, ExpectisEmpty) 
    {
        std::string x{""};
        qor_pp_expect_that(x).isEmpty();
    }

    qor_pp_test_suite_case(StringTests, isNotEmpty) 
    {
        std::string x{"hello"};
        qor_pp_assert_that(x).isNotEmpty();
    }

    qor_pp_test_suite_case(StringTests, isEqual) 
    {
        std::string x{"hello"};
        std::string y{"hello"};
        qor_pp_assert_that(x).isEqualTo(y);
        qor_pp_assert_that(x).isEqualTo("hello");
    }

    qor_pp_test_suite_case(StringTests, isNotEqual) 
    {
        std::string x{"hello"};
        std::string y{"sdgfifgh"};
        qor_pp_assert_that(x).isNotEqualTo(y);
    }

    qor_pp_test_suite_case(StringTests, HasSize) 
    {
        std::string str{"hello"};
        qor_pp_assert_that(str).hasSize(5);
    }

    qor_pp_test_suite_case(StringTests, HasSizeThat) 
    {
        std::string str{"hello"};
        qor_pp_assert_that(str).hasSizeThat().isEqualTo(5);
    }

    qor_pp_test_suite_case(StringTests, Contains) 
    {
        std::string str{"hello"};
        qor_pp_assert_that(str).contains("el");
    }

    qor_pp_test_suite_case(StringTests, DoesNotContain) 
    {
        std::string str{"hello"};
        qor_pp_assert_that(str).doesNotContain("ABC");
    }

    qor_pp_test_suite_case(StringTests, StartsWith) 
    {
        std::string str{"hello"};
        qor_pp_assert_that(str).startsWith("hel");
    }

    qor_pp_test_suite_case(StringTests, EndsWith) 
    {
        std::string str{"hello"};
        qor_pp_assert_that(str).endsWith("llo");
    }

    qor_pp_test_suite_case(StringTests, ContainsMatch) 
    {
        std::string str{"hello"};
        qor_pp_assert_that(str).containsMatch("l.o");
    }

    qor_pp_test_suite_case(StringTests, DoesNotContainsMatch) 
    {
        std::string str{"hello"};
        qor_pp_assert_that(str).doesNotContainMatch("z.o");
    }

    qor_pp_test_suite_case(StringTests, Matches) 
    {
        std::string str{"hello"};
        qor_pp_assert_that(str).matches("hel.o");
    }

    qor_pp_test_suite_case(StringTests, DoesNotMatch) 
    {
        std::string str{"hello"};
        qor_pp_assert_that(str).doesNotMatch("z.o");
    }

}//test_string
