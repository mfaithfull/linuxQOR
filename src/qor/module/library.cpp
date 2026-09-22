// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/platform/compiler/compiler.h"
#include "library.h"
#include "module.h"

namespace qor{
	
	Library::Library(const char* name, const char* version, bool doRegister) : 
		m_Name(name), 
		m_Version(version)
	{
		if(doRegister)
		{
			ThisModule().RegisterLibrary(this); //Register this library into the chain of the owning module
		}
	}
	
	void Library::Append(const Library* last) const
	{
		const Library* end = this;

		while(end->m_Next != nullptr && end != last)
		{
			end = end->m_Next;
		}

		if(end != last)
		{
			Library* modfiableEnd;
			memcpy(&modfiableEnd, &end, sizeof(Library*));
			modfiableEnd->m_Next = last;
		}
	}

}//qor

static qor::Library _qor_library("Querysoft Open Runtime: Module Library", qor_pp_module_ver_string );

qor::Library& qor_module() noexcept
{
	return _qor_library;
}