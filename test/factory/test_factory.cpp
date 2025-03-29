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

#include "../../src/configuration/configuration.h"

#include "../../src/qor/test/test.h"
#include "../../src/qor/assert/assert.h"

#include "../../src/qor/factory/factory.h"
#include "../../src/qor/instance/instance.h"
#include "../../src/qor/instance/default.h"

using namespace qor;
using namespace qor::test;


struct FactoryTestSuite{};

size_t global_widget_count = 0;

class Test_Widget
{
private:
    int m_i;

public:

    Test_Widget() : m_i(0)
    {
        ++global_widget_count;
    }

    Test_Widget(int i) : m_i(i)
    {
        ++global_widget_count;
    }

    ~Test_Widget()
    {
        --global_widget_count;
    }

    int Value()
    {
        return m_i;
    }
};

qor_pp_declare_factory_of(Test_Widget, ExternalFactory);

qor_pp_test_suite_case(FactoryTestSuite, defaultSinlgeItemConstructionAndDestruction)
{
    Test_Widget* p = qor::factory_of<Test_Widget>::type::Construct();

    qor_pp_assert_that(p).isNotNull();
    qor_pp_assert_that(global_widget_count).isEqualTo(1);

    qor::factory_of<Test_Widget>::type::Destruct(p);
    qor_pp_assert_that(global_widget_count).isEqualTo(0);
}

qor_pp_test_suite_case(FactoryTestSuite, defaultMultiItemConstructionAndDestruction)
{
    Test_Widget* p = qor::factory_of<Test_Widget>::type::Construct(2);

    qor_pp_assert_that(p).isNotNull();
    qor_pp_assert_that(global_widget_count).isEqualTo(2);

    qor::factory_of<Test_Widget>::type::Destruct(p,2);
    qor_pp_assert_that(global_widget_count).isEqualTo(0);
}

qor_pp_test_suite_case(FactoryTestSuite, SinlgeItemBuildAndTearDown)
{
    Test_Widget* p = qor::factory_of<Test_Widget>::type::Build(1, 123);

    qor_pp_assert_that(p).isNotNull();
    qor_pp_assert_that(global_widget_count).isEqualTo(1);
    qor_pp_assert_that(p->Value()).isEqualTo(123);

    qor::factory_of<Test_Widget>::type::TearDown(p);
    qor_pp_assert_that(global_widget_count).isEqualTo(0);
}

qor_pp_test_suite_case(FactoryTestSuite, MultiItembuildAndTearDown)
{
    Test_Widget* p = qor::factory_of<Test_Widget>::type::Build(2, 456);

    qor_pp_assert_that(p).isNotNull();
    qor_pp_assert_that(global_widget_count).isEqualTo(2);
    qor_pp_assert_that(p[0].Value()).isEqualTo(456);
    qor_pp_assert_that(p[1].Value()).isEqualTo(456);

    qor::factory_of<Test_Widget>::type::TearDown(p, 2);
    qor_pp_assert_that(global_widget_count).isEqualTo(0);
}
