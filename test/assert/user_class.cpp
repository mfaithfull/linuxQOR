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

#include "../../src/qor/assert/adl.h"
#include "../../src/qor/test/test.h"
#include "../../src/qor/assert/assert.h"

namespace test_user_class
{

    using namespace qor;
    using namespace qor::test;

    struct UserClassTests{};

    class A 
    {
        public:
        int x;
        bool operator==(const A& rhs) const { return x == rhs.x; }
    };

    std::ostream& operator<<(std::ostream& os, const A& a) 
    {
        os << "[A " << a.x << " ]";
        return os;
    }
}

namespace qor{ namespace assert{

    auto assert_that_internal( Adl dummy, const bool failOnError, const char* file, int line, const test_user_class::A& v) 
    {
        // v needs to be a reference otherwise UserClassTests will store a reference to a temporary
        return subject::GenericSubject(false, file, line, v);
    }
}}//qor::assert

namespace test_user_class
{
    qor_pp_test_suite_case(UserClassTests, IsEqual) 
    {
        A a{1};
        A b{1};
        qor_pp_assert_that(a).isEqualTo(b);
    }

    qor_pp_test_suite_case(UserClassTests, ExpectIsEqual) 
    {
        A a{1};
        A b{1};
        qor_pp_expect_that(a).isEqualTo(b);
    }

    qor_pp_test_suite_case(UserClassTests, IsNotEqual) 
    {
        A a{1};
        A b{2};
        qor_pp_assert_that(a).isNotEqualTo(b);
    }
}//test_user_class
