// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_EXCEPTIONHOLDER
#define QOR_PP_H_TESTMOCK_EXCEPTIONHOLDER

namespace qor{ namespace mock{
    
    class ExceptionHolder
    {
    public:

        virtual ~ExceptionHolder() {}
        virtual void rethrow() = 0;
        template <typename T>
        static ExceptionHolder* Create(T ex);
    };

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_EXCEPTIONHOLDER
