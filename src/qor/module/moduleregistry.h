// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0


#ifndef QOR_PP_H_MODULEREGISTRY
#define QOR_PP_H_MODULEREGISTRY

#include <vector>
#include <algorithm>
#include "module.h"

namespace qor{

    class ModuleRegistry final
	{
	public:

		inline ModuleRegistry() noexcept = default;

        inline ~ModuleRegistry()
        {
            m_Modules.erase(m_Modules.cbegin(), m_Modules.cend());
        }

		inline void Register(Module& module)
		{
            m_Modules.emplace_back(&module);
		}
		
		inline void Unregister(Module& module)
		{
            auto it =  std::find(m_Modules.begin(), m_Modules.end(), &module);
            if(it != m_Modules.end())
            {
                m_Modules.erase(it);
            }
		}

        inline void VisitModules(void(f)(Module*))
        {
            std::for_each( m_Modules.begin(), m_Modules.end(), f );
        }

    private:

        std::vector<Module*> m_Modules;

        ModuleRegistry(const ModuleRegistry&) = delete;
	};

}//qor

#endif//QOR_PP_H_MODULEREGISTRY
