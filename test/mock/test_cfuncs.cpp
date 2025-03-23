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

#ifdef qor_pp_mock_cfuncsupport

int a()
{
  return 1;
}

qor_pp_test_case (checkFunctionReplacedAndChecked)
{
	qor_pp_test_equal(a(), 1);
	MockRepository mocks;
	mocks.ExpectCallFunc(a).Return(2);
	qor_pp_test_equal(a(), 2);
}

qor_pp_test_case (checkFunctionReturnedToOriginal)
{
	{
		qor_pp_test_equal(a(), 1);
		MockRepository mocks;
		mocks.ExpectCallFunc(a).Return(2);
		qor_pp_test_equal(a(), 2);
	}
	qor_pp_test_equal(a(), 1);
}

qor_pp_test_case(ExpectCallOverridesNeverCall)
{
    MockRepository mocks;
    mocks.NeverCallFunc(a);
    mocks.ExpectCallFunc(a).Return(42);
    a();
}


#	if(qor_pp_os_target == qor_pp_os_windows)
#		include <windows.h>
qor_pp_test_case (checkCanMockGetSystemTime) 
{
	MockRepository mocks;
	SYSTEMTIME outtime;
	outtime.wDay = 1;
	SYSTEMTIME systime;
	systime.wDay = 0;
	mocks.ExpectCallFunc(GetSystemTime).With(Out(outtime));
	GetSystemTime(&systime);
	qor_pp_test_equal(systime.wDay, 1);
}
#	endif

#endif
