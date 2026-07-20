// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_UI_ADVANCEDCONSOLE
#define QOR_PP_H_OS_WINDOWS_UI_ADVANCEDCONSOLE

#include "console.h"
#include "graphconsurface.h"

//Legacy Windows Console in maxed out 'graphics' mode.

namespace qor { namespace ui { namespace win {

	class qor_pp_module_interface(QOR_WINCONSOLE) GraphicalConsole
	{
	public:

		GraphicalConsole(int width, int height, int fontw, int fonth);
		~GraphicalConsole();
		
        int Construct(int width, int height, int fontw, int fonth);
		int Width();
		int Height();
		void Present(GraphConSurface* surface);
		ref_of<GraphConSurface>::type CreateSurface();
		
	private:

        Console m_Console;
        int m_nScreenWidth;
	    int m_nScreenHeight;
	    qor::platform::win::SmallRect m_rectWindow;	    
	};

}}}//qor::ui::win

#endif//QOR_PP_H_OS_WINDOWS_UI_ADVANCEDCONSOLE
