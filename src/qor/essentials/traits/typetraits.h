// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ESSENTIALS_TRAITS_TYPETRAITS
#define QOR_PP_H_ESSENTIALS_TRAITS_TYPETRAITS

#include <type_traits>

namespace qor{

    template<class T, template<class...> class U>
    inline constexpr bool is_instance_of_v = std::false_type{};

    template< template<class...> class U, class... Vs>
    inline constexpr bool is_instance_of_v<U<Vs...>,U> = std::true_type{};

}//qor

#endif//QOR_PP_H_ESSENTIALS_TRAITS_TYPETRAITS