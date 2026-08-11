// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#ifdef NDEBUG
#include <buildnumber.h>
#endif
#include "host.h"
#include "src/qor/module/moduleregistry.h"

namespace {
    static qor::ModuleRegistry _theModuleRegistry;
}

extern qor::Module& HostModule(void);

namespace qor{

    Module* TheHost()
    {
        return &(HostModule());
    }
    
    Host::Host() : Module( "Querysoft Open Runtime: Host Module", qor_pp_module_ver_string, false )
    {
        m_ModuleReg = &_theModuleRegistry;
        m_ModuleReg->Register(*this);
    }

    Host::~Host() = default;

    Module& Host::Instance()
    {
        return *(TheHost());
    }
    
	void Host::RegisterModule(Module* module)
	{
        m_ModuleReg->Register(*module);
	}

	void Host::UnregisterModule(Module* module)
	{
        m_ModuleReg->Unregister(*module);
	}

}//qor
