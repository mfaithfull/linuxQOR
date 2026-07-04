// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_TRAITS
#define QOR_PP_H_REFLECTION_TRAITS

#pragma once

#include "detail/config.h"

#include "detail/possible_reflectable.h"
#include <type_traits>

// Contains traits is_reflectable and is_implicitly_reflectable for detecting an ability to reflect type.

namespace qor_reflection {

qor_pp_refl_begin_module_export

    // Has a static const member variable `value` when it is known that type T can or can't be reflected using QOR Reflection; otherwise, there is no member variable.
    // Every user may (and in some difficult cases - should) specialize is_reflectable on his own.

    //     namespace qor_reflection {
    //         template<class All> struct is_reflectable<A, All> : std::false_type {};       // 'A' won't be interpreted as reflectable everywhere
    //         template<> struct is_reflectable<B, boost_fusion_tag> : std::false_type {};   // 'B' won't be interpreted as reflectable in only Boost Fusion
    //     }}
    // is_reflectable affects is_implicitly_reflectable, the decision made by is_reflectable is used by is_implicitly_reflectable.
    template<class T, class WhatFor>
    struct is_reflectable { /*does not have 'value' because value is unknown*/ };

    // these specs can't be inherited from 'std::integral_constant< bool, qor_reflection::is_reflectable<T, WhatFor>::value >',
    // because it will break the sfinae-friendliness
    template<class T, class WhatFor>
    struct is_reflectable<const T, WhatFor> : qor_reflection::is_reflectable<T, WhatFor> {};

    template<class T, class WhatFor>
    struct is_reflectable<volatile T, WhatFor> : qor_reflection::is_reflectable<T, WhatFor> {};

    template<class T, class WhatFor>
    struct is_reflectable<const volatile T, WhatFor> : qor_reflection::is_reflectable<T, WhatFor> {};

    // Checks the input type for the potential to be reflected.
    // Specialize is_reflectable if you disagree with is_implicitly_reflectable's default decision.
    template<class T, class WhatFor>
    using is_implicitly_reflectable = std::integral_constant< bool, qor_reflection::detail::possible_reflectable<T, WhatFor>(1L) >;

    // Checks the input type for the potential to be reflected.
    // Specialize is_reflectable if you disagree with is_implicitly_reflectable_v's default decision.
    template<class T, class WhatFor>
    constexpr bool is_implicitly_reflectable_v = is_implicitly_reflectable<T, WhatFor>::value;

    qor_pp_refl_end_module_export

} // namespace qor_reflection

#endif//QOR_PP_H_REFLECTION_TRAITS