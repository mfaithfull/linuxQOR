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

#ifndef QOR_PP_H_FLYERMAP
#define QOR_PP_H_FLYERMAP

#include <map>

#include "src/platform/compiler/compiler.h"
#include "src/qor/objectcontext/objectcontextbase.h"
#include "src/qor/datastructures/guid.h"

namespace qor{ namespace framework{

    class qor_pp_module_interface(QOR_THREAD) FlyerMap final
    {
    public:

        FlyerMap() = default;
        ~FlyerMap() = default;

        ObjectContextBase& GetByClassID( const GUID* classID )
        {
            auto it = m_Map.find(*classID);
            return it != m_Map.end() ? it->second : ObjectContextBase::NullContext();
        }

        ObjectContextBase Configure(const GUID* classID, ObjectContextBase context)
        {
            ObjectContextBase result;
            if(m_Map.size() > 0)
            {
                auto it = m_Map.find(*classID);
                if( it != m_Map.end());
                {
                    result = it->second;
                    m_Map.erase(*classID);
                }
            }
            m_Map.insert(std::make_pair(*classID, context));
            return result;
        }

        void Unconfigure(const GUID* classID, ObjectContextBase context)
        {
            m_Map.erase(m_Map.find(*classID));
            if(!context.IsNull())
            {
                m_Map.insert(std::make_pair(*classID, context));
            }
        }

        ObjectContextBase Lookup(const GUID* classID)
		{
			ObjectContextBase result;
			auto it = m_Map.find(*classID);
			if (it != m_Map.end())
			{
				result = it->second;
			}
			return result;
		}

    private:

        std::map< GUID, ObjectContextBase > m_Map;
    };

}}//qor::framework

#endif//QOR_PP_H_FLYERMAP
