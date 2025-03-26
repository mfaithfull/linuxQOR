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

#include <unordered_map>

#include "../../src/qor/test/test.h"
#include "../../src/qor/assert/assertcc.h"

namespace test_unordered_multimap
{
    using namespace qor;
    using namespace qor::test;

    struct UnorderedMultiMapSubjectTests {};

    qor_pp_test_suite_case(UnorderedMultiMapSubjectTests, isEmpty) 
    {
        std::unordered_multimap<int, int> x{};
        assertThat(x).isEmpty();
    }

    qor_pp_test_suite_case(UnorderedMultiMapSubjectTests, ExpectisEmpty) 
    {
        std::unordered_multimap<int, int> x{};
        expectThat(x).isEmpty();
    }

    qor_pp_test_suite_case(UnorderedMultiMapSubjectTests, IsNotEmpty) 
    {
        std::unordered_multimap<int, int> x{{1, 2}, {2, 3}};
        assertThat(x).isNotEmpty();
    }

    qor_pp_test_suite_case(UnorderedMultiMapSubjectTests, IsEqual) 
    {
        std::unordered_multimap<int, int> x{{1, 2}, {2, 3}};
        std::unordered_multimap<int, int> y{{1, 2}, {2, 3}};
        assertThat(x).isEqualTo(y);
    }

    qor_pp_test_suite_case(UnorderedMultiMapSubjectTests, IsNotEqual) 
    {
        std::unordered_multimap<int, int> x{{1, 2}, {2, 4}};
        std::unordered_multimap<int, int> y{{1, 2}, {2, 3}};
        assertThat(x).isNotEqualTo(y);
    }

    qor_pp_test_suite_case(UnorderedMultiMapSubjectTests, HasSize) 
    {
        std::unordered_multimap<int, int> x{{1, 2}, {2, 4}};
        assertThat(x).hasSizeThat().isEqualTo(2);
    }

}//test_unordered_multimap
