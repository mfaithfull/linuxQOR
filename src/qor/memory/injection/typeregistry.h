// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TYPEREGISTRY
#define QOR_PP_H_TYPEREGISTRY

#include <map>
#include "src/qor/essentials/datastructures/guid.h"
#include "src/qor/essentials/objectcontext/typedany.h"

namespace qor{

	class qor_pp_module_interface(QOR_INJECTION) TypeRegistry;
	qor_pp_module_interface(QOR_INJECTION) TypeRegistry* TheTypeRegistry();

    class qor_pp_module_interface(QOR_INJECTION) TypeRegistry final
	{
	public:

		inline TypeRegistry() noexcept = default;
		TypeRegistry(const TypeRegistry&) = delete;

		inline ~TypeRegistry()
		{
			m_regMap.erase(m_regMap.cbegin(), m_regMap.cend());
		}

		inline void Register(GUID classID, AnyObject factory)
		{
			m_regMap.insert(std::pair< GUID, AnyObject >(classID, factory));
		}
		
		inline void Unregister(GUID classID)
		{
			m_regMap.erase(m_regMap.find(classID));
		}

		//Get the factory for creating instances of a class by GUID
		inline AnyObject GetFactory(GUID classID)
		{
			auto it = m_regMap.find(classID);
			return (it != m_regMap.end()) ? it->second : AnyObject::EmptyObject();
		}

	private:

		std::multimap< GUID, AnyObject > m_regMap;		//A map from class identifiers to factories for creating instances

	};

}//qor


#endif//QOR_PP_H_TYPEREGISTRY
