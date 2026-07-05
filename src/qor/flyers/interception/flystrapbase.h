// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FLYSTRAPBASE
#define QOR_PP_H_FLYSTRAPBASE

#include "src/platform/compiler/compiler.h"
#include "src/qor/essentials/datastructures/tflink.h"
#include "flyer.h"

//Approximately a 'point cut' base class in AOP terminology

namespace qor {

    class FlyStrapBase : public tflink< FlyStrapBase* >
	{
	public:

		qor_pp_module_interface(QOR_INTERCEPTION) FlyStrapBase();
		qor_pp_module_interface(QOR_INTERCEPTION) FlyStrapBase(const FlyStrapBase&);
		FlyStrapBase& operator = (const FlyStrapBase&) = delete;
		qor_pp_module_interface(QOR_INTERCEPTION) virtual ~FlyStrapBase();
		qor_pp_module_interface(QOR_INTERCEPTION) void StrapOn( FlyStrapBase* strap );
		qor_pp_module_interface(QOR_INTERCEPTION) virtual void UnStrap();
		qor_pp_module_interface(QOR_INTERCEPTION) virtual void OnDeactivate();
		qor_pp_module_interface(QOR_INTERCEPTION) virtual void OnReactivate();
		qor_pp_module_interface(QOR_INTERCEPTION) virtual void OnActivate();

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

		qor_pp_module_interface(QOR_INTERCEPTION) virtual void BeforeStrapOn( FlyStrapBase* strap );
		qor_pp_module_interface(QOR_INTERCEPTION) virtual void AfterStrapOn( FlyStrapBase* strap );

	};

}//qor

#endif//QOR_PP_H_FLYSTRAPBASE
