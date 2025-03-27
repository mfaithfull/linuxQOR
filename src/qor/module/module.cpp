// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "../../configuration/configuration.h"

#include "module.h"
#include "../../framework/host/host.h"

namespace qor{
	
    Module::Module( const char* name, const char* version) : Library( name, version, false), m_pStaticLibraryList( nullptr )
	{
		framework::Host& host = framework::Host::Instance();
		host.ModuleRegistry().Register(this);
	}
	
	Module::~Module() noexcept
	{
		framework::Host& host = framework::Host::Instance();
		host.ModuleRegistry().Unregister(this);
	}

	bool Module::RegisterLibrary( Library* pLibrary )
	{
		if (pLibrary)
		{
			if (m_pStaticLibraryList == nullptr)
			{
				m_pStaticLibraryList = pLibrary;
			}
			else
			{
				m_pStaticLibraryList->Append(pLibrary);
			}
		}
		return pLibrary != nullptr;
	}
	
	void Module::UnregisterLibrary( Library* pLibrary )
	{
		Library* pSearch = m_pStaticLibraryList;
		if (pSearch == pLibrary)
		{
			m_pStaticLibraryList = const_cast<Library*>(pLibrary->Next());
		}
		else
		{
			while (pSearch != nullptr)
			{
				if (pSearch->Next() == pLibrary)
				{
					pSearch->SetNext(pLibrary->Next());
				}
                else
                {
                    pSearch = const_cast<Library*>(pSearch->Next());
                }
			}
		}
	}

}//qor
