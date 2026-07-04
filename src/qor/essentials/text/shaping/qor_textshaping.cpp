// Copyright Querysoft Limited 2008 - 2025
//

#include "src/configuration/configuration.h"
#ifdef NDEBUG
#include <buildnumber.h>
#endif
#include "src/qor/module/module.h"

extern "C"
{
	qor::Module& ThisModule(void)
	{
		static qor::Module QORModule("Querysoft Open Runtime: Text Shaping Module", qor_pp_module_ver_string );
		return QORModule;
	}
}
