// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#ifdef NDEBUG
#include <buildnumber.h>
#endif
#include "src/qor/module/module.h"

#include "resourcehub.h"

extern "C"
{
	qor::Module& ThisModule(void)
	{
		static qor::Module QORModule("Querysoft Open Runtime: Resources Module", qor_pp_module_ver_string );

		static qor::TypeRegEntry< qor::framework::ResourceHub, qor::framework::ResourceHub > regResourceHub;
		return QORModule;
	}
}
