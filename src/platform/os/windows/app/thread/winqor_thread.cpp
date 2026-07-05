// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#ifdef NDEBUG
#include <buildnumber.h>
#endif
#include "src/qor/module/module.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "currentthread.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/factory/internalfactory.h"
#include "src/qor/memory/injection/typeregistry.h"
#include "src/qor/memory/injection/typeregentry.h"
#include "src/qor/memory/reference/newref.h"

extern "C"
{
	qor::Module& ThisModule(void)
	{
		static qor::Module QORModule("Querysoft Open Runtime: Windows Thread Module", qor_pp_module_ver_string );
		static qor::TypeRegEntry< qor::win::CurrentThread, qor::ICurrentThread > reg;  //Register the Windows specific implementation of ICurrentThread
		return QORModule;
	}
}
