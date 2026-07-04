// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_RTTIINFO
#define QOR_PP_H_TESTMOCK_RTTIINFO

namespace qor{ namespace mock{
    
    struct RttiInfo
    {
        void* baseRttiInfoType;
        const char* typeName;
        const std::type_info* baseClassName;

        RttiInfo(const std::type_info& base, const std::type_info& actualType)
        {
            RttiInfo* baseR = (RttiInfo*)&base;
            baseRttiInfoType = baseR->baseRttiInfoType; // Single-inheritance
            typeName = baseR->typeName;                 // Mock<T> class
            baseClassName = &actualType;                // that now inherits from actualType.
        }
    };

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_RTTIINFO
