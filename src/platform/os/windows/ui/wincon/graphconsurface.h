// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_UI_GRAPHCONSURFACE
#define QOR_PP_H_OS_WINDOWS_UI_GRAPHCONSURFACE

#include "src/platform/os/windows/common/structures.h"

namespace qor { namespace ui { namespace win {

	class qor_pp_module_interface(QOR_WINCONSOLE) GraphConSurface
	{
	public:

		GraphConSurface(int width, int height) : m_width(std::min(512, width)), m_height(std::min(512, height))
        {
    		m_surface = new platform::win::CharInfo[m_width * m_height];
    		memset(m_surface, 0, sizeof(platform::win::CharInfo) * m_width * m_height);
        }

		~GraphConSurface()
        {
            delete [] m_surface;
        }

        platform::win::CharInfo* operator()()
        {
            return m_surface;
        }

	private:
        
        int m_width;
	    int m_height;
	    platform::win::CharInfo* m_surface{nullptr};
	};

}}}//qor::ui::win

#endif//QOR_PP_H_OS_WINDOWS_UI_GRAPHCONSURFACE
