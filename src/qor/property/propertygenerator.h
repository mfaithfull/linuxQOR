// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROPERTY_GENERATOR
#define QOR_PP_H_PROPERTY_GENERATOR

#include "src/qor/reflection/reflection.h"

namespace qor {

    template <std::size_t I, std::size_t N>
    struct PropertyGenerator
    {
        template< class T, class R>
        static void Generate(PropertyVector& container, const T& value, const R& org_val)
        {
            auto val = qor_reflection::detail::sequence_tuple::get<I>(value);
            auto c = qor_reflection::get_name<I, R>();
            std::string name(c);
            Property prop(PropertyName(name.c_str()));
            prop.SetValue(PropertyValue().Set(val));
            container.emplace_back(prop);
            PropertyGenerator<I + 1, N>::Generate(container, value, org_val);
        }
    };

    template <std::size_t I>
    struct PropertyGenerator<I, I>
    {
        template <class T, class R> static void Generate(PropertyVector& /*container*/, const T& /*value*/, const R& /*org_val*/) noexcept {}
    };
}

#endif//QOR_PP_H_PROPERTY_GENERATOR