// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_EXCEPTIONWRAPPER
#define QOR_PP_H_TESTMOCK_EXCEPTIONWRAPPER

namespace qor{ namespace mock{

    template <class T>
    class ExceptionWrapper : public ExceptionHolder
    {
    private:

        T exception;

    public:
        
        ExceptionWrapper(T ex) : exception(ex) {}
        
        void rethrow() 
        { 
            throw exception; 
        }
    };

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_EXCEPTIONWRAPPER
