// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_CODEPAGEREGISTRY
#define QOR_PP_H_CODEPAGEREGISTRY

#include <map>
#include "src/qor/essentials/objectcontext/typedany.h"
#include "codepage.h"

//a map of Mibs to CopePage classes

namespace qor{

	class qor_pp_module_interface(QOR_TEXT) CodePageRegistry;
	qor_pp_module_interface(QOR_TEXT) CodePageRegistry* TheCodePageRegistry();

    class qor_pp_module_interface(QOR_TEXT) CodePageRegistry final
	{

	public:

		inline CodePageRegistry() noexcept = default;

		inline ~CodePageRegistry()
		{
			m_codePageMap.erase(m_codePageMap.cbegin(), m_codePageMap.cend());
		}

		inline void Register(Mib mib, AnyObject codePage)
		{
			m_codePageMap.insert(std::pair< Mib, AnyObject >(mib, codePage));
		}
		
		inline void Unregister(Mib mib)
		{
			m_codePageMap.erase(m_codePageMap.find(mib));
		}

		//Get the Code Page
		inline AnyObject GetCodePage(Mib mib)
		{
			auto it = m_codePageMap.find(mib);
			return (it != m_codePageMap.end()) ? it->second : AnyObject::EmptyObject();
		}

    private:

		std::map< Mib, AnyObject > m_codePageMap;
    };
}//qor

#endif//QOR_PP_H_CODEPAGEREGISTRY
