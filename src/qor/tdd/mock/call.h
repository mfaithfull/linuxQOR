// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_CALL
#define QOR_PP_H_TESTMOCK_CALL

#include <vector>
#include <iostream>

namespace qor{ namespace mock{
    
    class Call
    {
    public:

        base_mock* mock;
        std::pair<int, int> funcIndex;
        std::vector<Call*> previousCalls;
        unsigned int called;
        RegistrationType expectation;
        int lineno;
        const char* funcName;
        const char* fileName;
                
        virtual ~Call(){}

        // This function checks if the call we've now received applies to this mock and function. If so we can use the type info.
        bool applies(base_mock* rhsMock, std::pair<int, int> rhsIndex)
        {
            return mock == rhsMock && funcIndex == rhsIndex && previousCallsSatisfied();
        }

        bool isSatisfied() const
        {
            return called >= expectation.minimum && previousCallsSatisfied();
        }

        virtual void printArgs(std::ostream& os) const = 0;

    private:

        inline bool previousCallsSatisfied() const
        {
            for (auto& c : previousCalls)
            {
                if (!c->isSatisfied()) return false;
            }
            return true;
        }

    protected:

        Call(RegistrationType expect, base_mock* baseMock, const std::pair<int, int>& index, int X, const char* func, const char* file)
            : mock(baseMock)
            , funcIndex(index)
            , called(0)
            , expectation(expect)
            , lineno(X)
            , funcName(func)
            , fileName(file){ }
    };

    inline std::ostream& operator<<(std::ostream& os, const Call& call)
    {
        os << call.fileName << "(" << call.lineno << ") ";
        if (call.expectation == Once)
            os << "Expectation for ";
        else
            os << "Result set for ";

        os << call.funcName;

        call.printArgs(os);

        os << " on the mock at 0x" << call.mock << " was ";

        if (!call.isSatisfied())
            os << "not ";

        if (call.expectation == Once)
            os << "satisfied." << std::endl;
        else
            os << "used." << std::endl;

        return os;
    }

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_CALL
