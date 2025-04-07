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

#ifndef QOR_PP_H_TYPEREGISTRY
#define QOR_PP_H_TYPEREGISTRY

#include <map>
#include "src/qor/datastructures/guid.h"
#include "src/qor/objectcontext/typedany.h"

namespace qor{

	class qor_pp_module_interface(QOR_INJECTION) TypeRegistry;
	qor_pp_module_interface(QOR_INJECTION) TypeRegistry* TheTypeRegistry();

    class qor_pp_module_interface(QOR_INJECTION) TypeRegistry final
	{

	public:

		inline TypeRegistry() noexcept = default;

		inline ~TypeRegistry() noexcept = default;

		inline void Register(GUID classID, AnyObject factory)
		{
			m_regMap.insert(std::pair< GUID, AnyObject >(classID, factory));
		}
		
		inline void Unregister(GUID classID)
		{
			m_regMap.erase(m_regMap.find(classID));
		}

		//Get the factory for creating instances of a class by ID
		inline AnyObject GetFactory(GUID classID)
		{
			auto it = m_regMap.find(classID);
			return (it != m_regMap.end()) ? it->second : AnyObject::NullObject();
		}

	private:

		std::multimap< GUID, AnyObject > m_regMap;		//A map from class identifiers to factories for creating instances

	};

}//qor


#endif//QOR_PP_H_TYPEREGISTRY
