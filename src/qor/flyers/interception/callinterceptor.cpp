// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "callinterceptor.h"
#include "flystrap.h"

namespace qor {

	CallInterceptor::CallInterceptor() : Flyer< CallInterceptor, FlyStrapBase >()
	{
		Push();
		if( m_Previous )
		{
			FlyStrapBase* flyStrapBase = dynamic_cast< FlyStrapBase* >( m_Previous );
			if( flyStrapBase )
			{
				flyStrapBase->OnDeactivate();
			}
		}
		OnActivate();
	}

	CallInterceptor::~CallInterceptor()
	{
		FlyStrap* next = Next( (FlyStrap*)(nullptr) );

		if( next )
		{
			next->UnStrap();
		}

		if( m_Previous )
		{
			FlyStrapBase* flyStrapBase = dynamic_cast< FlyStrapBase* >( m_Previous );
			if( flyStrapBase )
			{
				flyStrapBase->OnReactivate();
			}
		}
		Pop();
	}

	void CallInterceptor::CallMade( CallContext* call, IFunctionContext* function)
	{
		FlyStrap* next = Next( (FlyStrap*)(nullptr) );
		if( next )
		{
			next->CallPre( call, function );
		}
	}

	void CallInterceptor::OnReturnAssignment( CallContext* call )
	{
		FlyStrap* next = Next( (FlyStrap*)(nullptr) );
		if( next )
		{
			next->CallOnReturnAssignment( call );
		}
	}

	void CallInterceptor::OnReturn(CallContext* call)
	{
		FlyStrap* next = Next((FlyStrap*)(nullptr));
		if(next)
		{
			next->OnReturn(call);
		}
	}

	void CallInterceptor::CallCompleted( CallContext* call )
	{
		FlyStrap* next = Next( (FlyStrap*)(nullptr) );
		if( next )
		{
			next->CallPost( call );
		}
	}

}//qor
//Review 2027/07/07