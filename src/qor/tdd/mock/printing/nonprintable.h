// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_NOTPRINTABLE
#define QOR_PP_H_TESTMOCK_NOTPRINTABLE

#include <iostream>

namespace qor{ namespace mock {

    struct NotPrintable
    {
        template <typename T> NotPrintable(T const&){}
    };

    inline std::ostream& operator << (std::ostream& os, NotPrintable const&)
    {
        os << "???";
        return os;
    }

    inline std::wostream& operator << (std::wostream& os, NotPrintable const&)
    {
        os << L"???";
        return os;
    }
    
}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_NOTPRINTABLE
