// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/newref.h"
#include "flystrapbase.h"

//Approximately a 'point cut' base class in AOP terminology

namespace qor {

    FlyStrapBase::FlyStrapBase() : tflink< FlyStrapBase* >( this ){ }

	FlyStrapBase::FlyStrapBase(const FlyStrapBase&) : tflink< FlyStrapBase* >( this ){ }

	FlyStrapBase::~FlyStrapBase() = default;

	void FlyStrapBase::BeforeStrapOn(FlyStrapBase* /*strap*/){ }

	void FlyStrapBase::StrapOn(FlyStrapBase* strap)
	{
		BeforeStrapOn( strap );
		Append( strap );
		AfterStrapOn( strap );
	}

	void FlyStrapBase::AfterStrapOn( FlyStrapBase* /*strap*/ ){ }
	void FlyStrapBase::UnStrap(){ }
	void FlyStrapBase::OnDeactivate(){ }
	void FlyStrapBase::OnReactivate(){ }
	void FlyStrapBase::OnActivate(){ }

}//qor
