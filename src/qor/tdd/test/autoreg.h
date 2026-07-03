// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TESTAUTOREG
#define QOR_PP_H_TESTAUTOREG

#include "factory.h"

namespace qor {	namespace test {

    struct AutoReg
    {
        AutoReg(std::string name, const char* at, TestFunc f)
        {
            Factory::Instance().Register(TestCase(name, std::string(at), f));
        }
    };

}}//qor::test

#endif//QOR_PP_H_TESTAUTOREG
