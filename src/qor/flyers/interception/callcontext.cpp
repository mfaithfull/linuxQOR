// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "callcontext.h"
#include "callinterceptor.h"
#include "src/qor/memory/reference/newref.h"

namespace qor{

	void ParameterBase::Clear(void)
	{
		m_p = nullptr;
	}

	void CallContext::CallMade(IFunctionContext* pCalledContext)
	{
		//Execution has reached the context of the callee
		typename ref_of< CallInterceptor >::type ptrInterceptor = new_ref< CallInterceptor >();
		if (!ptrInterceptor.IsNull())
		{
			ptrInterceptor->CallMade(this, pCalledContext);
		}
	}

	void CallContext::OnReturnAssignment()
	{
		typename ref_of< CallInterceptor >::type ptrInterceptor = new_ref< CallInterceptor >();
		if (!ptrInterceptor.IsNull())
		{
			ptrInterceptor->OnReturnAssignment(this);
		}
	}

	void CallContext::OnReturn()
	{
		typename ref_of< CallInterceptor >::type ptrInterceptor = new_ref< CallInterceptor >();
		if (!ptrInterceptor.IsNull())
		{
			ptrInterceptor->OnReturn(this);
		}
	}

	void CallContext::CallCompleted()
	{
		//Execution of the callee is winding up in preparation for returning
		typename ref_of< CallInterceptor >::type ptrInterceptor = new_ref< CallInterceptor >();
		if (!ptrInterceptor.IsNull())
		{
			ptrInterceptor->CallCompleted(this);
		}
	}

	ParameterBase* CallContext::Parameters(void)
	{
		return m_aParameters;	//Access to the list of registered parameters
	}

	ParameterBase* CallContext::ReturnValue(void)
	{
		return &m_ReturnValue;
	}


	void CallContext::OutOfLineRegistration(ParameterBase& Param)
	{
		if (m_paramCount < 9)
		{
			m_aParameters[m_paramCount++] = Param;
		}
	}

}//qor
