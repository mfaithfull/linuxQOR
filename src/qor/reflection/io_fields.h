// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright (c) 2016-2024 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QOR_PP_H_REFLECTION_IOFIELDS
#define QOR_PP_H_REFLECTION_IOFIELDS

#pragma once

#include "detail/config.h"
#include "detail/core.h"

#include <type_traits>
#include <utility>      // metaprogramming stuff

#include "detail/sequence_tuple.h"
#include "detail/io.h"
#include "detail/make_integer_sequence.h"
#include "tuple_size.h"

// Contains IO manipulator io_fields to read/write any aggregate field-by-field.
//
//     struct my_struct {
//         int i;
//         short s;
//     };
//
//     std::ostream& operator<<(std::ostream& os, const my_struct& x) {
//         return os << qor_reflection::io_fields(x);  // Equivalent to: os << "{ " << x.i << " ," <<  x.s << " }"
//     }
//
//     std::istream& operator>>(std::istream& is, my_struct& x) {
//         return is >> qor_reflection::io_fields(x);  // Equivalent to: is >> "{ " >> x.i >> " ," >>  x.s >> " }"
//     }

namespace qor_reflection { namespace detail {

        template <class T>
        struct io_fields_impl 
        {
            T value;
        };


        template <class Char, class Traits, class T>
        std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& out, io_fields_impl<const T&>&& x) 
        {
            const T& value = x.value;
            constexpr std::size_t fields_count_val = qor_reflection::detail::fields_count<T>();
            out << '{';
#if qor_pp_refl_use_cpp17 || qor_pp_refl_use_loophole
            detail::print_impl<0, fields_count_val>::print(out, detail::tie_as_tuple(value));
#else
            ::qor_reflection::detail::for_each_field_dispatcher(
                value,
                [&out](const auto& val) {
                    // We can not reuse `fields_count_val` in lambda because compilers had issues with
                    // passing constexpr variables into lambdas. Computing is again is the most portable solution.
                    constexpr std::size_t fields_count_val_lambda = qor_reflection::detail::fields_count<T>();
                    detail::print_impl<0, fields_count_val_lambda>::print(out, val);
                },
                detail::make_index_sequence<fields_count_val>{}
            );
#endif
            return out << '}';
        }


        template <class Char, class Traits, class T>
        std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& out, io_fields_impl<T>&& x) 
        {
            return out << io_fields_impl<const std::remove_reference_t<T>&>{x.value};
        }

        template <class Char, class Traits, class T>
        std::basic_istream<Char, Traits>& operator>>(std::basic_istream<Char, Traits>& in, io_fields_impl<T&>&& x) 
        {
            T& value = x.value;
            constexpr std::size_t fields_count_val = qor_reflection::detail::fields_count<T>();

            const auto prev_exceptions = in.exceptions();
            in.exceptions( typename std::basic_istream<Char, Traits>::iostate(0) );
            const auto prev_flags = in.flags( typename std::basic_istream<Char, Traits>::fmtflags(0) );

            char parenthis = {};
            in >> parenthis;
            if (parenthis != '{') in.setstate(std::basic_istream<Char, Traits>::failbit);

#if qor_pp_refl_use_cpp17 || qor_pp_refl_use_loophole
            detail::read_impl<0, fields_count_val>::read(in, detail::tie_as_tuple(value));
#else
            ::qor_reflection::detail::for_each_field_dispatcher(
                value,
                [&in](const auto& val) {
                    // We can not reuse `fields_count_val` in lambda because compilers had issues with
                    // passing constexpr variables into lambdas. Computing is again is the most portable solution.
                    constexpr std::size_t fields_count_val_lambda = qor_reflection::detail::fields_count<T>();
                    detail::read_impl<0, fields_count_val_lambda>::read(in, val);
                },
                detail::make_index_sequence<fields_count_val>{}
            );
#endif

            in >> parenthis;
            if (parenthis != '}') in.setstate(std::basic_istream<Char, Traits>::failbit);

            in.flags(prev_flags);
            in.exceptions(prev_exceptions);

            return in;
        }

        qor_pp_refl_begin_module_export

        template <class Char, class Traits, class T>
        std::basic_istream<Char, Traits>& operator>>(std::basic_istream<Char, Traits>& in, io_fields_impl<const T&>&& ) 
        {
            static_assert(sizeof(T) && false, "====================> QOR Reflection: Attempt to use istream operator on a qor_reflection::io_fields wrapped type T with const qualifier.");
            return in;
        }

        template <class Char, class Traits, class T>
        std::basic_istream<Char, Traits>& operator>>(std::basic_istream<Char, Traits>& in, io_fields_impl<T>&& ) 
        {
            static_assert(sizeof(T) && false, "====================> QOR Reflection: Attempt to use istream operator on a qor_reflection::io_fields wrapped temporary of type T.");
            return in;
        }

        qor_pp_refl_end_module_export

    }//detail

    qor_pp_refl_begin_module_export

    // IO manipulator to read/write aggregate `value` field-by-field.
    //
    //     struct my_struct {
    //         int i;
    //         short s;
    //     };
    //
    //     std::ostream& operator<<(std::ostream& os, const my_struct& x) {
    //         return os << qor_reflection::io_fields(x);  // Equivalent to: os << "{ " << x.i << " ," <<  x.s << " }"
    //     }
    //
    //     std::istream& operator>>(std::istream& is, my_struct& x) {
    //         return is >> qor_reflection::io_fields(x);  // Equivalent to: is >> "{ " >> x.i >> " ," >>  x.s >> " }"
    //     }
    //
    // Input and output streaming operators for `qor_reflection::io_fields` are symmetric, meaning that you get the original value by streaming it and
    // reading back if each fields streaming operator is symmetric.

    template <class T>
    auto io_fields(T&& value) noexcept 
    {
        return detail::io_fields_impl<T>{std::forward<T>(value)};
    }

    qor_pp_refl_end_module_export

}//qor_reflection

#endif//QOR_PP_H_REFLECTION_IOFIELDS
