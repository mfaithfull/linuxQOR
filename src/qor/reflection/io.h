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

#ifndef QOR_PP_H_REFLECTION_IO
#define QOR_PP_H_REFLECTION_IO

#pragma once

#include "detail/config.h"
#include "detail/detectors.h"
#include "io_fields.h"

/// \file pfr/io.hpp
/// Contains IO stream manipulator \forcedlink{io} for types.
/// If type is streamable using its own operator or its conversion operator, then the types operator is used.
///
/// \b Example:
/// \code
///     #include <pfr/io.hpp>
///     struct comparable_struct {      // No operators defined for that structure
///         int i; short s; char data[7]; bool bl; int a,b,c,d,e,f;
///     };
///     // ...
///
///     comparable_struct s1 {0, 1, "Hello", false, 6,7,8,9,10,11};
///     std::cout << pfr::io(s1);  // Outputs: {0, 1, H, e, l, l, o, , , 0, 6, 7, 8, 9, 10, 11}
/// \endcode
///
/// \podops for other ways to define operators and more details.
///
/// \b Synopsis:
namespace pfr {

namespace detail {

///////////////////// Helper typedefs
    template <class Stream, class Type>
    using enable_not_ostreamable_t = std::enable_if_t<
        not_appliable<ostreamable_detector, Stream&, const std::remove_reference_t<Type>&>::value,
        Stream&
    >;

    template <class Stream, class Type>
    using enable_not_istreamable_t = std::enable_if_t<
        not_appliable<istreamable_detector, Stream&, Type&>::value,
        Stream&
    >;

    template <class Stream, class Type>
    using enable_ostreamable_t = std::enable_if_t<
        !not_appliable<ostreamable_detector, Stream&, const std::remove_reference_t<Type>&>::value,
        Stream&
    >;

    template <class Stream, class Type>
    using enable_istreamable_t = std::enable_if_t<
        !not_appliable<istreamable_detector, Stream&, Type&>::value,
        Stream&
    >;

///////////////////// IO impl

template <class T>
struct io_impl {
    T value;
};

qor_pp_refl_begin_module_export

template <class Char, class Traits, class T>
enable_not_ostreamable_t<std::basic_ostream<Char, Traits>, T> operator<<(std::basic_ostream<Char, Traits>& out, io_impl<T>&& x) {
    return out << pfr::io_fields(std::forward<T>(x.value));
}

template <class Char, class Traits, class T>
enable_ostreamable_t<std::basic_ostream<Char, Traits>, T> operator<<(std::basic_ostream<Char, Traits>& out, io_impl<T>&& x) {
    return out << x.value;
}

template <class Char, class Traits, class T>
enable_not_istreamable_t<std::basic_istream<Char, Traits>, T> operator>>(std::basic_istream<Char, Traits>& in, io_impl<T>&& x) {
    return in >> pfr::io_fields(std::forward<T>(x.value));
}

template <class Char, class Traits, class T>
enable_istreamable_t<std::basic_istream<Char, Traits>, T> operator>>(std::basic_istream<Char, Traits>& in, io_impl<T>&& x) {
    return in >> x.value;
}

qor_pp_refl_end_module_export

} // namespace detail

qor_pp_refl_begin_module_export

/// IO manipulator to read/write \aggregate `value` using its IO stream operators or using \forcedlink{io_fields} if operators are not available.
///
/// \b Example:
/// \code
///     struct my_struct { int i; short s; };
///     my_struct x;
///     std::stringstream ss;
///     ss << "{ 12, 13 }";
///     ss >> pfr::io(x);
///     assert(x.i == 12);
///     assert(x.s == 13);
/// \endcode
///
/// \customio
template <class T>
auto io(T&& value) noexcept {
    return detail::io_impl<T>{std::forward<T>(value)};
}

qor_pp_refl_end_module_export

} // namespace pfr

#endif//QOR_PP_H_REFLECTION_IO