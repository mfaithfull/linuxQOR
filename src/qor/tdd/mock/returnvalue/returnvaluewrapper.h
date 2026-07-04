// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_RETURNVALUEWRAPPER
#define QOR_PP_H_TESTMOCK_RETURNVALUEWRAPPER

namespace qor{ namespace mock{
    
    template <class T>
    class ReturnValueWrapper
    {
    public:

        virtual ~ReturnValueWrapper() {}
        virtual T value() = 0;
    };

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_RETURNVALUEWRAPPER
