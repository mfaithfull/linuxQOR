// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TESTCASE
#define QOR_PP_H_TESTCASE

#include <string>
#include "testfunc.h"

namespace qor { namespace test {

    struct TestCase
    {
        TestCase(std::string name, std::string location, TestFunc f) : 
        m_Name(name), m_At(location), m_Func(f), m_Index(-1), m_Passed(false){}

        std::string m_Name;
        std::string m_At;
        TestFunc m_Func;
        int m_Index;
        bool m_Passed;
    };
}}
#endif//QOR_PP_H_TESTCASE