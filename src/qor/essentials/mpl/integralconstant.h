// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_INTEGRAL_CONSTANT
#define QOR_PP_H_MPL_INTEGRAL_CONSTANT

namespace qor { namespace mpl {

    template <typename T, T val>
    struct integral_constant 
    {
        static constexpr T value = val;
    };

    namespace eager 
    {
        template <typename T>
        struct is_integral 
        {
            static constexpr bool value = false;
        };

        template <typename T, T val>
        struct is_integral<integral_constant<T, val>> 
        {
            static constexpr bool value = true;
        };

        template <typename T>
        struct is_integral<integral_constant<T, nullptr>> 
        {
            static constexpr bool value = true;
        };

        template <>
        struct is_integral<integral_constant<decltype(nullptr), nullptr>> 
        {
            static constexpr bool value = true;
        };

    }//eager

}}//qor::mpl

#endif//QOR_PP_H_MPL_INTEGRAL_CONSTANT