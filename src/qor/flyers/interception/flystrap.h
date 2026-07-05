// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FLYSTRAP
#define QOR_PP_H_FLYSTRAP

#include "flystrapbase.h"
#include "callcontext.h"

namespace qor {

    class FlyStrap : public FlyStrapBase
	{
	public:

        qor_pp_module_interface(QOR_INTERCEPTION) FlyStrap();
		qor_pp_module_interface(QOR_INTERCEPTION) FlyStrap(const FlyStrap&);
		FlyStrap& operator = (const FlyStrap&) = delete;
        qor_pp_module_interface(QOR_INTERCEPTION) FlyStrap( FlyStrapBase* );
		qor_pp_module_interface(QOR_INTERCEPTION) qor_pp_noinline virtual ~FlyStrap() = default;

		qor_pp_module_interface(QOR_INTERCEPTION) virtual bool PreCondition( CallContext* call, IFunctionContext* function );		
		qor_pp_module_interface(QOR_INTERCEPTION) virtual bool OnAssignmentCondition( CallContext* call );
		qor_pp_module_interface(QOR_INTERCEPTION) virtual bool PostCondition( CallContext* call );
		qor_pp_module_interface(QOR_INTERCEPTION) virtual bool Pre( CallContext* call, IFunctionContext* function );
		qor_pp_module_interface(QOR_INTERCEPTION) virtual bool OnReturnAssignment( CallContext* call );
		qor_pp_module_interface(QOR_INTERCEPTION) virtual bool OnReturn( CallContext* call );
		qor_pp_module_interface(QOR_INTERCEPTION) virtual bool Post( CallContext* call );

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

		qor_pp_module_interface(QOR_INTERCEPTION) bool CallPre( CallContext* call, IFunctionContext* function );
		qor_pp_module_interface(QOR_INTERCEPTION) bool CallOnReturnAssignment( CallContext* call );
		qor_pp_module_interface(QOR_INTERCEPTION) bool CallPost( CallContext* call );

	};

}//qor

#endif//QOR_PP_H_FLYSTRAP
