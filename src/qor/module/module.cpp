// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/platform/compiler/compiler.h"
#include "module.h"

namespace qor
{
	qor_pp_import Module* TheHost();

	/*We'll get a linker warning when, and only when, we link this file into qor_host which implements TheHost
	In every other case linking in this library will make the module import TheHost from qor_host. The warning is
	harmless so we supress in, in the qor_host build only.*/

    Module::Module(const char* name, const char* version, bool doRegister) : Library(name, version, false)
	{
		if(doRegister)
		{
			Module* host = TheHost();
			if(host)
			{
				host->RegisterModule(this);
			}
		}
	}
	
	Module::~Module() noexcept
	{
		Module* host = TheHost();
		if(host)
		{
			host->UnregisterModule(this);
		}
	}

	bool Module::RegisterLibrary(Library* library)
	{
		if (library)
		{
			if (m_StaticLibraryList == nullptr)
			{
				m_StaticLibraryList = library;
			}
			else
			{
				m_StaticLibraryList->Append(library);
			}
		}
		return library != nullptr;
	}
	
	void Module::UnregisterLibrary(Library* library)
	{
		Library* search = m_StaticLibraryList;
		if (search == library)
		{
			m_StaticLibraryList = const_cast<Library*>(library->Next());
		}
		else
		{
			while (search != nullptr)
			{
				if (search->Next() == library)
				{
					search->SetNext(library->Next());
				}
                else
                {
                    search = const_cast<Library*>(search->Next());
                }
			}
		}
	}

    ModuleRegistry* Module::Modules()
    {
        return m_ModuleReg;
    }

}//qor
