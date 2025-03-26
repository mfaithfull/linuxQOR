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
#include "../../src/qor/assert/proposition/isequaltopropositions.h"
#include "../../src/qor/assert/subject/base.h"
#include "../../src/qor/assert/util/failmessage.h"

namespace test_user_subject
{

    using namespace qor;
    using namespace qor::test;

    struct UserSubjectTests{};

    class Foo 
    {
    public:
        int x;
        bool operator==(const Foo& rhs) const { return x == rhs.x; }
    };

    std::ostream& operator<<(std::ostream& os, const Foo& foo) 
    {
        os << "[Foo " << foo.x << " ]";
        return os;
    }

    class FooSubject : public virtual qor::assert::subject::Base, public qor::assert::proposition::IsEqualToPropositions<FooSubject, Foo> 
    {
        const Foo& d_value;

    protected:

        const Foo* getValue() const override 
        { 
            return &d_value; 
        }

    public:

        FooSubject(const bool failOnError, const char* file, int line, const Foo& v) : Base(failOnError, file, line), d_value(v) {}
    
        FooSubject& contains42() 
        {
            if (getValue()->x != 42) 
            {
                qor::assert::util::FailMessage::create()
                .file(getFile())
                .line(getLine())
                .fact("Expected the value to be equal to 42")
                .fact("Got", *getValue());
            }
            return *this;
        }
    };
}//test_user_subject

namespace qor{ namespace assert
{
    auto assert_that_internal( Adl dummy, const bool failOnError, const char* file, int line, const test_user_subject::Foo& foo) 
    { // v needs to be a reference otherwise GenericSubject  will store a reference to a temporary
        return test_user_subject::FooSubject(false, file, line, foo);
    }
}}//qor::assert

namespace test_user_subject
{
    qor_pp_test_suite_case(UserSubjectTests, isEqual) 
    {
        Foo a{1};
        Foo b{1};
        qor_pp_assert_that(a).isEqualTo(b);
    }

    qor_pp_test_suite_case(UserSubjectTests, ExpectIsEqual) 
    {
        Foo a{1};
        Foo b{1};
        qor_pp_expect_that(a).isEqualTo(b);
    }

    qor_pp_test_suite_case(UserSubjectTests, isNotEqual) 
    {
        Foo a{1};
        Foo b{2};
        qor_pp_assert_that(a).isNotEqualTo(b);
    }

    qor_pp_test_suite_case(UserSubjectTests, Contains42) 
    {
        Foo a{42};
        qor_pp_assert_that(a).contains42();
    }
}//test_user_subject
