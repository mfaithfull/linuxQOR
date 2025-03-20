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

#ifndef QOR_PP_H_TESTMACROS
#define QOR_PP_H_TESTMACROS

#define qor_pp_test_case(_CASE) \
static void _CASE();\
qor::test::AutoReg autoReg##_CASE(#_CASE, qor_pp_compiler_at, &_CASE);\
static void _CASE()

#define qor_pp_test_suite_case(_SUITE, _CASE)\
struct _CASE: public qor::test::Test<_SUITE, _CASE>{ _CASE(); };\
static void _SUITE##_CASE() {_CASE();}\
qor::test::AutoReg autoReg##_CASE(qor_pp_stringize(_SUITE::_CASE), qor_pp_compiler_at, &_SUITE##_CASE);\
_CASE::_CASE()

#define qor_pp_test_equal(e,a) \
qor::test::equal (e ,a , qor_pp_compiler_at, "qor_pp_test_equal(" #e " == " #a ") failed ")

#define qor_pp_test_unequal(e,a) \
qor::test::unequal (e, a, qor_pp_compiler_at, "qor_pp_test_unequal(" #e " != " #a ") failed ")

#define qor_pp_test_check(e) \
qor::test::check (e, qor_pp_compiler_at, "qor_pp_test_check(" #e ") failed ")

#define qor_pp_test_fail(s) qor::test::fail (s, qor_pp_compiler_at);

#define qor_pp_test_assert_throw(s, e) \
try { \
    s; \
		throw qor::test::failure (qor_pp_compiler_at,  #s " failed to throw");\
} \
catch(const e&)\
{}

#endif//QOR_PP_H_TESTMACROS
