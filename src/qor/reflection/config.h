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

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_CONFIG
#define QOR_PP_H_REFLECTION_CONFIG
#pragma once

#if __cplusplus >= 201402L || (defined(_MSC_VER) && defined(_MSVC_LANG) && _MSC_VER > 1900)
#include <type_traits> // to get non standard platform macro definitions (__GLIBCXX__ for example)
#endif

// Contains all the macros that describe QOR Reflection configuration, like qor_pp_refl_enabled
//
// This header file doesn't require C++14 Standard and supports all C++ compilers, even pre C++14 compilers (C++11, C++03...).

// Reminder:
//  * MSVC++ 14.2 _MSC_VER == 1927 <- Loophole is known to work (Visual Studio ????)
//  * MSVC++ 14.1 _MSC_VER == 1916 <- Loophole is known to NOT work (Visual Studio 2017)
//  * MSVC++ 14.0 _MSC_VER == 1900 (Visual Studio 2015)
//  * MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)

#ifdef qor_pp_refl_not_supported
#   error Please, do not set qor_pp_refl_not_supported value manually, use '-Dqor_pp_refl_enabled=0' instead of it
#endif

#if defined(_MSC_VER)
#   if !defined(_MSVC_LANG) || _MSC_VER <= 1900
#       define qor_pp_refl_not_supported 1
#   endif
#elif __cplusplus < 201402L
#   define qor_pp_refl_not_supported 1
#endif

#ifndef qor_pp_refl_use_loophole
#   if defined(_MSC_VER)
#       if _MSC_VER >= 1927
#           define qor_pp_refl_use_loophole 1
#       else
#           define qor_pp_refl_use_loophole 0
#       endif
#   elif defined(__clang_major__) && __clang_major__ >= 8
#       define qor_pp_refl_use_loophole 0
#   else
#       define qor_pp_refl_use_loophole 1
#   endif
#endif

#ifndef qor_pp_refl_use_cpp17
#   ifdef __cpp_structured_bindings
#       define qor_pp_refl_use_cpp17 1
#   elif defined(_MSVC_LANG)
#       if _MSVC_LANG >= 201703L
#           define qor_pp_refl_use_cpp17 1
#       else
#           define qor_pp_refl_use_cpp17 0
#       endif
#   else
#       define qor_pp_refl_use_cpp17 0
#   endif
#endif

#if (!qor_pp_refl_use_cpp17 && !qor_pp_refl_use_loophole)
#   if (defined(_MSC_VER) && _MSC_VER < 1916) ///< in Visual Studio 2017 v15.9 PFR library with classic engine normally works
#      define qor_pp_refl_not_supported 1
#   endif
#endif

#ifndef qor_pp_refl_use_std_make_integral_sequence
// Assume that libstdc++ since GCC-7.3 does not have linear instantiation depth in std::make_integral_sequence
#   if defined( __GLIBCXX__) && __GLIBCXX__ >= 20180101
#       define qor_pp_refl_use_std_make_integral_sequence 1
#   elif defined(_MSC_VER)
#       define qor_pp_refl_use_std_make_integral_sequence 1
//# elif other known working lib
#   else
#       define qor_pp_refl_use_std_make_integral_sequence 0
#   endif
#endif

#ifndef qor_pp_refl_has_guaranteed_copy_elision
#   if  defined(__cpp_guaranteed_copy_elision) && (!defined(_MSC_VER) || _MSC_VER > 1928)
#       define qor_pp_refl_has_guaranteed_copy_elision 1
#   else
#       define qor_pp_refl_has_guaranteed_copy_elision 0
#   endif
#endif

#ifndef qor_pp_refl_enable_implicit_reflection
#   if  defined(__cpp_lib_is_aggregate)
#       define qor_pp_refl_enable_implicit_reflection 1
#   else
// There is no way to detect potential ability to be reflectable without std::is_aggregare
#       define qor_pp_refl_enable_implicit_reflection 0
#   endif
#endif

#ifndef qor_pp_refl_name_enabled
#   if  (__cplusplus >= 202002L) || (defined(_MSVC_LANG) && (_MSVC_LANG >= 202002L))
#       if (defined(__cpp_nontype_template_args) && __cpp_nontype_template_args >= 201911) \
         || (defined(__clang_major__) && __clang_major__ >= 12)
#           define qor_pp_refl_name_enabled 1
#       else
#           define qor_pp_refl_name_enabled 0
#       endif
#   else
#       define qor_pp_refl_name_enabled 0
#   endif
#endif


#ifndef qor_pp_refl_core_name_parsing
#   if defined(_MSC_VER) && !defined(__clang__)
#       define qor_pp_refl_core_name_parsing (sizeof("auto __cdecl qor_reflection::detail::name_of_field_impl<") - 1, sizeof(">(void) noexcept") - 1, backward("->"))
#   elif defined(__clang__)
#       define qor_pp_refl_core_name_parsing (sizeof("auto qor_reflection::detail::name_of_field_impl() [MsvcWorkaround = ") - 1, sizeof("}]") - 1, backward("."))
#   elif defined(__GNUC__)
#       define qor_pp_refl_core_name_parsing (sizeof("consteval auto qor_reflection::detail::name_of_field_impl() [with MsvcWorkaround = ") - 1, sizeof(")]") - 1, backward("::"))
#   else
// Default parser for other platforms... Just skip nothing!
#       define qor_pp_refl_core_name_parsing (0, 0, "")
#   endif
#endif

#if defined(__has_cpp_attribute)
#   if __has_cpp_attribute(maybe_unused)
#       define qor_pp_refl_maybe_unused [[maybe_unused]]
#   endif
#endif

#ifndef qor_pp_refl_maybe_unused
#   define qor_pp_refl_maybe_unused
#endif

#ifndef qor_pp_refl_enabled
#   ifdef qor_pp_refl_not_supported
#       define qor_pp_refl_enabled 0
#   else
#       define qor_pp_refl_enabled 1
#   endif
#endif

#undef qor_pp_refl_not_supported

#ifndef qor_pp_refl_begin_module_export
#   define qor_pp_refl_begin_module_export
#endif

#ifndef qor_pp_refl_end_module_export
#   define qor_pp_refl_end_module_export
#endif

#endif // QOR_PP_H_REFLECTION_CONFIG
