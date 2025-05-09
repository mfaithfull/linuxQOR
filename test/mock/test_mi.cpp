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

class BaseA 
{
public:
	virtual void a() = 0;
};

class BaseB 
{ 
public:
	virtual void b() = 0;
};

class ClassC : public BaseA, public BaseB 
{ 
public:
	virtual void c() = 0;
};

qor_pp_test_case (checkMICallsWork)
{
	MockRepository mocks;
	ClassC *iamock = mocks.Mock<ClassC>();
	mocks.ExpectCall(iamock, ClassC::a);
	mocks.ExpectCall(iamock, ClassC::c);
	mocks.ExpectCall(iamock, ClassC::b);
	iamock->a();
	iamock->c();
	iamock->b();
}
