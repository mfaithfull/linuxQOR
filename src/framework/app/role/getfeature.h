// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0
#ifndef QOR_PP_H_ROLE_GETFEATURE
#define QOR_PP_H_ROLE_GETFEATURE

//A convenience header. The QOR itself can never include this header as it requires the defintion of any derived application class
//in use to already be present before it's included. QOR derived code however can safely include this header after any derived
//Application class has been defined.

#include "src/qor/memory/reference/reference.h"
#include "src/framework/app/application/builder.h"

namespace qor{
    template< class TFeature >
    typename ref_of<TFeature>::type GetFeature()
    {
        return AppBuilder().TheApplication(qor_shared)->GetRole(qor_shared)->template GetFeature<TFeature>();
    }

    template< class TApplication >
    typename ref_of<TApplication>::type GetApplication()
    {
        return AppBuilder().TheApplication().AsRef<TApplication>();
    }
}

#endif//QOR_PP_H_ROLE_GETFEATURE