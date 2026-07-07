// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/newref.h"
#include "flystrap.h"

namespace qor {

    FlyStrap::FlyStrap() : FlyStrapBase() { }
	FlyStrap::FlyStrap(const FlyStrap& src) : FlyStrapBase() { }

    FlyStrap::FlyStrap( FlyStrapBase* joinPoint ) : FlyStrapBase()
    {
        if( joinPoint )
        {
            joinPoint->StrapOn(this);
        }
    }

    bool FlyStrap::CallPre( CallContext* call, IFunctionContext* function )
    {
        bool result = ( PreCondition( call, function ) && Pre( call, function) );
        FlyStrap* next = Next((FlyStrap*)(nullptr));
        if( next != nullptr)
        {
            result = next->CallPre( call, function );
        }
        return result;
    }

    bool FlyStrap::CallOnReturnAssignment( CallContext* call )
	{
		FlyStrap* next = Next( (FlyStrap*)(nullptr) );
		if( next != nullptr )
		{
			next->CallOnReturnAssignment( call );
		}
		return ( OnAssignmentCondition( call ) && OnReturnAssignment( call ) );
	}

	bool FlyStrap::CallPost( CallContext* call )
	{
		FlyStrap* next = Next( (FlyStrap*)(nullptr) );
		if( next != nullptr )
		{
			next->CallPost( call );
		}
		return ( PostCondition( call ) && Post( call ) );
	}

	bool FlyStrap::Pre( CallContext* /*call*/, IFunctionContext* /*function*/ )
	{
		return true;
	}

	bool FlyStrap::Post( CallContext* /*call*/ )
	{
		return true;
	}

	bool FlyStrap::OnReturnAssignment( CallContext* /*call*/ )
	{
		return true;
	}

	bool FlyStrap::OnReturn(CallContext* /*call*/)
	{
		return true;
	}

	bool FlyStrap::PreCondition( CallContext* /*call*/, IFunctionContext* /*function*/ )
	{
		return false;
	}

	bool FlyStrap::OnAssignmentCondition( CallContext* /*call*/ )
	{
		return false;
	}

	bool FlyStrap::PostCondition( CallContext* /*call*/ )
	{
		return false;
	}

}//qor
//Review 2027/07/07