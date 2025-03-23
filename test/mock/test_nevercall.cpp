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

class IR 
{
public:
	
	virtual ~IR() {}

	virtual void f() {}

	virtual void g() {}

	virtual int h() 
	{ 
		return 0; 
	}
};

qor_pp_test_case (checkNeverCallWorks)
{
	bool exceptionCaught = false;
	MockRepository mocks;
	IR *iamock = mocks.Mock<IR>();
	Call &callF = mocks.ExpectCall(iamock, IR::f);
	mocks.OnCall(iamock, IR::g);
	mocks.NeverCall(iamock, IR::g).After(callF);
	iamock->g();
	iamock->g();
	iamock->f();
	try 
	{
		iamock->g();
	}
	catch (qor::mock::ExpectationException &)
	{
		exceptionCaught = true;
	}
	qor_pp_test_check(exceptionCaught);
}

qor_pp_test_case (checkNeverCallExceptionDetail)
{
	bool exceptionCaught = false;
	MockRepository mocks;
	IR *iamock = mocks.Mock<IR>();
	mocks.NeverCall(iamock, IR::g);
	try
	{
		iamock->g();
	}
	catch (qor::mock::ExpectationException &ex)
	{
		exceptionCaught = true;
		qor_pp_test_check(strstr(ex.what(), "IR::g") != NULL);
	}
	qor_pp_test_check(exceptionCaught);
}

qor_pp_test_case (checkInteractionBetweenCallTypesWorks)
{
	bool exceptionCaught = false;
	MockRepository mocks;
	mocks.autoExpect = false;
	IR *iamock = mocks.Mock<IR>();
	Call &callF = mocks.ExpectCall(iamock, IR::f);
	Call &onCallG = mocks.OnCall(iamock, IR::g);
	mocks.OnCall(iamock, IR::h).Return(2);
	Call &returnThree = mocks.ExpectCall(iamock, IR::h).After(onCallG).Return(3);
	Call &returnFour = mocks.ExpectCall(iamock, IR::h).After(callF).Return(4);
	mocks.NeverCall(iamock, IR::h).After(returnThree).After(returnFour);
	qor_pp_test_check(iamock->h() == 2);
	qor_pp_test_check(iamock->h() == 2);
	iamock->f();
	qor_pp_test_check(iamock->h() == 4);
	qor_pp_test_check(iamock->h() == 2);
	iamock->g();
	qor_pp_test_check(iamock->h() == 3);
	try 
	{
		iamock->h();
	}
	catch (qor::mock::ExpectationException &)
	{
		exceptionCaught = true;
	}
	qor_pp_test_check(exceptionCaught);
}
