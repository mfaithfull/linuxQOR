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

#ifndef QOR_PP_H_REFLECTION_DETAIL_IO
#define QOR_PP_H_REFLECTION_DETAIL_IO

#pragma once

#include "config.h"
#include "sequence_tuple.h"

#ifdef qor_pp_refl_has_std_module
import std;
#else
#include <iosfwd>       // stream operators
#include <iomanip>

#if defined(__has_include)
#   if __has_include(<string_view>) && qor_pp_refl_use_cpp17
#       include <string_view>
#   endif
#endif

#endif

namespace qor_reflection { namespace detail {

    inline auto quoted_helper(const std::string& s) noexcept 
    {
        return std::quoted(s);
    }

#if defined(__has_include)
#   if __has_include(<string_view>) && qor_pp_refl_use_cpp17
    template <class CharT, class Traits>
    inline auto quoted_helper(std::basic_string_view<CharT, Traits> s) noexcept 
    {
        return std::quoted(s);
    }
#   endif
#endif

    inline auto quoted_helper(std::string& s) noexcept 
    {
        return std::quoted(s);
    }

    template <class T>
    inline decltype(auto) quoted_helper(T&& v) noexcept 
    {
        return std::forward<T>(v);
    }

    template <std::size_t I, std::size_t N>
    struct print_impl 
    {
        template <class Stream, class T>
        static void print (Stream& out, const T& value) 
        {
            if (!!I) out << ", ";
            out << detail::quoted_helper(qor_reflection::detail::sequence_tuple::get<I>(value));
            print_impl<I + 1, N>::print(out, value);
        }
    };

    template <std::size_t I>
    struct print_impl<I, I> 
    {
        template <class Stream, class T> static void print (Stream&, const T&) noexcept {}
    };

    template <std::size_t I, std::size_t N>
    struct read_impl 
    {
        template <class Stream, class T>
        static void read (Stream& in, const T& value) 
        {
            char ignore = {};
            if (!!I) 
            {
                in >> ignore;
                if (ignore != ',') in.setstate(Stream::failbit);
                in >> ignore;
                if (ignore != ' ')  in.setstate(Stream::failbit);
            }
            in >> detail::quoted_helper( qor_reflection::detail::sequence_tuple::get<I>(value) );
            read_impl<I + 1, N>::read(in, value);
        }
    };

    template <std::size_t I>
    struct read_impl<I, I> 
    {
        template <class Stream, class T> static void read (Stream&, const T&) {}
    };

}}//qor_reflection::detail

#endif//QOR_PP_H_REFLECTION_DETAIL_IO
