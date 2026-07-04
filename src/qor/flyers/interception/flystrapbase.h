// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FLYSTRAPBASE
#define QOR_PP_H_FLYSTRAPBASE

#include "src/qor/essentials/datastructures/tflink.h"
#include "flyer.h"

//Approximately a 'point cut' base class in AOP terminology

namespace qor {

    class qor_pp_module_interface(QOR_INTERCEPTION) FlyStrapBase : public tflink< FlyStrapBase* >
	{
	public:

		FlyStrapBase();
		virtual ~FlyStrapBase() = default;
		void StrapOn( FlyStrapBase* strap );
		virtual void UnStrap();
		virtual void OnDeactivate();
		virtual void OnReactivate();
		virtual void OnActivate();

		template< class TFSDerived > TFSDerived* Next(TFSDerived* dummy = nullptr)
		{
			TFSDerived* next = dummy;
			if( m_Next )
			{
				next = dynamic_cast< TFSDerived* >(m_Next->Item());
			}
			return next;
		}

	protected:

		virtual void BeforeStrapOn( FlyStrapBase* strap );
		virtual void AfterStrapOn( FlyStrapBase* strap );

	};

}//qor

#endif//QOR_PP_H_FLYSTRAPBASE
