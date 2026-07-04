// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_EXPECTATIONEXCEPTION
#define QOR_PP_H_TESTMOCK_EXPECTATIONEXCEPTION

namespace qor{ namespace mock{

    class ExpectationException : public BaseException 
    {
    public:
        ExpectationException(MockRepository& repo, const std::string& args, const char* funcName)
        {
            std::stringstream text;
            text << "Function " << funcName << args << " called with mismatching expectation!" << std::endl;
            text << repo;
            txt = text.str();
        }
    };

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_EXPECTATIONEXCEPTION
