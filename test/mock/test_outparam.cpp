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

class IOutParam 
{ 
public:

	virtual ~IOutParam() {}

	virtual void a(std::string& out) = 0;
	virtual void b(std::string** out) = 0;
	virtual void c(char** out) = 0;
};

qor_pp_test_case (checkOutParamsAreFilledIn_ConstChar)
{
	MockRepository mocks;
	IOutParam *iamock = mocks.Mock<IOutParam>();
	mocks.ExpectCall(iamock, IOutParam::a).With(Out("Hello World"));
	
	std::string out;
	iamock->a(out);

	qor_pp_test_check(out == "Hello World");
}

qor_pp_test_case (checkOutParamsAreFilledIn_String)
{
	MockRepository mocks;
	IOutParam *iamock = mocks.Mock<IOutParam>();
	std::string teststring("Hello World");
	mocks.ExpectCall(iamock, IOutParam::a).With(Out(teststring));
	
	std::string out;
	iamock->a(out);

	qor_pp_test_check(out == teststring);
}

qor_pp_test_case (checkOutParamsAreFilledIn_PointerToString)
{
	MockRepository mocks;
	IOutParam *iamock = mocks.Mock<IOutParam>();
	std::string teststring("Hello World");
  std::string *outString = new std::string(teststring);
	mocks.ExpectCall(iamock, IOutParam::b).With(Out(outString));
	
	std::string* out = 0;
	iamock->b(&out);

	qor_pp_test_check(*out == teststring);

  delete outString;
}

qor_pp_test_case (checkOutParamsAreFilledIn_Char)
{
	MockRepository mocks;
	IOutParam *iamock = mocks.Mock<IOutParam>();
	const char* teststring = "Hello World";
	mocks.ExpectCall(iamock, IOutParam::c).With(Out((char*)teststring));
	
	char* out = 0;
	iamock->c(&out);

	qor_pp_test_check(strcmp(out, teststring) == 0);
}
