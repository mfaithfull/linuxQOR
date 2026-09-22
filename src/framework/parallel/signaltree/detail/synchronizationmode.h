// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PARALLEL_SIGNALTREE_DETAIL_SYNCMODE
#define QOR_PP_H_PARALLEL_SIGNALTREE_DETAIL_SYNCMODE

#include <cstdint>

namespace qor{ namespace par{

    enum class synchronization_mode : uint32_t
    {
        synchronous     = 0,
        blocking        = synchronous,
        sync            = synchronous,
        asynchronous    = 1,
        non_blocking    = asynchronous,
        async           = asynchronous
    };    

}}

#endif//QOR_PP_H_PARALLEL_SIGNALTREE_DETAIL_SYNCMODE