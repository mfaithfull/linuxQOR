// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_EXCEPTIONHOLDERIMPL
#define QOR_PP_H_TESTMOCK_EXCEPTIONHOLDERIMPL

namespace qor{ namespace mock{

    template <typename T>
    ExceptionHolder* ExceptionHolder::Create(T ex)
    {
        return new ExceptionWrapper<T>(ex);
    }

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_EXCEPTIONHOLDERIMPL
