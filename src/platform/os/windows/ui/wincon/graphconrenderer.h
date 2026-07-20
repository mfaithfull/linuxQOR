// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_UI_GRAPHCONRENDERER
#define QOR_PP_H_OS_WINDOWS_UI_GRAPHCONRENDERER

#include "src/platform/os/windows/common/structures.h"
#include "graphcon.h"
#include "graphconsurface.h"

namespace qor { namespace ui { namespace win {

	class qor_pp_module_interface(QOR_WINCONSOLE) GraphConRenderer
	{
	public:

		GraphConRenderer(GraphicalConsole* console, GraphConSurface* surface = nullptr);
		~GraphConRenderer();
        void SetSurface(GraphConSurface* surface);
        void Present();
        void Draw(int x, int y, short c = 0x2588, short col = 0x000F);
        void DrawString(int x, int y, std::wstring c, short col = 0x000F);
        void DrawStringAlpha(int x, int y, std::wstring c, short col = 0x000F);
        void Clip(int &x, int &y);
        void Fill(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F);
        void DrawLine(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F);
        void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c = 0x2588, short col = 0x000F);
        void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c = 0x2588, short col = 0x000F);
        void DrawCircle(int xc, int yc, int r, short c = 0x2588, short col = 0x000F);
        void FillCircle(int xc, int yc, int r, short c = 0x2588, short col = 0x000F);

	private:
        
        GraphicalConsole* m_console{nullptr};
        GraphConSurface* m_surface{nullptr};
	};

}}}//qor::ui::win

#endif//QOR_PP_H_OS_WINDOWS_UI_GRAPHCONSURFACE
