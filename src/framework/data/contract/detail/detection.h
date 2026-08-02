// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_CONTRACT_DETAIL_DETECTION
#define QOR_PP_H_CONTRACT_DETAIL_DETECTION

// Copyright 2026 Ilya Korolev
// Licensed under the Apache License, Version 2.0
// SPDX-License-Identifier: Apache-2.0

#include "../tag.h"

#include <type_traits>
#include <utility>

namespace qor{ namespace contract::detail {

    template<class Object, auto MemberPtr>
    concept can_member_ref = requires(Object&& object) 
    {
        object.*MemberPtr;
    };

    template<class Field, class Object>
    concept has_free_contract_get = requires(const Field& field, Object&& object) 
    {
        contract_get(field, object);
    };

    template<class Field, class Object, class Value>
    concept has_free_contract_set = requires(const Field& field, Object&& object, Value&& value) 
    {
        contract_set(field, object, value);
    };

    template<class Object, class Field>
    concept has_member_contract_get = requires(Object&& object, const Field& field) 
    {
        object.contract_get(field);
    };

    template<class Object, class Field, class Value>
    concept has_member_contract_set = requires(Object&& object, const Field& field, Value&& value) 
    {
        object.contract_set(field, value);
    };

}}//qor::contract::detail

#endif//QOR_PP_H_CONTRACT_DETAIL_DETECTION
