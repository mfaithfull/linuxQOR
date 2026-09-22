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

extern qor::Module& ThisModule(void);

namespace qor{

    Module* TheHost()
    {
        return &(ThisModule());
    }
    
    Host::Host() : Module( "Querysoft Open Runtime: Host Module", qor_pp_module_ver_string, false )
    {
        m_ModuleReg = &_theModuleRegistry;
        m_ModuleReg->Register(*this);
    }

    Host::~Host() noexcept = default;

    Module& Host::Instance()
    {
        return *(TheHost());
    }
    
	void Host::RegisterModule(Module* module)
	{
        if(m_ModuleReg)
        {
            m_ModuleReg->Register(*module);
        }
	}

	void Host::UnregisterModule(Module* module) noexcept
	{
        if(m_ModuleReg)
        {
            m_ModuleReg->Unregister(*module);
        }
	}

}//qor
