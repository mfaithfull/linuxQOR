// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_REGISTRATIONTYPE
#define QOR_PP_H_TESTMOCK_REGISTRATIONTYPE

#include <limits>

namespace qor { namespace mock {

    struct RegistrationType
    {
        RegistrationType(unsigned int min, unsigned int max) : minimum(min), maximum(max) {}

        unsigned int minimum;
        unsigned int maximum;
    };
    
    inline bool operator==(RegistrationType const& rhs, RegistrationType const& lhs)
    {
        return rhs.minimum == lhs.minimum && rhs.maximum == lhs.maximum;
    }

    const RegistrationType Any = RegistrationType(1, (std::numeric_limits<unsigned int>::max)());
    const RegistrationType Never = RegistrationType((std::numeric_limits<unsigned int>::min)(), (std::numeric_limits<unsigned int>::min)());
    const RegistrationType Once = RegistrationType(1, 1);        

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_REGISTRATIONTYPE
