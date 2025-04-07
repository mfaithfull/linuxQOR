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

#ifndef QOR_PP_H_REFLECTION_DETAIL_DETECTORS
#define QOR_PP_H_REFLECTION_DETAIL_DETECTORS

#pragma once

#include "config.h"

#ifdef qor_pp_refl_has_std_module
import std;
#else
#include <functional>
#include <type_traits>
#endif

namespace qor_reflection { namespace detail {

    ///////////////////// `value` is true if Detector<Tleft, Tright> does not compile (SFINAE)
    struct can_not_apply{};

    template <template <class, class> class Detector, class Tleft, class Tright>
    struct not_appliable 
    {
        static constexpr bool value = std::is_same<
            Detector<Tleft, Tright>,
            can_not_apply
        >::value;
    };

    ///////////////////// Detectors for different operators
    template <class S, class T> auto comp_eq_detector_msvc_helper(long) -> decltype(std::declval<S>() == std::declval<T>());
    template <class S, class T> can_not_apply comp_eq_detector_msvc_helper(int);
    template <class T1, class T2> using comp_eq_detector = decltype(comp_eq_detector_msvc_helper<T1,T2>(1L));

    template <class S, class T> auto comp_ne_detector_msvc_helper(long) -> decltype(std::declval<S>() != std::declval<T>());
    template <class S, class T> can_not_apply comp_ne_detector_msvc_helper(int);
    template <class T1, class T2> using comp_ne_detector = decltype(comp_ne_detector_msvc_helper<T1,T2>(1L));

    template <class S, class T> auto comp_lt_detector_msvc_helper(long) -> decltype(std::declval<S>() < std::declval<T>());
    template <class S, class T> can_not_apply comp_lt_detector_msvc_helper(int);
    template <class T1, class T2> using comp_lt_detector = decltype(comp_lt_detector_msvc_helper<T1,T2>(1L));

    template <class S, class T> auto comp_le_detector_msvc_helper(long) -> decltype(std::declval<S>() <= std::declval<T>());
    template <class S, class T> can_not_apply comp_le_detector_msvc_helper(int);
    template <class T1, class T2> using comp_le_detector = decltype(comp_le_detector_msvc_helper<T1,T2>(1L));

    template <class S, class T> auto comp_gt_detector_msvc_helper(long) -> decltype(std::declval<S>() > std::declval<T>());
    template <class S, class T> can_not_apply comp_gt_detector_msvc_helper(int);
    template <class T1, class T2> using comp_gt_detector = decltype(comp_gt_detector_msvc_helper<T1,T2>(1L));

    template <class S, class T> auto comp_ge_detector_msvc_helper(long) -> decltype(std::declval<S>() >= std::declval<T>());
    template <class S, class T> can_not_apply comp_ge_detector_msvc_helper(int);
    template <class T1, class T2> using comp_ge_detector = decltype(comp_ge_detector_msvc_helper<T1,T2>(1L));


    template <class S> auto hash_detector_msvc_helper(long) -> decltype(std::hash<S>{}(std::declval<S>()));
    template <class S> can_not_apply hash_detector_msvc_helper(int);
    template <class T1, class T2> using hash_detector = decltype(hash_detector_msvc_helper<T1,T2>(1L));


    template <class S, class T> auto ostreamable_detector_msvc_helper(long) -> decltype(std::declval<S>() << std::declval<T>());
    template <class S, class T> can_not_apply ostreamable_detector_msvc_helper(int);
    template <class S, class T> using ostreamable_detector = decltype(ostreamable_detector_msvc_helper<S,T>(1L));

    template <class S, class T> auto istreamable_detector_msvc_helper(long) -> decltype(std::declval<S>() >> std::declval<T>());
    template <class S, class T> can_not_apply istreamable_detector_msvc_helper(int);
    template <class S, class T> using istreamable_detector = decltype(istreamable_detector_msvc_helper<S,T>(1L));

}}//qor_reflection::detail

#endif//QOR_PP_H_REFLECTION_DETAIL_DETECTORS
