// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

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
