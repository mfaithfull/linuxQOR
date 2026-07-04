// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_CALLMISSINGEXCEPTION
#define QOR_PP_H_TESTMOCK_CALLMISSINGEXCEPTION

#include <sstream>

namespace qor{ namespace mock{

    class CallMissingException : public BaseException 
    {
    public:
        CallMissingException(MockRepository& repo)
        {
            std::stringstream text;
            text << "Function with expectation not called!" << std::endl;
            text << repo;
            txt = text.str();
        }
    };

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_CALLMISSINGEXCEPTION