// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_MIXINS_FOREACH
#define QOR_PP_H_FRAMEWORK_MIXINS_FOREACH

#include "access.h"

namespace qor{ namespace mix{

    template< typename T, typename A, typename L >
    void for_each( access< T >* p, A, L l)
    {
        auto& data = p->get_data();
        //magic_here
    }

}}//qor::mix


#endif//QOR_PP_H_FRAMEWORK_MIXINS_FOREACH