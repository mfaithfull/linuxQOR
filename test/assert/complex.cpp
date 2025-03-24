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

#include <complex>

#include "../../src/qor/test/test.h"
#include "../../src/qor/assert/assertcc.h"

using namespace qor;
using namespace qor::test;

struct ComplexTests {};

qor_pp_test_suite_case(ComplexTests, IsEqualTo) {
  std::complex<int> a(1, 2);
  std::complex<int> b(1, 2);
  assertThat(a).isEqualTo(b);
}

qor_pp_test_suite_case(ComplexTests, IsNotEqualTo) {
  std::complex<int> a(1, 2);
  std::complex<int> b(3, 2);
  expectThat(a).isNotEqualTo(b);
}

/*
qor_pp_test_suite_case(ComplexTests, IsNotEqualTo_Failure) {
  std::complex<int> a(1, 2);
  std::complex<int> b(1, 23);
  assertThat(a).isNotEqualTo(3).isNotEqualTo(b);
  EXPECT_FATAL_FAILURE(assertThat(std::complex<int>(1, 2)).isNotEqualTo(std::complex<int>(1, 2)),
                       "");
}*/

qor_pp_test_suite_case(ComplexTests, IsIn) {
  std::complex<int> a(1, 2);
  assertThat(a).isIn({std::complex<int>(1, 2), std::complex<int>(2, 3)});
  std::vector<std::complex<int>> v{
      std::complex<int>(1, 2), std::complex<int>(2, 2), std::complex<int>(2, 3)};
  assertThat(a).isIn(v);
}

qor_pp_test_suite_case(ComplexTests, IsNotIn) {
  int a = 10;
  assertThat(a).isNotIn({1, 2, 3});
  std::vector<int> v{1, 2, 3};
  assertThat(a).isNotIn<std::vector>(v);
}
