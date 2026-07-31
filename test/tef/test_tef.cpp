// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <iostream>
#include <string>
#include <vector>

#include "src/qor/tdd/tdd.h"
#include "src/qor/function/tef.h"

using namespace qor;

struct TypeErasedFunctionTestSuite{};

qor_pp_test_suite_case(TypeErasedFunctionTestSuite, emptyTest)
{
    qor_pp_assert_that(true).isEqualTo(true);
}
