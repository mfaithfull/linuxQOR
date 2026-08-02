// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_CONTRACT_TAG
#define QOR_PP_H_CONTRACT_TAG

// Copyright 2026 Ilya Korolev
// Licensed under the Apache License, Version 2.0
// SPDX-License-Identifier: Apache-2.0

namespace qor { namespace contract {

    template<class T>
    struct tag 
    {
        using type = T;
    };

}}//qor::contract

#endif//QOR_PP_H_CONTRACT_TAG