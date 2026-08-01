// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from the Frozen library by QuarksLab
/*
 * Frozen
 * Copyright 2016 QuarksLab
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef QOR_PP_H_DATASTRUCTURES_FROZEN_STRING
#define QOR_PP_H_DATASTRUCTURES_FROZEN_STRING

#include "bits/elsa.h"
#include "bits/hash_string.h"
#include "bits/defines.h"

#include <cstddef>
#include <stdint.h>

#ifdef qor_pp_has_string_view
#include <string_view>
#endif

namespace qor { namespace frozen {

    template <typename _CharT>
    class basic_string 
    {
        using chr_t = _CharT;
        chr_t const *data_;
        std::size_t size_;

    public:

        template <std::size_t N>
        constexpr basic_string(chr_t const (&data)[N]) : data_(data), size_(N - 1) 
        { }

        constexpr basic_string(chr_t const *data, std::size_t size) : data_(data), size_(size) 
        { }

#ifdef qor_pp_has_string_view
        constexpr basic_string(std::basic_string_view<chr_t> data) : data_(data.data()), size_(data.size()) 
        { }

        explicit constexpr operator std::basic_string_view<chr_t>() const 
        {
            return std::basic_string_view<chr_t>(data_, size_);
        }
#endif

        constexpr basic_string(const basic_string &) noexcept = default;
        constexpr basic_string &operator=(const basic_string &) noexcept = default;

        constexpr std::size_t length() const { return size_; }
        constexpr std::size_t size() const { return size_; }

        constexpr chr_t operator[](std::size_t i) const { return data_[i]; }

        constexpr bool operator==(basic_string other) const 
        {
            if (size_ != other.size_)
            {
                return false;
            }
            for (std::size_t i = 0; i < size_; ++i)
            {
                if (data_[i] != other.data_[i])
                {
                    return false;
                }
            }
            return true;
        }

        constexpr bool operator<(const basic_string &other) const 
        {
            unsigned i = 0;
            while (i < size() && i < other.size()) 
            {
                if ((*this)[i] < other[i]) 
                {
                    return true;
                }
                if ((*this)[i] > other[i]) 
                {
                    return false;
                }
                ++i;
            }
            return size() < other.size();
        }

        friend constexpr bool operator>(const basic_string& lhs, const basic_string& rhs) 
        {
            return rhs < lhs;
        }

        friend constexpr bool operator>=(const basic_string& lhs, const basic_string& rhs) 
        {
            return !(lhs < rhs);
        }

        friend constexpr bool operator<=(const basic_string& lhs, const basic_string& rhs) 
        {
            return !(lhs > rhs);
        }

        constexpr const chr_t *data() const { return data_; }
        constexpr const chr_t *begin() const { return data(); }
        constexpr const chr_t *end() const { return data() + size(); }
    };

    template <typename _CharT> struct elsa<basic_string<_CharT>> 
    {
        constexpr std::size_t operator()(basic_string<_CharT> value) const 
        {
            return hash_string(value);
        }
        constexpr std::size_t operator()(basic_string<_CharT> value, std::size_t seed) const 
        {
            return hash_string(value, seed);
        }
    };

    using string = basic_string<char>;
    using wstring = basic_string<wchar_t>;
    using u16string = basic_string<char16_t>;
    using u32string = basic_string<char32_t>;

#ifdef qor_pp_has_char8_t
    using u8string = basic_string<char8_t>;
#endif

    namespace string_literals {

    constexpr string operator""_s(const char *data, std::size_t size) 
    {
        return {data, size};
    }

    constexpr wstring operator""_s(const wchar_t *data, std::size_t size) 
    {
        return {data, size};
    }

    constexpr u16string operator""_s(const char16_t *data, std::size_t size) 
    {
        return {data, size};
    }

    constexpr u32string operator""_s(const char32_t *data, std::size_t size) 
    {
        return {data, size};
    }

#ifdef qor_pp_has_char8_t
    constexpr u8string operator""_s(const char8_t *data, std::size_t size) 
    {
        return {data, size};
    }
#endif

    }//string_literals

}}//qor::frozen

namespace std 
{
    template <typename _CharT> struct hash<qor::frozen::basic_string<_CharT>> 
    {
        std::size_t operator()(qor::frozen::basic_string<_CharT> s) const 
        {
            return qor::frozen::elsa<qor::frozen::basic_string<_CharT>>{}(s);
        }
    };
} // namespace std

#endif//QOR_PP_H_DATASTRUCTURES_FROZEN_STRING
