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

#include <map>
#include <string>

#include "../../src/qor/test/test.h"
#include "../../src/qor/assert/assertcc.h"

namespace test_map {

    using namespace qor;
    using namespace qor::test;

    struct MapTests {};

    qor_pp_test_suite_case(MapTests, IsEmpty) 
    {
        std::map<int, int> x{};
        assertThat(x).isEmpty();
    }

    qor_pp_test_suite_case(MapTests, IsNotEmpty) 
    {
        std::map<int, int> x{{1, 2}, {2, 3}};
        assertThat(x).isNotEmpty();
    }

    qor_pp_test_suite_case(MapTests, IsEqual) 
    {
        std::map<int, int> x{{1, 2}, {2, 3}};
        std::map<int, int> y{{1, 2}, {2, 3}};
        assertThat(x).isEqualTo(y);
    }

    qor_pp_test_suite_case(MapTests, IsNotEqual) 
    {
        std::map<int, int> x{{1, 2}, {2, 4}};
        std::map<int, int> y{{1, 2}, {2, 3}};
        assertThat(x).isNotEqualTo(y);
    }

    qor_pp_test_suite_case(MapTests, HasSize) 
    {
        std::map<int, int> x{{1, 2}, {2, 4}};
        assertThat(x).hasSizeThat().isEqualTo(2);
    }

    qor_pp_test_suite_case(MapTests, ContainsEntry) 
    {
        std::map<std::string, int> x{{"a", 1}, {"b", 2}};
        assertThat(x).containsEntry("a", 1).containsEntry("b", 2);
    }

    qor_pp_test_suite_case(MapTests, DoesNotContainsEntry) 
    {
        std::map<std::string, int> x{{"a", 1}, {"b", 2}};
        assertThat(x).doesNotContainEntry("a", 2).doesNotContainEntry("c", 3);
    }

    qor_pp_test_suite_case(MapTests, ContainsKey) 
    {
        std::map<std::string, int> x{{"a", 1}, {"b", 2}};
        assertThat(x).containsKey("a").containsKey("b");
    }

    qor_pp_test_suite_case(MapTests, DoesNotContainsKey) 
    {
        std::map<std::string, int> x{{"a", 1}, {"b", 2}};
        assertThat(x).doesNotContainKey("d");
    }

}//test_map
