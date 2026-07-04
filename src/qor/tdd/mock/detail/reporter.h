// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCKREPORTER
#define QOR_PP_H_TESTMOCKREPORTER

#include <string>

namespace qor{ namespace mock{
    
    class Call;

    class Reporter 
    {
    public:
        virtual void CallMissing(Call& call, MockRepository& repo) = 0;
        virtual void ExpectationExceeded(Call& call, MockRepository& repo, const std::string& args, const char* funcName) = 0;
        virtual void FunctionCallToZombie(MockRepository& repo, const std::string& args) = 0;
        virtual void InvalidBaseOffset(size_t baseOffset, MockRepository& repo) = 0;
        virtual void InvalidFuncIndex(size_t funcIndex, MockRepository& repo) = 0;
        virtual void NoExpectationMatches(MockRepository& repo, const std::string& args, const char* funcName) = 0;
        virtual void NoResultSetUp(Call& call, MockRepository& repo, const std::string& args, const char* funcName) = 0;
        virtual void UnknownFunction(MockRepository& repo) = 0;
        virtual void TestStarted() = 0;
        virtual void TestFinished() = 0;
    };

    namespace detail {
	    Reporter* GetDefaultReporter();
    }
}}//qor::mock

#endif//QOR_PP_H_TESTMOCKREPORTER
