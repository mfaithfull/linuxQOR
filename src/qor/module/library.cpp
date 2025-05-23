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
#include "src/platform/compiler/compiler.h"
#include "library.h"
#include "module.h"

namespace qor{
	
	Library::Library( const char* name, const char* version, bool bRegister) : m_Name(name), m_Version(version), m_pNext(nullptr)
	{
		if(bRegister)
		{
			ThisModule().RegisterLibrary(this); //Register this library into the chain of the owning module
		}
	}
	
	Library::~Library() noexcept
	{
	}

	const char* Library::Name() const
	{
		return m_Name;
	}

	const char* Library::Version() const
	{
		return m_Version;
	}
		
	const Library* Library::Next() const
	{
		return m_pNext;
	}
	
	void Library::SetNext(const Library* pNext)
	{
		m_pNext = pNext;
	}

	void Library::Append( const Library* pLast )
	{
		Library* pEnd = this;

		while( pEnd->m_pNext != nullptr && pEnd != pLast )
		{
			pEnd = const_cast<Library*>(pEnd->m_pNext);
		}

		if( pEnd != pLast )
		{
			pEnd->m_pNext = pLast;
		}
	}

}//qor

static qor::Library _qor_module("Querysoft Open Runtime: Module Library", 
    qor_pp_stringize(qor_pp_ver_major) "." qor_pp_stringize(qor_pp_ver_minor) "." qor_pp_stringize(qor_pp_ver_patch) "." __DATE__ "_" __TIME__);

qor::Library& qor_module()
{
	return _qor_module;
}