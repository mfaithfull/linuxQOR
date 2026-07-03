// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FLYERMAP
#define QOR_PP_H_FLYERMAP

#include <map>

#include "src/platform/compiler/compiler.h"
#include "src/qor/essentials/objectcontext/anyobject.h"
#include "src/qor/essentials/datastructures/guid.h"

namespace qor{ namespace detail{

    //Thread local container for Flyer instances by GUID. Each Flyer type has 1 current instance, the top of its notional stack.
    class qor_pp_module_interface(QOR_THREAD) FlyerMap final
    {
    public:

        FlyerMap() = default;
        FlyerMap(const FlyerMap&) = delete;
        
        ~FlyerMap()
        {
            m_Map.clear();
        }

        AnyObject& GetByClassID( const GUID* classID )
        {
            auto it = m_Map.find(*classID);
            return it != m_Map.end() ? it->second : AnyObject::NullObject();
        }

        AnyObject Configure(const GUID* classID, AnyObject context)
        {
            AnyObject result;
            if(m_Map.size() > 0)
            {
                auto it = m_Map.find(*classID);
                if( it != m_Map.end())
                {
                    result = it->second;
                    m_Map.erase(*classID);
                }
            }
            m_Map.insert(std::make_pair(*classID, context));
            return result;
        }

        void Unconfigure(const GUID* classID, AnyObject context)
        {
            if(classID)
            {
                auto it = m_Map.find(*classID);
                if(it != m_Map.end())
                {
                    m_Map.erase(it);
                    if(!context.IsNull())
                    {
                        m_Map.insert(std::make_pair(*classID, context));
                    }
                }
            }
        }

        AnyObject Lookup(const GUID* classID)
		{
			AnyObject result;
			auto it = m_Map.find(*classID);
			if (it != m_Map.end())
			{
				result = it->second;
			}
			return result;
		}

    private:

        std::map< GUID, AnyObject > m_Map;
    };

}}//qor::detail

#endif//QOR_PP_H_FLYERMAP
