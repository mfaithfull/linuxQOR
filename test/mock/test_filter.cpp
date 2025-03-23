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

#include "../../src/configuration/configuration.h"
#include "../../src/qor/test/test.h"
#include "../../src/qor/mock/mocks.h"

using namespace qor;
using namespace qor::test;

class IAA 
{ 
public:

    virtual int f(int a, int b, int c) 
	{ 
		(void)a; (void)b; (void)c; return 0; 
	}
};

bool allEven(int a, int b, int c) 
{ 
	return (a % 2 == 0) && (b % 2 == 0) && (c % 2 == 0); 
}

bool oeo(int a, int b, int c) 
{ 
	return (a % 2 == 1) && (b % 2 == 0) && (c % 2 == 1); 
}

bool eod(int a, int b, int) 
{ 
	return (a % 2 == 0) && (b % 2 == 1); 
}

bool never(int, int, int) 
{ 
	return false; 
}

qor_pp_test_case (checkFilterIsApplied)
{
	MockRepository mocks;
	IAA *iamock = mocks.Mock<IAA>();
	mocks.OnCall(iamock, IAA::f).Return(4);
	mocks.OnCall(iamock, IAA::f).Match(never).Return(3);
	mocks.OnCall(iamock, IAA::f).Match(eod).Return(2);
	mocks.OnCall(iamock, IAA::f).Match(oeo).Return(1);
	mocks.OnCall(iamock, IAA::f).Match(allEven).Return(5);
	qor_pp_test_equal(5, iamock->f(0, 0, 0));
	qor_pp_test_equal(4, iamock->f(0, 0, 1));
	qor_pp_test_equal(2, iamock->f(0, 1, 0));
	qor_pp_test_equal(2, iamock->f(0, 1, 1));
	qor_pp_test_equal(4, iamock->f(1, 0, 0));
	qor_pp_test_equal(1, iamock->f(1, 0, 1));
	qor_pp_test_equal(4, iamock->f(1, 1, 0));
	qor_pp_test_equal(4, iamock->f(1, 1, 1));
}

