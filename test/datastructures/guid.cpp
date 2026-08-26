// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/tdd/tdd.h"
#include "src/qor/essentials/datastructures/guid.h"

using namespace qor;
using namespace qor::test;

namespace test_guid {

    qor_pp_test_case (canStackGUID)
    {
        GUID g;
        qor_pp_assert_that(&g).isNotEqualTo(nullptr);
    }

    qor_pp_test_case (canStackinitialiseGUID)
    {
        GUID g = {0x00000001, 0x0001, 0x0001, { 0x01, 0x03, 0x03, 0x04, 0x05, 0x06, 0x07, 0x8}};
        qor_pp_assert_that(g == GUID({0x00000001, 0x0001, 0x0001, { 0x01, 0x03, 0x03, 0x04, 0x05, 0x06, 0x07, 0x8}}));
    }

    qor_pp_test_case (canConstExprinitialiseGUID)
    {
        constexpr GUID guid = {0x00000001, 0x0001, 0x0002, { 0x01, 0x03, 0x03, 0x04, 0x05, 0x06, 0x07, 0x8}};
        const GUID* luid = &guid;
        qor_pp_assert_that(*luid == GUID({0x00000001, 0x0001, 0x0002, { 0x01, 0x03, 0x03, 0x04, 0x05, 0x06, 0x07, 0x8}}));
    }

    qor_pp_test_case (GUIdComparison)
    {
        constexpr GUID guid = {0x00000001, 0x0001, 0x0002, { 0x01, 0x03, 0x03, 0x04, 0x05, 0x06, 0x07, 0x8}};
        qor_pp_assert_that(guid != null_guid);
        qor_pp_assert_that(guid > null_guid);
    }

}//test_guid
