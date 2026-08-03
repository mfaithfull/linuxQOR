// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_CONTRACT_ATTRIBUTES_SECURITY_CHECK
#define QOR_PP_H_CONTRACT_ATTRIBUTES_SECURITY_CHECK

// Copyright 2026 Ilya Korolev
// Licensed under the Apache License, Version 2.0
// SPDX-License-Identifier: Apache-2.0

#include "../adapter_traits.h"

namespace qor { namespace contract::security {

    struct vocabulary {};

    struct sensitive {};
    struct secret {};
    struct no_log {};
    struct encrypt {};

}}//qor::contract::security

namespace qor { namespace contract {

    template<>
    struct attr_traits<security::sensitive> 
    {
        using vocabulary = contract::security::vocabulary;
        static constexpr attr_targets targets{false, true, false, false, false};
        static constexpr bool repeatable = false;
    };

    template<>
    struct attr_traits<security::secret> 
    {
        using vocabulary = contract::security::vocabulary;
        static constexpr attr_targets targets{false, true, false, false, false};
        static constexpr bool repeatable = false;
    };

    template<>
    struct attr_traits<security::no_log> 
    {
        using vocabulary = contract::security::vocabulary;
        static constexpr attr_targets targets{false, true, false, false, false};
        static constexpr bool repeatable = false;
    };

    template<>
    struct attr_traits<security::encrypt> 
    {
        using vocabulary = contract::security::vocabulary;
        static constexpr attr_targets targets{false, true, false, false, false};
        static constexpr bool repeatable = false;
    };

}}//qor::contract

#endif//QOR_PP_H_CONTRACT_ATTRIBUTES_SECURITY_CHECK
