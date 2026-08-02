// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright 2026 Ilya Korolev
// Licensed under the Apache License, Version 2.0
// SPDX-License-Identifier: Apache-2.0

#ifndef QOR_PP_H_CONTRACT_MACROS
#define QOR_PP_H_CONTRACT_MACROS

#include <type_traits>
#include <utility>

#define qor_pp_contract_cat_impl(a, b) a##b
#define qor_pp_contract_cat(a, b) qor_pp_contract_cat_impl(a, b)
#define qor_pp_contract_unparen(...) __VA_ARGS__
#define qor_pp_contract_first(first, ...) first

// Preprocessor utilities below are just enough to keep the public DSL small.
// Max 32 field entries per qor_pp_contract(...) call (raised from 16 - see the
// qor_pp_contract_for_each*/BOOL chains below, which must all be extended in
// lockstep with this). Per-field attribute count (PROPERTY_*/MAKE_ENTRY_*_*)
// is a separate, independent limit, still 16 - not raised, not requested.
#define qor_pp_contract_narg_impl( \
    _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, \
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, \
    _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, \
    _30, _31, _32, N, ...) N

#define qor_pp_contract_narg(...) \
    qor_pp_contract_narg_impl(0 __VA_OPT__(,) __VA_ARGS__, \
        32, 31, 30, 29, 28, 27, 26, 25, 24, 23, \
        22, 21, 20, 19, 18, 17, 16, 15, 14, 13, \
        12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define qor_pp_contract_bool_0 0
#define qor_pp_contract_bool_1 1
#define qor_pp_contract_bool_2 1
#define qor_pp_contract_bool_3 1
#define qor_pp_contract_bool_4 1
#define qor_pp_contract_bool_5 1
#define qor_pp_contract_bool_6 1
#define qor_pp_contract_bool_7 1
#define qor_pp_contract_bool_8 1
#define qor_pp_contract_bool_9 1
#define qor_pp_contract_bool_10 1
#define qor_pp_contract_bool_11 1
#define qor_pp_contract_bool_12 1
#define qor_pp_contract_bool_13 1
#define qor_pp_contract_bool_14 1
#define qor_pp_contract_bool_15 1
#define qor_pp_contract_bool_16 1
#define qor_pp_contract_bool_17 1
#define qor_pp_contract_bool_18 1
#define qor_pp_contract_bool_19 1
#define qor_pp_contract_bool_20 1
#define qor_pp_contract_bool_21 1
#define qor_pp_contract_bool_22 1
#define qor_pp_contract_bool_23 1
#define qor_pp_contract_bool_24 1
#define qor_pp_contract_bool_25 1
#define qor_pp_contract_bool_26 1
#define qor_pp_contract_bool_27 1
#define qor_pp_contract_bool_28 1
#define qor_pp_contract_bool_29 1
#define qor_pp_contract_bool_30 1
#define qor_pp_contract_bool_31 1
#define qor_pp_contract_bool_32 1
#define qor_pp_contract_bool(value) qor_pp_contract_cat(qor_pp_contract_bool_, value)

#define qor_pp_contract_for_each_comma(macro, ...) \
    qor_pp_contract_cat(qor_pp_contract_for_each_comma_, qor_pp_contract_narg(__VA_ARGS__))(macro, __VA_ARGS__)

#define qor_pp_contract_for_each_comma_arg(macro, arg, ...) \
    qor_pp_contract_cat(qor_pp_contract_for_each_comma_arg_, qor_pp_contract_narg(__VA_ARGS__))(macro, arg, __VA_ARGS__)

#define qor_pp_contract_for_each_arg(macro, arg, ...) \
    qor_pp_contract_cat(qor_pp_contract_for_each_arg_, qor_pp_contract_narg(__VA_ARGS__))(macro, arg, __VA_ARGS__)

#define qor_pp_contract_for_each_comma_1(m, x) m(x)
#define qor_pp_contract_for_each_comma_2(m, x, ...) m(x), qor_pp_contract_for_each_comma_1(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_3(m, x, ...) m(x), qor_pp_contract_for_each_comma_2(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_4(m, x, ...) m(x), qor_pp_contract_for_each_comma_3(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_5(m, x, ...) m(x), qor_pp_contract_for_each_comma_4(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_6(m, x, ...) m(x), qor_pp_contract_for_each_comma_5(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_7(m, x, ...) m(x), qor_pp_contract_for_each_comma_6(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_8(m, x, ...) m(x), qor_pp_contract_for_each_comma_7(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_9(m, x, ...) m(x), qor_pp_contract_for_each_comma_8(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_10(m, x, ...) m(x), qor_pp_contract_for_each_comma_9(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_11(m, x, ...) m(x), qor_pp_contract_for_each_comma_10(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_12(m, x, ...) m(x), qor_pp_contract_for_each_comma_11(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_13(m, x, ...) m(x), qor_pp_contract_for_each_comma_12(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_14(m, x, ...) m(x), qor_pp_contract_for_each_comma_13(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_15(m, x, ...) m(x), qor_pp_contract_for_each_comma_14(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_16(m, x, ...) m(x), qor_pp_contract_for_each_comma_15(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_17(m, x, ...) m(x), qor_pp_contract_for_each_comma_16(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_18(m, x, ...) m(x), qor_pp_contract_for_each_comma_17(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_19(m, x, ...) m(x), qor_pp_contract_for_each_comma_18(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_20(m, x, ...) m(x), qor_pp_contract_for_each_comma_19(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_21(m, x, ...) m(x), qor_pp_contract_for_each_comma_20(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_22(m, x, ...) m(x), qor_pp_contract_for_each_comma_21(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_23(m, x, ...) m(x), qor_pp_contract_for_each_comma_22(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_24(m, x, ...) m(x), qor_pp_contract_for_each_comma_23(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_25(m, x, ...) m(x), qor_pp_contract_for_each_comma_24(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_26(m, x, ...) m(x), qor_pp_contract_for_each_comma_25(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_27(m, x, ...) m(x), qor_pp_contract_for_each_comma_26(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_28(m, x, ...) m(x), qor_pp_contract_for_each_comma_27(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_29(m, x, ...) m(x), qor_pp_contract_for_each_comma_28(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_30(m, x, ...) m(x), qor_pp_contract_for_each_comma_29(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_31(m, x, ...) m(x), qor_pp_contract_for_each_comma_30(m, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_32(m, x, ...) m(x), qor_pp_contract_for_each_comma_31(m, __VA_ARGS__)

#define qor_pp_contract_for_each_comma_arg_1(m, arg, x) m(arg, x)
#define qor_pp_contract_for_each_comma_arg_2(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_1(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_3(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_2(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_4(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_3(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_5(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_4(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_6(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_5(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_7(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_6(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_8(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_7(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_9(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_8(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_10(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_9(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_11(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_10(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_12(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_11(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_13(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_12(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_14(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_13(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_15(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_14(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_16(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_15(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_17(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_16(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_18(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_17(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_19(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_18(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_20(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_19(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_21(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_20(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_22(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_21(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_23(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_22(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_24(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_23(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_25(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_24(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_26(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_25(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_27(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_26(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_28(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_27(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_29(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_28(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_30(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_29(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_31(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_30(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_comma_arg_32(m, arg, x, ...) m(arg, x), qor_pp_contract_for_each_comma_arg_31(m, arg, __VA_ARGS__)

#define qor_pp_contract_for_each_arg_1(m, arg, x) m(arg, x)
#define qor_pp_contract_for_each_arg_2(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_1(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_3(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_2(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_4(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_3(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_5(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_4(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_6(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_5(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_7(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_6(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_8(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_7(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_9(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_8(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_10(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_9(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_11(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_10(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_12(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_11(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_13(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_12(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_14(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_13(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_15(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_14(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_16(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_15(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_17(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_16(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_18(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_17(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_19(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_18(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_20(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_19(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_21(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_20(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_22(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_21(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_23(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_22(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_24(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_23(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_25(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_24(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_26(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_25(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_27(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_26(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_28(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_27(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_29(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_28(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_30(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_29(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_31(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_30(m, arg, __VA_ARGS__)
#define qor_pp_contract_for_each_arg_32(m, arg, x, ...) m(arg, x) qor_pp_contract_for_each_arg_31(m, arg, __VA_ARGS__)

#define qor_pp_contract_stringize_impl(value) #value
#define qor_pp_contract_stringize(value) qor_pp_contract_stringize_impl(value)
#define qor_pp_contract_describe_attribute(value) \
    qor::contract::describe_attribute((value), qor_pp_contract_stringize(value))

#define BASE(type, offset) (qor_pp_contract_base_marker, type, offset)
#define REFERENCE(...) qor_pp_contract_cat(qor_pp_contract_reference_, qor_pp_contract_narg(__VA_ARGS__))(__VA_ARGS__)
#define qor_pp_contract_reference_2(name, id) (qor_pp_contract_reference_marker, name, id)
#define qor_pp_contract_reference_with_attrs(name, id, ...) \
    (qor_pp_contract_reference_marker, name, id, __VA_ARGS__)
#define qor_pp_contract_reference_3 qor_pp_contract_reference_with_attrs
#define qor_pp_contract_reference_4 qor_pp_contract_reference_with_attrs
#define qor_pp_contract_reference_5 qor_pp_contract_reference_with_attrs
#define qor_pp_contract_reference_6 qor_pp_contract_reference_with_attrs
#define qor_pp_contract_reference_7 qor_pp_contract_reference_with_attrs
#define qor_pp_contract_reference_8 qor_pp_contract_reference_with_attrs
#define qor_pp_contract_reference_9 qor_pp_contract_reference_with_attrs
#define qor_pp_contract_reference_10 qor_pp_contract_reference_with_attrs
#define qor_pp_contract_reference_11 qor_pp_contract_reference_with_attrs
#define qor_pp_contract_reference_12 qor_pp_contract_reference_with_attrs
#define qor_pp_contract_reference_13 qor_pp_contract_reference_with_attrs
#define qor_pp_contract_reference_14 qor_pp_contract_reference_with_attrs
#define qor_pp_contract_reference_15 qor_pp_contract_reference_with_attrs
#define qor_pp_contract_reference_16 qor_pp_contract_reference_with_attrs
// PROPERTY keeps the legacy 3-argument form and adds an attrs-bearing form.
#define PROPERTY(...) qor_pp_contract_cat(qor_pp_contract_property_, qor_pp_contract_narg(__VA_ARGS__))(__VA_ARGS__)
#define qor_pp_contract_property_3(name, id, type) (qor_pp_contract_property_marker, name, id, type)
#define qor_pp_contract_property_with_attrs(name, id, type, ...) \
    (qor_pp_contract_property_marker, name, id, type, __VA_ARGS__)
#define qor_pp_contract_property_4 qor_pp_contract_property_with_attrs
#define qor_pp_contract_property_5 qor_pp_contract_property_with_attrs
#define qor_pp_contract_property_6 qor_pp_contract_property_with_attrs
#define qor_pp_contract_property_7 qor_pp_contract_property_with_attrs
#define qor_pp_contract_property_8 qor_pp_contract_property_with_attrs
#define qor_pp_contract_property_9 qor_pp_contract_property_with_attrs
#define qor_pp_contract_property_10 qor_pp_contract_property_with_attrs
#define qor_pp_contract_property_11 qor_pp_contract_property_with_attrs
#define qor_pp_contract_property_12 qor_pp_contract_property_with_attrs
#define qor_pp_contract_property_13 qor_pp_contract_property_with_attrs
#define qor_pp_contract_property_14 qor_pp_contract_property_with_attrs
#define qor_pp_contract_property_15 qor_pp_contract_property_with_attrs
#define qor_pp_contract_property_16 qor_pp_contract_property_with_attrs
#define ATTRS(...) (contract_attrs_marker, __VA_ARGS__)

#define qor_pp_contract_second(a, b, ...) b
#define qor_pp_contract_probe() ~, 1
#define qor_pp_contract_is_probe(...) qor_pp_contract_second(__VA_ARGS__, 0)

#define qor_pp_contract_is_base_marker(value) qor_pp_contract_is_probe(qor_pp_contract_cat(qor_pp_contract_is_base_marker_, value))
#define qor_pp_contract_is_base_marker_qor_pp_contract_base_marker qor_pp_contract_probe()

#define qor_pp_contract_is_property_marker(value) qor_pp_contract_is_probe(qor_pp_contract_cat(qor_pp_contract_is_property_marker_, value))
#define qor_pp_contract_is_property_marker_qor_pp_contract_property_marker qor_pp_contract_probe()

#define qor_pp_contract_is_reference_marker(value) qor_pp_contract_is_probe(qor_pp_contract_cat(qor_pp_contract_is_reference_marker_, value))
#define qor_pp_contract_is_reference_marker_qor_pp_contract_reference_marker qor_pp_contract_probe()

#define qor_pp_contract_is_attrs_marker(value) qor_pp_contract_is_probe(qor_pp_contract_cat(qor_pp_contract_is_attrs_marker_, value))
#define qor_pp_contract_is_attrs_marker_contract_attrs_marker qor_pp_contract_probe()

// Every entry starts with a marker so the DSL can remain a single tuple form.
#define qor_pp_contract_entry_head(entry) qor_pp_contract_entry_head_impl entry
#define qor_pp_contract_entry_head_impl(first, ...) first

// Entry detection is marker-based, so the same tuple syntax can carry attrs.
#define qor_pp_contract_entry_is_base(entry) qor_pp_contract_is_base_marker(qor_pp_contract_entry_head(entry))
#define qor_pp_contract_entry_is_property(entry) qor_pp_contract_is_property_marker(qor_pp_contract_entry_head(entry))
#define qor_pp_contract_entry_is_reference(entry) qor_pp_contract_is_reference_marker(qor_pp_contract_entry_head(entry))
#define qor_pp_contract_entry_is_attrs(entry) qor_pp_contract_is_attrs_marker(qor_pp_contract_entry_head(entry))
#define qor_pp_contract_entry_kind(entry) \
    qor_pp_contract_cat(qor_pp_contract_entry_kind_base_, qor_pp_contract_entry_is_base(entry))(entry)

#define qor_pp_contract_entry_kind_base_1(entry) 1
#define qor_pp_contract_entry_kind_base_0(entry) \
    qor_pp_contract_cat(qor_pp_contract_entry_kind_reference_, qor_pp_contract_entry_is_reference(entry))(entry)

#define qor_pp_contract_entry_kind_reference_1(entry) 2
#define qor_pp_contract_entry_kind_reference_0(entry) \
    qor_pp_contract_cat(qor_pp_contract_entry_kind_property_, qor_pp_contract_entry_is_property(entry))(entry)

#define qor_pp_contract_entry_kind_property_1(entry) 3
#define qor_pp_contract_entry_kind_property_0(entry) 0

#define qor_pp_contract_base_type(entry) qor_pp_contract_base_type_impl entry
#define qor_pp_contract_base_type_impl(marker, type, offset) type

#define qor_pp_contract_base_offset(entry) qor_pp_contract_base_offset_impl entry
#define qor_pp_contract_base_offset_impl(marker, type, offset) offset

#define qor_pp_contract_property_name(entry) qor_pp_contract_property_name_impl entry
#define qor_pp_contract_property_name_impl(marker, name, id, type, ...) name

#define qor_pp_contract_reference_name(entry) qor_pp_contract_reference_name_impl entry
#define qor_pp_contract_reference_name_impl(marker, name, id, ...) name

#define qor_pp_contract_attrs_values(entry) qor_pp_contract_attrs_values_impl entry
#define qor_pp_contract_attrs_values_impl(marker, ...) __VA_ARGS__

#define qor_pp_contract_make_field_attributes(...) \
    qor::contract::make_field_attributes( \
        __VA_OPT__(qor_pp_contract_for_each_comma(qor_pp_contract_describe_attribute, __VA_ARGS__)))

#define qor_pp_contract_make_entry(contract_self_type, entry) \
    qor_pp_contract_cat(qor_pp_contract_make_entry_, qor_pp_contract_entry_kind(entry))(contract_self_type, entry)

#define qor_pp_contract_declare_field(contract_self_type, entry) \
    qor_pp_contract_cat(qor_pp_contract_declare_field_, qor_pp_contract_entry_kind(entry))(contract_self_type, entry)

#define qor_pp_contract_declare_field_0(contract_self_type, pair) \
    qor_pp_contract_declare_field_0_expand(contract_self_type, qor_pp_contract_unparen pair)

#define qor_pp_contract_declare_field_0_expand(contract_self_type, ...) \
    qor_pp_contract_cat(qor_pp_contract_declare_field_0_, qor_pp_contract_narg(__VA_ARGS__))(contract_self_type, __VA_ARGS__)

#define qor_pp_contract_declare_field_0_build(contract_self_type, name, id, ...) \
    using name = decltype(qor::contract::make_member_field<contract_self_type, id, &contract_self_type::name>( \
        qor_pp_contract_stringize(name), qor_pp_contract_make_field_attributes(__VA_ARGS__)));

#define qor_pp_contract_declare_field_0_2(contract_self_type, name, id) \
    qor_pp_contract_declare_field_0_build(contract_self_type, name, id)
#define qor_pp_contract_declare_field_0_with_attrs(contract_self_type, name, id, ...) \
    qor_pp_contract_declare_field_0_build(contract_self_type, name, id, __VA_ARGS__)
#define qor_pp_contract_declare_field_0_3 qor_pp_contract_declare_field_0_with_attrs
#define qor_pp_contract_declare_field_0_4 qor_pp_contract_declare_field_0_with_attrs
#define qor_pp_contract_declare_field_0_5 qor_pp_contract_declare_field_0_with_attrs
#define qor_pp_contract_declare_field_0_6 qor_pp_contract_declare_field_0_with_attrs
#define qor_pp_contract_declare_field_0_7 qor_pp_contract_declare_field_0_with_attrs
#define qor_pp_contract_declare_field_0_8 qor_pp_contract_declare_field_0_with_attrs
#define qor_pp_contract_declare_field_0_9 qor_pp_contract_declare_field_0_with_attrs
#define qor_pp_contract_declare_field_0_10 qor_pp_contract_declare_field_0_with_attrs
#define qor_pp_contract_declare_field_0_11 qor_pp_contract_declare_field_0_with_attrs
#define qor_pp_contract_declare_field_0_12 qor_pp_contract_declare_field_0_with_attrs
#define qor_pp_contract_declare_field_0_13 qor_pp_contract_declare_field_0_with_attrs
#define qor_pp_contract_declare_field_0_14 qor_pp_contract_declare_field_0_with_attrs
#define qor_pp_contract_declare_field_0_15 qor_pp_contract_declare_field_0_with_attrs
#define qor_pp_contract_declare_field_0_16 qor_pp_contract_declare_field_0_with_attrs

#define qor_pp_contract_declare_field_1(contract_self_type, entry)

#define qor_pp_contract_declare_field_2(contract_self_type, entry) \
    qor_pp_contract_declare_reference_field(contract_self_type, entry)

#define qor_pp_contract_declare_reference_field(contract_self_type, entry) \
    qor_pp_contract_declare_reference_field_expand(contract_self_type, qor_pp_contract_unparen entry)

#define qor_pp_contract_declare_reference_field_expand(contract_self_type, ...) \
    qor_pp_contract_cat(qor_pp_contract_declare_reference_field_, qor_pp_contract_narg(__VA_ARGS__))(contract_self_type, __VA_ARGS__)

#define qor_pp_contract_declare_reference_field_build(contract_self_type, name, id, ...) \
    struct name : qor::contract::field< \
        contract_self_type, id, ::contract::field_kind::reference, \
        decltype(qor_pp_contract_make_field_attributes(__VA_ARGS__)), \
        std::remove_cvref_t<decltype(contract_self_type::name)>, nullptr, name> { \
        using base_type = ::contract::field< \
            contract_self_type, id, ::contract::field_kind::reference, \
            decltype(qor_pp_contract_make_field_attributes(__VA_ARGS__)), \
            std::remove_cvref_t<decltype(contract_self_type::name)>, nullptr, name>; \
        using base_type::base_type; \
        template<class Obj> \
        static constexpr decltype(auto) contract_ref_get(Obj& obj) { return (obj.name); } \
        template<class Obj> \
        static constexpr decltype(auto) contract_ref_get(const Obj& obj) { return (obj.name); } \
        template<class Obj, class Value> \
        static constexpr void contract_ref_set(Obj& obj, Value&& value) \
            requires std::is_assignable_v<decltype((std::declval<Obj&>().name)), Value&&> { \
            obj.name = std::forward<Value>(value); \
        } \
    };

#define qor_pp_contract_declare_reference_field_3(contract_self_type, marker, name, id) \
    qor_pp_contract_declare_reference_field_build(contract_self_type, name, id)
#define qor_pp_contract_declare_reference_field_with_attrs(contract_self_type, marker, name, id, ...) \
    qor_pp_contract_declare_reference_field_build(contract_self_type, name, id, __VA_ARGS__)
#define qor_pp_contract_declare_reference_field_4 qor_pp_contract_declare_reference_field_with_attrs
#define qor_pp_contract_declare_reference_field_5 qor_pp_contract_declare_reference_field_with_attrs
#define qor_pp_contract_declare_reference_field_6 qor_pp_contract_declare_reference_field_with_attrs
#define qor_pp_contract_declare_reference_field_7 qor_pp_contract_declare_reference_field_with_attrs
#define qor_pp_contract_declare_reference_field_8 qor_pp_contract_declare_reference_field_with_attrs
#define qor_pp_contract_declare_reference_field_9 qor_pp_contract_declare_reference_field_with_attrs
#define qor_pp_contract_declare_reference_field_10 qor_pp_contract_declare_reference_field_with_attrs
#define qor_pp_contract_declare_reference_field_11 qor_pp_contract_declare_reference_field_with_attrs
#define qor_pp_contract_declare_reference_field_12 qor_pp_contract_declare_reference_field_with_attrs
#define qor_pp_contract_declare_reference_field_13 qor_pp_contract_declare_reference_field_with_attrs
#define qor_pp_contract_declare_reference_field_14 qor_pp_contract_declare_reference_field_with_attrs
#define qor_pp_contract_declare_reference_field_15 qor_pp_contract_declare_reference_field_with_attrs
#define qor_pp_contract_declare_reference_field_16 qor_pp_contract_declare_reference_field_with_attrs

#define qor_pp_contract_declare_field_3(contract_self_type, entry) \
    qor_pp_contract_declare_property_field(contract_self_type, entry)

#define qor_pp_contract_declare_property_field(contract_self_type, entry) \
    qor_pp_contract_declare_property_field_expand(contract_self_type, qor_pp_contract_unparen entry)
#define qor_pp_contract_declare_property_field_expand(contract_self_type, ...) \
    qor_pp_contract_cat(qor_pp_contract_declare_property_field_, qor_pp_contract_narg(__VA_ARGS__))(contract_self_type, __VA_ARGS__)
#define qor_pp_contract_declare_property_field_4(contract_self_type, marker, name, id, type) \
    using name = decltype(qor::contract::make_property_field<contract_self_type, id, type>( \
        qor_pp_contract_stringize(name), qor_pp_contract_make_field_attributes()));
#define qor_pp_contract_declare_property_field_with_attrs(contract_self_type, marker, name, id, type, ...) \
    using name = decltype(qor::contract::make_property_field<contract_self_type, id, type>( \
        qor_pp_contract_stringize(name), qor_pp_contract_make_field_attributes(__VA_ARGS__)));
#define qor_pp_contract_declare_property_field_5 qor_pp_contract_declare_property_field_with_attrs
#define qor_pp_contract_declare_property_field_6 qor_pp_contract_declare_property_field_with_attrs
#define qor_pp_contract_declare_property_field_7 qor_pp_contract_declare_property_field_with_attrs
#define qor_pp_contract_declare_property_field_8 qor_pp_contract_declare_property_field_with_attrs
#define qor_pp_contract_declare_property_field_9 qor_pp_contract_declare_property_field_with_attrs
#define qor_pp_contract_declare_property_field_10 qor_pp_contract_declare_property_field_with_attrs
#define qor_pp_contract_declare_property_field_11 qor_pp_contract_declare_property_field_with_attrs
#define qor_pp_contract_declare_property_field_12 qor_pp_contract_declare_property_field_with_attrs
#define qor_pp_contract_declare_property_field_13 qor_pp_contract_declare_property_field_with_attrs
#define qor_pp_contract_declare_property_field_14 qor_pp_contract_declare_property_field_with_attrs
#define qor_pp_contract_declare_property_field_15 qor_pp_contract_declare_property_field_with_attrs
#define qor_pp_contract_declare_property_field_16 qor_pp_contract_declare_property_field_with_attrs

#define qor_pp_contract_make_entry_0(contract_self_type, pair) \
    qor_pp_contract_make_entry_0_expand(contract_self_type, qor_pp_contract_unparen pair)

#define qor_pp_contract_make_entry_0_expand(contract_self_type, ...) \
    qor_pp_contract_cat(qor_pp_contract_make_entry_0_, qor_pp_contract_narg(__VA_ARGS__))( \
        contract_self_type, __VA_ARGS__)

#define qor_pp_contract_make_entry_0_2(contract_self_type, name, id) \
    qor_pp_contract_make_entry_0_build(contract_self_type, name, id)

#define qor_pp_contract_make_entry_0_with_attrs(contract_self_type, name, id, ...) \
    qor_pp_contract_make_entry_0_build(contract_self_type, name, id, __VA_ARGS__)

#define qor_pp_contract_make_entry_0_build(contract_self_type, name, id, ...) \
    qor::contract::make_member_field<contract_self_type, id, &contract_self_type::name>( \
            qor_pp_contract_stringize(name), \
            qor_pp_contract_make_field_attributes(__VA_ARGS__))

#define qor_pp_contract_make_entry_0_3 qor_pp_contract_make_entry_0_with_attrs
#define qor_pp_contract_make_entry_0_4 qor_pp_contract_make_entry_0_with_attrs
#define qor_pp_contract_make_entry_0_5 qor_pp_contract_make_entry_0_with_attrs
#define qor_pp_contract_make_entry_0_6 qor_pp_contract_make_entry_0_with_attrs
#define qor_pp_contract_make_entry_0_7 qor_pp_contract_make_entry_0_with_attrs
#define qor_pp_contract_make_entry_0_8 qor_pp_contract_make_entry_0_with_attrs
#define qor_pp_contract_make_entry_0_9 qor_pp_contract_make_entry_0_with_attrs
#define qor_pp_contract_make_entry_0_10 qor_pp_contract_make_entry_0_with_attrs
#define qor_pp_contract_make_entry_0_11 qor_pp_contract_make_entry_0_with_attrs
#define qor_pp_contract_make_entry_0_12 qor_pp_contract_make_entry_0_with_attrs
#define qor_pp_contract_make_entry_0_13 qor_pp_contract_make_entry_0_with_attrs
#define qor_pp_contract_make_entry_0_14 qor_pp_contract_make_entry_0_with_attrs
#define qor_pp_contract_make_entry_0_15 qor_pp_contract_make_entry_0_with_attrs
#define qor_pp_contract_make_entry_0_16 qor_pp_contract_make_entry_0_with_attrs

#define qor_pp_contract_make_entry_1(contract_self_type, entry) \
    qor::contract::base<qor_pp_contract_base_type(entry), qor_pp_contract_base_offset(entry)>{}

#define qor_pp_contract_make_entry_2(contract_self_type, entry) \
    qor_pp_contract_make_reference_entry_expand(contract_self_type, qor_pp_contract_unparen entry)

#define qor_pp_contract_make_reference_entry_expand(contract_self_type, ...) \
    qor_pp_contract_cat(qor_pp_contract_make_reference_entry_, qor_pp_contract_narg(__VA_ARGS__))(contract_self_type, __VA_ARGS__)

#define qor_pp_contract_make_reference_entry_3(contract_self_type, marker, name, id) \
    typename contract_fields::name{qor_pp_contract_stringize(name), qor_pp_contract_make_field_attributes()}

#define qor_pp_contract_make_reference_entry_with_attrs(contract_self_type, marker, name, id, ...) \
    typename contract_fields::name{qor_pp_contract_stringize(name), qor_pp_contract_make_field_attributes(__VA_ARGS__)}

#define qor_pp_contract_make_reference_entry_4 qor_pp_contract_make_reference_entry_with_attrs
#define qor_pp_contract_make_reference_entry_5 qor_pp_contract_make_reference_entry_with_attrs
#define qor_pp_contract_make_reference_entry_6 qor_pp_contract_make_reference_entry_with_attrs
#define qor_pp_contract_make_reference_entry_7 qor_pp_contract_make_reference_entry_with_attrs
#define qor_pp_contract_make_reference_entry_8 qor_pp_contract_make_reference_entry_with_attrs
#define qor_pp_contract_make_reference_entry_9 qor_pp_contract_make_reference_entry_with_attrs
#define qor_pp_contract_make_reference_entry_10 qor_pp_contract_make_reference_entry_with_attrs
#define qor_pp_contract_make_reference_entry_11 qor_pp_contract_make_reference_entry_with_attrs
#define qor_pp_contract_make_reference_entry_12 qor_pp_contract_make_reference_entry_with_attrs
#define qor_pp_contract_make_reference_entry_13 qor_pp_contract_make_reference_entry_with_attrs
#define qor_pp_contract_make_reference_entry_14 qor_pp_contract_make_reference_entry_with_attrs
#define qor_pp_contract_make_reference_entry_15 qor_pp_contract_make_reference_entry_with_attrs
#define qor_pp_contract_make_reference_entry_16 qor_pp_contract_make_reference_entry_with_attrs

// Property entries are still descriptors, not storage. Their value access is
// resolved by hooks in the field layer.
#define qor_pp_contract_make_entry_3(contract_self_type, entry) \
    qor_pp_contract_make_entry_2_expand(contract_self_type, qor_pp_contract_unparen entry)

#define qor_pp_contract_make_entry_2_expand(contract_self_type, ...) \
    qor_pp_contract_cat(qor_pp_contract_make_entry_2_, qor_pp_contract_narg(__VA_ARGS__))( \
        contract_self_type, __VA_ARGS__)

#define qor_pp_contract_make_entry_2_4(contract_self_type, marker, name, id, type) \
    qor::contract::make_property_field<contract_self_type, id, type>( \
            qor_pp_contract_stringize(name), \
            qor_pp_contract_make_field_attributes())

#define qor_pp_contract_make_entry_2_with_attrs(contract_self_type, marker, name, id, type, ...) \
    qor::contract::make_property_field<contract_self_type, id, type>( \
            qor_pp_contract_stringize(name), \
            qor_pp_contract_make_field_attributes(__VA_ARGS__))

#define qor_pp_contract_make_entry_2_5 qor_pp_contract_make_entry_2_with_attrs
#define qor_pp_contract_make_entry_2_6 qor_pp_contract_make_entry_2_with_attrs
#define qor_pp_contract_make_entry_2_7 qor_pp_contract_make_entry_2_with_attrs
#define qor_pp_contract_make_entry_2_8 qor_pp_contract_make_entry_2_with_attrs
#define qor_pp_contract_make_entry_2_9 qor_pp_contract_make_entry_2_with_attrs
#define qor_pp_contract_make_entry_2_10 qor_pp_contract_make_entry_2_with_attrs
#define qor_pp_contract_make_entry_2_11 qor_pp_contract_make_entry_2_with_attrs
#define qor_pp_contract_make_entry_2_12 qor_pp_contract_make_entry_2_with_attrs
#define qor_pp_contract_make_entry_2_13 qor_pp_contract_make_entry_2_with_attrs
#define qor_pp_contract_make_entry_2_14 qor_pp_contract_make_entry_2_with_attrs
#define qor_pp_contract_make_entry_2_15 qor_pp_contract_make_entry_2_with_attrs
#define qor_pp_contract_make_entry_2_16 qor_pp_contract_make_entry_2_with_attrs

#define qor_pp_contract_define(contract_self_type, make_contract_expression, ...) \
    struct contract_fields { \
        qor_pp_contract_for_each_arg(qor_pp_contract_declare_field, contract_self_type, __VA_ARGS__) \
    }; \
    friend constexpr auto contract_definition(qor::contract::tag<contract_self_type>) { \
        return make_contract_expression( \
            qor_pp_contract_stringize(contract_self_type), \
            qor_pp_contract_for_each_comma_arg(qor_pp_contract_make_entry, contract_self_type, __VA_ARGS__)); \
    }

#define qor_pp_contract_impl_0(contract_self_type, ...) \
    qor_pp_contract_define( \
        contract_self_type, \
        qor::contract::make_contract<contract_self_type>, \
        __VA_ARGS__)

// The first entry decides whether the contract has a top-level ATTRS(...) pack.
#define qor_pp_contract_dispatch(contract_self_type, has_attributes, ...) \
    qor_pp_contract_cat(qor_pp_contract_dispatch_, has_attributes)(contract_self_type, __VA_ARGS__)

// The first entry decides whether the contract starts with ATTRS(...).
#define qor_pp_contract_dispatch_0(contract_self_type, ...) \
    qor_pp_contract_impl_0(contract_self_type, __VA_ARGS__)

#define qor_pp_contract_dispatch_1(contract_self_type, attrs_entry, ...) \
    qor_pp_contract_define_with_attributes( \
        contract_self_type, \
        attrs_entry, \
        __VA_ARGS__)

#define qor_pp_contract_define_with_attributes(contract_self_type, attrs_entry, ...) \
    struct contract_fields { \
        qor_pp_contract_for_each_arg(qor_pp_contract_declare_field, contract_self_type, __VA_ARGS__) \
    }; \
    friend constexpr auto contract_definition(qor::contract::tag<contract_self_type>) { \
        return qor::contract::make_contract_with_attributes<contract_self_type>( \
            qor_pp_contract_stringize(contract_self_type), \
            qor::contract::make_contract_attributes( \
                qor_pp_contract_for_each_comma( \
                    qor_pp_contract_describe_attribute, \
                    qor_pp_contract_attrs_values(attrs_entry))), \
            qor_pp_contract_for_each_comma_arg(qor_pp_contract_make_entry, contract_self_type, __VA_ARGS__)); \
    }

#define qor_pp_contract_contract_0(contract_self_type, ...) \
    friend constexpr auto contract_definition(qor::contract::tag<contract_self_type>) { \
        return qor::contract::make_contract<contract_self_type>( \
            qor_pp_contract_stringize(contract_self_type)); \
    }

#define qor_pp_contract_contract_1(contract_self_type, ...) \
    qor_pp_contract_dispatch( \
        contract_self_type, \
        qor_pp_contract_entry_is_attrs(qor_pp_contract_first(__VA_ARGS__)), \
        __VA_ARGS__)

#define qor_pp_contract(contract_self_type, ...) \
    qor_pp_contract_cat(qor_pp_contract_contract_, qor_pp_contract_bool(qor_pp_contract_narg(__VA_ARGS__)))( \
        contract_self_type, __VA_ARGS__)

#endif//QOR_PP_H_CONTRACT_MACROS
