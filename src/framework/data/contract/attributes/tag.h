// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_CONTRACT_ATTRIBUTES_TAGS
#define QOR_PP_H_CONTRACT_ATTRIBUTES_TAGS


// Copyright 2026 Ilya Korolev
// Licensed under the Apache License, Version 2.0
// SPDX-License-Identifier: Apache-2.0

#include <type_traits>

namespace qor { namespace contract {

    template<auto... Tags>
    struct tags {};

    template<class Attr>
    struct attr_traits;

    namespace detail {

    template<class Set, auto Tag>
    struct tags_contains : std::false_type {};

    template<auto... Tags, auto Tag>
    struct tags_contains<tags<Tags...>, Tag>
        : std::bool_constant<((Tags == Tag) || ...)> {};

    template<class Attr, class = void>
    struct attribute_tags 
    {
        using type = tags<>;
    };

    template<class Attr>
    struct attribute_tags<
        Attr,
        std::void_t<typename attr_traits<std::remove_cv_t<std::remove_reference_t<Attr>>>::tags>> 
    {
        using type = typename attr_traits<std::remove_cv_t<std::remove_reference_t<Attr>>>::tags;
    };

    }//detail

    template<class Attr, auto Tag>
    inline constexpr bool has_attribute_tag_v = detail::tags_contains<typename detail::attribute_tags<Attr>::type, Tag>::value;

}}//qor::contract

#endif//QOR_PP_H_CONTRACT_ATTRIBUTES_TAGS