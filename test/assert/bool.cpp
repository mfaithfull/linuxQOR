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

using namespace qor;
using namespace qor::test;

struct BoolSubjectTests{};

qor_pp_test_suite_case(BoolSubjectTests, IsTrue) {
  bool b = true;
  qor_pp_assert_that(b).isTrue();
  qor_pp_assert_that(true).isTrue();
}

qor_pp_test_suite_case(BoolSubjectTests, ExpectIsTrue) {
  bool b = true;
  qor_pp_expect_that(b).isTrue();
  qor_pp_expect_that(true).isTrue();
}

qor_pp_test_suite_case(BoolSubjectTests, IsFalse) {
  bool b = false;
  qor_pp_assert_that(b).isFalse();
  qor_pp_assert_that(false).isFalse();
}

qor_pp_test_suite_case(BoolSubjectTests, CanAssertTrueAndFalse) {
  bool b0 = true;
  qor_pp_assert_that(b0).isTrue();
  bool b1 = false;
  qor_pp_assert_that(b1).isFalse();
  qor_pp_assert_that(true).isTrue();
  qor_pp_assert_that(false).isFalse();
}

qor_pp_test_suite_case(BoolSubjectTests, NotEqualTo) {
  bool b0 = true;
  bool b1 = false;
  qor_pp_assert_that(b0).isNotEqualTo(b1).isNotEqualTo(false);
  qor_pp_assert_that(b1).isNotEqualTo(b0).isNotEqualTo(true);
}

qor_pp_test_suite_case(BoolSubjectTests, EqualTo) {
  bool b0 = true;
  bool b1 = true;
  qor_pp_assert_that(b0).isEqualTo(b1).isEqualTo(true);
  qor_pp_assert_that(b1).isEqualTo(b1).isEqualTo(true);
  qor_pp_assert_that(true).isEqualTo(true);
  qor_pp_assert_that(false).isEqualTo(false);
}
