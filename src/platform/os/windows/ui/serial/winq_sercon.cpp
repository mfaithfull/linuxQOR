// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#ifdef NDEBUG
#include <buildnumber.h>
#endif
#include "src/qor/module/module.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/factory/internalfactory.h"
#include "src/qor/memory/injection/typeregistry.h"
#include "src/qor/memory/injection/typeregentry.h"
#include "src/qor/memory/reference/newref.h"

#include "sercon.h"

extern "C"
{
	qor::Module& ThisModule(void)
	{
		static qor::Module QORModule("Querysoft Open Runtime: Windows Serial Console Module", qor_pp_module_ver_string );
		//Register the Windows specific implementation of Serial Console
		static qor::TypeRegEntry< qor::ui::win::SerCon, qor::ui::SerialConsole > regTerminal;
		return QORModule;
	}
}

