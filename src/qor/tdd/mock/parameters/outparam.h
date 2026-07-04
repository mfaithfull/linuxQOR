// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_OUTPARAM
#define QOR_PP_H_TESTMOCK_OUTPARAM

namespace qor { namespace mock {

    template <typename T>
    struct OutParam : public DontCare
    {
        T value;

        explicit OutParam(T val) : value(val){}
    };

    template <typename T>
    OutParam<T> Out(T t) 
    { 
        return OutParam<T>(t); 
    }

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_OUTPARAM
