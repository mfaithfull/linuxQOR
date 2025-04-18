// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

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
