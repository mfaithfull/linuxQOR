// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_UI_ADVANCEDCONSOLE
#define QOR_PP_H_OS_WINDOWS_UI_ADVANCEDCONSOLE

#include "consolehelper.h"

namespace qor { namespace ui { namespace win {

	class qor_pp_module_interface(QOR_WINCONSOLE) AdvancedConsole
	{
	public:

		AdvancedConsole(void* outFile, void* inFile, int width, int height, int fontw, int fonth);
		~AdvancedConsole();
		
        int Construct(int width, int height, int fontw, int fonth);
		void Present();
        void Draw(int x, int y, short c = 0x2588, short col = 0x000F);
        void Clip(int &x, int &y);
        void Fill(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F);
        void DrawString(int x, int y, std::wstring c, short col = 0x000F);
        void DrawStringAlpha(int x, int y, std::wstring c, short col = 0x000F);
        void DrawLine(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F);
        void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c = 0x2588, short col = 0x000F);
        void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c = 0x2588, short col = 0x000F);
        void DrawCircle(int xc, int yc, int r, short c = 0x2588, short col = 0x000F);
        void FillCircle(int xc, int yc, int r, short c = 0x2588, short col = 0x000F);

	private:

        ConsoleHelper m_helper;
        int m_nScreenWidth;
	    int m_nScreenHeight;
	    qor::platform::win::CharInfo* m_bufScreen;
	    std::wstring m_sAppName;
	    //void* m_hOriginalConsole;
	    //qor::platform::win::ConsoleScreenBufferInfo m_OriginalConsoleInfo;
	    void* m_hConsole{nullptr};
	    void* m_hConsoleIn{nullptr};
	    qor::platform::win::SmallRect m_rectWindow;
	    //short m_keyOldState[256] = { 0 };
	    //short m_keyNewState[256] = { 0 };
	    //bool m_mouseOldState[5] = { 0 };
	    //bool m_mouseNewState[5] = { 0 };
	    bool m_bConsoleInFocus = true;	
	    //bool m_bEnableSound = false;		
	};

}}}//qor::ui::win

#endif//QOR_PP_H_OS_WINDOWS_UI_ADVANCEDCONSOLE
