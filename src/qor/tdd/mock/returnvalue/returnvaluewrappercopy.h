// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_RETURNVALUEWRAPPERCOPY
#define QOR_PP_H_TESTMOCK_RETURNVALUEWRAPPERCOPY

#include <functional>

namespace qor{ namespace mock{

    template <typename X> struct no_cref 
    { 
        typedef X type; 
    };

    template <typename X> 
    struct no_cref<const X&> 
    { 
        typedef X type; 
    };

    template <class Y, class RY>
    class ReturnValueWrapperCopy : public ReturnValueWrapper<Y>
    {
    public:
        typename no_cref<Y>::type rv;

        ReturnValueWrapperCopy(RY retValue) : rv(retValue) {}

        virtual Y value() 
        { 
            return rv; 
        };
    };

    template <class Y, class RY>
    class ReturnValueWrapperCopy<Y, std::reference_wrapper<RY>> : public ReturnValueWrapper<Y>
    {
    public:
        typename std::reference_wrapper<RY> rv;
        
        ReturnValueWrapperCopy(std::reference_wrapper<RY> retValue) : rv(retValue) {}

        virtual Y value() 
        { 
            return rv; 
        };
    };

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_RETURNVALUEWRAPPERCOPY
