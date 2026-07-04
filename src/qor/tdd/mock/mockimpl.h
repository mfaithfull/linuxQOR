// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_MOCKIMPL
#define QOR_PP_H_TESTMOCK_MOCKIMPL

#include <tuple>

namespace qor{ namespace mock{

    template <typename T>
    template <int X>
    void mock<T>::mockedDestructor(int)
    {
        std::tuple<> argT;
        repo->DoVoidExpectation(this, translateX(X), argT);
        repo->VerifyPartial(this);
        isZombie = true;
    }

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_MOCKIMPL
