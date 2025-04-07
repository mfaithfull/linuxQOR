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

#include "src/configuration/configuration.h"

#include "src/qor/test/test.h"
#include "src/qor/assert/assert.h"
#include "src/framework/thread/currentthread.h"
#include "src/qor/objectcontext/anyobject.h"
#include "../../src/qor/injection/typeidentity.h"
#include "src/qor/reference/newref.h"

using namespace qor;
using namespace qor::test;


struct ReferenceTestSuite{};

class Test_Biscuit
{
private:
    int m_i;

public:

    Test_Biscuit() : m_i(0)
    {
    }

    Test_Biscuit(int i) : m_i(i)
    {
    }

    ~Test_Biscuit()
    {
    }

    int Value()
    {
        return m_i;
    }
};

qor_pp_declare_factory_of(Test_Biscuit, InternalFactory);

qor_pp_test_suite_case(ReferenceTestSuite, canGetAWidgetRef)
{
    auto ref = qor::new_ref<Test_Biscuit>();
    qor_pp_assert_that(ref.operator Test_Biscuit *()).isNotNull();
}

