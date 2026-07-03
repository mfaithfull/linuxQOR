// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/platform/compiler/compiler.h"
#include "library.h"
#include "module.h"

namespace qor{
	
	Library::Library( const char* name, const char* version, bool doRegister) : 
		m_Name(name), 
		m_Version(version), 
		m_Next(nullptr)
	{
		if(doRegister)
		{
			ThisModule().RegisterLibrary(this); //Register this library into the chain of the owning module
		}
	}
	
	void Library::Append( const Library* last )
	{
		Library* end = this;

		while( end->m_Next != nullptr && end != last )
		{
			end = const_cast<Library*>(end->m_Next);
		}

		if( end != last )
		{
			end->m_Next = last;
		}
	}

}//qor

static qor::Library _qor_library("Querysoft Open Runtime: Module Library", qor_pp_module_ver_string );

qor::Library& qor_module()
{
	return _qor_library;
}