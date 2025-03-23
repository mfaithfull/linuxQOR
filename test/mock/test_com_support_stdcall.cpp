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
#include "../../src/qor/mock/comsupport.h"

using namespace qor;
using namespace qor::test;

#ifdef _MSC_VER

#include <atlbase.h>
#include <atlcom.h>

using namespace qor;
using namespace qor::test;
using namespace qor::mock;

//------------------------------------------------------------------------------
namespace ComSupportTests
{

	//------------------------------------------------------------------------------
class ICom 
{
public:
	virtual ~ICom() {}
	virtual long __stdcall A(void) = 0;
	virtual long __stdcall B(int) = 0;
	virtual long __stdcall C(int, int) = 0;
	virtual long __stdcall D(int, int, int) = 0;
	virtual long __stdcall E(int, int, int, int) = 0;
	virtual long __stdcall F(int, int, int, int, int) = 0;
	virtual long __stdcall G(int, int, int, int, int, int) = 0;
	virtual long __stdcall H(int, int, int, int, int, int, int) = 0;
	virtual long __stdcall I(int, int, int, int, int, int, int, int) = 0;
	virtual long __stdcall J(int, int, int, int, int, int, int, int, int ) = 0;
	virtual long __stdcall K(int, int, int, int, int, int, int, int, int, int) = 0;
	virtual long __stdcall L(int, int, int, int, int, int, int, int, int, int, int) = 0;
	virtual long __stdcall M(int, int, int, int, int, int, int, int, int, int, int, int) = 0;
	virtual long __stdcall N(int, int, int, int, int, int, int, int, int, int, int, int, int) = 0;
	virtual long __stdcall O(int, int, int, int, int, int, int, int, int, int, int, int, int, int) = 0;
	virtual long __stdcall P(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) = 0;
};


qor_pp_test_case(checkStdCallBase)
{
	MockRepository mocks;

	ICom* ic = mocks.Mock<ICom>();
	mocks.ExpectCall(ic, ICom::A)
		.Return(1);

	long actual = ic->A();
	qor_pp_test_equal(1, actual);
}

qor_pp_test_case(checkStdCallTemplateWithArgumentsImplementations)
{
	MockRepository mocks;

	ICom* ic = mocks.Mock<ICom>();

	mocks.ExpectCall(ic, ICom::B)
		.Return(2);
	mocks.ExpectCall(ic, ICom::C)
		.Return(3);
	mocks.ExpectCall(ic, ICom::D)
		.Return(4);
	mocks.ExpectCall(ic, ICom::E)
		.Return(5);
	mocks.ExpectCall(ic, ICom::F)
		.Return(6);
	mocks.ExpectCall(ic, ICom::G)
		.Return(7);
	mocks.ExpectCall(ic, ICom::H)
		.Return(8);
	mocks.ExpectCall(ic, ICom::I)
		.Return(9);
	mocks.ExpectCall(ic, ICom::J)
		.Return(10);
	mocks.ExpectCall(ic, ICom::K)
		.Return(11);
	mocks.ExpectCall(ic, ICom::L)
		.Return(12);
	mocks.ExpectCall(ic, ICom::M)
		.Return(13);
	mocks.ExpectCall(ic, ICom::N)
		.Return(14);
	mocks.ExpectCall(ic, ICom::O)
		.Return(15);
	mocks.ExpectCall(ic, ICom::P)
		.Return(16);

	ic->B(1);
	ic->C(1,2);
	ic->D(1,2,3);
	ic->E(1,2,3,4);
	ic->F(1,2,3,4,5);
	ic->G(1,2,3,4,5,6);
	ic->H(1,2,3,4,5,6,7);
	ic->I(1,2,3,4,5,6,7,8);
	ic->J(1,2,3,4,5,6,7,8,9);
	ic->K(1,2,3,4,5,6,7,8,9,10);
	ic->L(1,2,3,4,5,6,7,8,9,10,11);
	ic->M(1,2,3,4,5,6,7,8,9,10,11,12);
	ic->N(1,2,3,4,5,6,7,8,9,10,11,12,13);
	ic->O(1,2,3,4,5,6,7,8,9,10,11,12,13,14);
	ic->P(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
}

qor_pp_test_case(checkUnexpectedMethodCallThrowsException)
{
	//MockRepository mocks;

	//ICom* ic = mocks.Mock<ICom>();

	//ic->A();
}


template <int s>
int virtual_function_index(unsigned char* pb)
{
	if (*pb == 0xE9)
	{
		return virtual_function_index<0>(pb + 5 + *(unsigned int *)(pb+1));
	}
	if (!pb) return -1;
	int pboff = -1;
	if (pb[0] == 0x8b && pb[1] == 0x01) 
	{	//mov eax, [ecx]
		pboff = 2;
	}
	else if (pb[0] == 0x8b && pb[1] == 0x44 && pb[2] == 0x24 && pb[3] == 0x04 &&		//mov eax, [esp+arg0]
		pb[4] == 0x8b && pb[5] == 0x00) //mov eax, [eax]
	{										
		pboff = 6;
	}

	if (pboff > 0) 
	{
		if (pb[pboff] == 0xff) 
		{
			switch (pb[pboff + 1]) 
			{
			case 0x20:	//jmp dword ptr [eax]
				return 0;
			case 0x60:	//jmp dword ptr [eax+0xNN]
				return (((int)pb[pboff + 2]) & 0xff) / 4;
			case 0xa0:	//jmp dword ptr [eax+0xNNN]
				return (*(unsigned int*)(pb + (pboff + 2))) / 4;
			default:
				break;
			}
		}
	}
	return -1;
}

template <class T>
int VirtualFIndex(T func)
{
	union {
		T pfn;
		unsigned char* pb;
	};
	pfn = func;
	return virtual_function_index<0>(pb);
}

struct AA
{
	virtual int Get1() = 0;
	virtual int Get2() {}
};

//class with only 1 virtual method
class IAA {
public:
	virtual int f(int a);
};


MIDL_INTERFACE("4745C05E-23E6-4c6d-B9F2-E483359A8B89")
COMInterface1 : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE getTObjectCount( 
        /* [out] */ unsigned long *pCount) = 0;
};
 
typedef GUID ESTypeID;

MIDL_INTERFACE("356D44D9-980A-4149-A586-C5CB8B191437")
COMInterface2 : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE getMappablePackages( 
        /* [out] */ long *pSize,
        /* [size_is][size_is][out] */ ESTypeID **pIdList) = 0;
};
#if 0
qor_pp_test_case(EnsureThat_VirtualFunctionIndexGenericWorksforAllCases)
{
	qor_pp_test_equal(1, VirtualFIndex(&AA::Get2));
	qor_pp_test_equal(0, VirtualFIndex(&AA::Get1));
	qor_pp_test_equal(0, VirtualFIndex(&IAA::f));
		
	qor_pp_test_equal(0, VirtualFIndex(&IUnknown::QueryInterface));
	qor_pp_test_equal(1, VirtualFIndex(&IUnknown::AddRef));
	qor_pp_test_equal(2, VirtualFIndex(&IUnknown::Release));
		
	qor_pp_test_equal(0, VirtualFIndex(&COMInterface1::QueryInterface));
	qor_pp_test_equal(1, VirtualFIndex(&COMInterface1::AddRef));
	qor_pp_test_equal(2, VirtualFIndex(&COMInterface1::Release));
}
#endif

qor_pp_test_case(CheckThat_AddCommExpectations_Stubs_QueryInterface_AddRef_Release)
{
	MockRepository mocks; 
	COMInterface1* deviceMock = mocks.Mock<COMInterface1>();

	AddComExpectations(mocks, deviceMock);
			
	{
		CComPtr<IUnknown> pUnk = deviceMock; 
		CComQIPtr<COMInterface1> pDevice = (IUnknown*)pUnk;

		qor_pp_test_check(pDevice == pUnk);

		IUnknown* p = NULL;
		pDevice->QueryInterface(__uuidof(IUnknown), (void**)&p);

		qor_pp_test_check(p == deviceMock);

	}
}

qor_pp_test_case(CheckThat_ConnectComInterfaces_Stubs_QueryInterface_ToEachOther)
{
	MockRepository mocks; 
	COMInterface1* deviceMock = mocks.Mock<COMInterface1>();
	COMInterface2* devMappingMock = mocks.Mock<COMInterface2>();

	ConnectComInterfaces(mocks, deviceMock, devMappingMock);

	{
		//Com objects can reach each other
		CComQIPtr<COMInterface2> pDevMapping = deviceMock;
			
		qor_pp_test_check(pDevMapping != NULL);
		qor_pp_test_check(pDevMapping == devMappingMock);

		CComQIPtr<COMInterface1> pDevNavigate = devMappingMock;

		qor_pp_test_check(pDevNavigate != NULL);
		qor_pp_test_check(pDevNavigate == deviceMock);
	}

}
} // end of namespace

#endif
