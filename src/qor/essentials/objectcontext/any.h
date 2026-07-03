// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ANY
#define QOR_PP_H_ANY

//This is used in the task library. It's here because it's an Any. Don't use it

namespace qor{	namespace detail{

    // Helper type that can be cast-to from any type.
    struct any
    {
        template<typename T>
        any(T&&) noexcept {}
    };

}}//qor::detail

#endif//QOR_PP_H_ANY
