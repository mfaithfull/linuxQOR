// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from the Frozen library by QuarksLab

#ifndef QOR_PP_H_DATASTRUCTURES_FROZEN_BITS_ELSA_STD
#define QOR_PP_H_DATASTRUCTURES_FROZEN_BITS_ELSA_STD

#include "defines.h"
#include "elsa.h"
#include "hash_string.h"

#ifdef qor_pp_has_string_view
#include <string_view>
#endif
#include <string>

namespace qor { namespace frozen {

#ifdef qor_pp_has_string_view

    template <typename CharT> struct elsa<std::basic_string_view<CharT>>
    {
        constexpr std::size_t operator()(const std::basic_string_view<CharT>& value) const {
            return hash_string(value);
        }
        constexpr std::size_t operator()(const std::basic_string_view<CharT>& value, std::size_t seed) const {
            return hash_string(value, seed);
        }
    };

#endif

    template <typename CharT> struct elsa<std::basic_string<CharT>>
    {
        constexpr std::size_t operator()(const std::basic_string<CharT>& value) const {
            return hash_string(value);
        }
        constexpr std::size_t operator()(const std::basic_string<CharT>& value, std::size_t seed) const {
            return hash_string(value, seed);
        }
    };

}}//qor::frozen

#endif//QOR_PP_H_DATASTRUCTURES_FROZEN_BITS_ELSA_STD
