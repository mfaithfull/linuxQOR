// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_EXCEPTIONFUNCTOR
#define QOR_PP_H_TESTMOCK_EXCEPTIONFUNCTOR

#include <functional>

namespace qor{ namespace mock{

    class ExceptionFunctor : public ExceptionHolder
    {
    public:

        std::function<void()> func;

        ExceptionFunctor(std::function<void()> func) : func(func) {}

        virtual void rethrow() 
        { 
            func(); 
        }
    };

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_EXCEPTIONFUNCTOR
