// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TESTREPORTING
#define QOR_PP_H_TESTREPORTING

#include <string>
#include "testcase.h"

namespace qor { namespace test 
{
    struct Reporting
    {
        virtual int FailCount() = 0;
        virtual void TestRunStarting() = 0;
        virtual void TestCaseStarting(const TestCase& info) = 0;
        virtual void TestCaseEnded(const TestCase& info) = 0;
        virtual void TestRunEnded() = 0;
        virtual void LogFailure(const std::string s) = 0;
    };
}}//qor::test

#endif//QOR_PP_H_TESTREPORTING