// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_DETAIL_DEFAULTREPORTER
#define QOR_PP_H_TESTMOCK_DETAIL_DEFAULTREPORTER

#include <functional>

namespace qor{ namespace mock{ namespace detail {

    inline Reporter* GetDefaultReporter() 
    {
        
        static struct DefaultReporter : Reporter 
        {
            DefaultReporter() : latentException([] {}) 
            {
            }

            std::function<void()> latentException;

            void CallMissing(Call& call, MockRepository& repo) override 
            {
                (void)call;
                qor_pp_mock_raiseexception(CallMissingException(repo));
            }

            void ExpectationExceeded(Call& call, MockRepository& repo, const std::string& args, const char* funcName) override 
            {
                (void)call;
                qor_pp_mock_raiseexception(ExpectationException(repo, args, funcName));
            }

            void FunctionCallToZombie(MockRepository& repo, const std::string& args) override 
            {
                (void)args;
                qor_pp_mock_raiseexception(ZombieMockException(repo));
            }

            void InvalidBaseOffset(size_t baseOffset, MockRepository& repo) override 
            {
                (void)baseOffset;
                (void)repo;
                std::terminate();
            }

            void InvalidFuncIndex(size_t funcIndex, MockRepository& repo) override 
            {
                (void)funcIndex;
                (void)repo;
                std::terminate();
            }

            void NoExpectationMatches(MockRepository& repo, const std::string& args, const char* funcName) override 
            {
                qor_pp_mock_raiseexception(ExpectationException(repo, args, funcName));
            }

            void NoResultSetUp(Call& call, MockRepository& repo, const std::string& args, const char* funcName) override 
            {
                (void)call;
                qor_pp_mock_raiseexception(NoResultSetUpException(repo, args, funcName));
            }

            void UnknownFunction(MockRepository& repo) override 
            {
                qor_pp_mock_raiseexception(NotImplementedException(repo));
            }

            void TestStarted() override 
            {
                latentException = [] {};
            }

            void TestFinished() override 
            {
                if (!std::uncaught_exceptions() && latentException) 
                {
                    latentException();
                }
            }
        } defaultReporter;
        return &defaultReporter;
    }

}}}//qor::mock::detail

#endif//QOR_PP_H_TESTMOCK_DETAIL_DEFAULTREPORTER
