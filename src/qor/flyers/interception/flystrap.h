// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FLYSTRAP
#define QOR_PP_H_FLYSTRAP

#include "flystrapbase.h"
#include "callcontext.h"

namespace qor {

    class qor_pp_module_interface(QOR_INTERCEPTION) FlyStrap : public FlyStrapBase
	{
	public:

        FlyStrap();
        FlyStrap( FlyStrapBase* );
		virtual ~FlyStrap() = default;

		virtual bool PreCondition( CallContext* call, IFunctionContext* function );		
		virtual bool OnAssignmentCondition( CallContext* call );
		virtual bool PostCondition( CallContext* call );
		virtual bool Pre( CallContext* call, IFunctionContext* function );
		virtual bool OnReturnAssignment( CallContext* call );
		virtual bool OnReturn( CallContext* call );
		virtual bool Post( CallContext* call );

		template< class T > void Strap()
		{
			FlyStrapBase* joinPoint = nullptr;
            auto lookup = CurrentThread::GetCurrent().Context().GetFlyerMap().Lookup( guid_of<T>::guid() );
						
			if (!lookup.IsNull())
			{
				typename ref_of< T >::type instance(lookup);
				if(!instance.IsNull())
				{
					joinPoint = const_cast< FlyStrapBase* >( instance.template As<const FlyStrapBase>() );
				}
			}

			if( joinPoint )
			{
				joinPoint->StrapOn(this);
			}
		}

		bool CallPre( CallContext* call, IFunctionContext* function );
		bool CallOnReturnAssignment( CallContext* call );
		bool CallPost( CallContext* call );

	};

}//qor

#endif//QOR_PP_H_FLYSTRAP
