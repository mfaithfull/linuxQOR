// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "graphcon.h"
#include "src/qor/flyers/error/error.h"
#include "src/platform/os/windows/common/constants.h"

namespace qor { namespace ui{ namespace win {

    GraphicalConsole::GraphicalConsole(int width, int height, int fontw, int fonth)
    {        
		if(m_Console.IsRedirected())
		{
			m_Console.Reallocate();
			//auto newScreenBuffer = m_Console.CreateScreenBuffer();
			//m_Console.SetActiveScreenBuffer(newScreenBuffer);
		}
        Construct(width, height, fontw, fonth);
    }

    GraphicalConsole::~GraphicalConsole()
    {        
		delete[] m_bufScreen;
    }

	int GraphicalConsole::Construct(int width, int height, int fontw, int fonth)
	{
		if(m_Console.OutputFile() == Invalid_Handle_Value)
        {
			continuable("Bad Handle");
            return -1;
        }

		m_nScreenWidth = width;
		m_nScreenHeight = height;

		// Change console visual size to a minimum so ScreenBuffer can shrink
		// below the actual visual size
		m_rectWindow = { 0, 0, 1, 1 };
		auto screenBuffer = m_Console.GetActiveScreenBuffer();
		screenBuffer->SetWindowInfo(true, &m_rectWindow);

		// Set the size of the screen buffer
		platform::win::Coord coord = { (short)m_nScreenWidth, (short)m_nScreenHeight };
		if (!screenBuffer->SetSize(coord))
        {
			continuable("SetConsoleScreenBufferSize");
        }

		// Assign screen buffer to the console
		if (!m_Console.SetActiveScreenBuffer(screenBuffer))
        {
			continuable("SetConsoleActiveScreenBuffer");
            return -1;
        }
		
		// Set the font size now that the screen buffer has been assigned to the console
        platform::win::ConsoleFontInfoEx cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.font = 0;
		cfi.fontSize.x = fontw;
		cfi.fontSize.y = fonth;
		cfi.FontFamily = FF_DontCare;
		cfi.FontWeight = FW_Normal;

		wcscpy_s(cfi.FaceName, L"Consolas");
		if (!screenBuffer->SetCurrentFontEx(false, &cfi))
        {
			continuable("SetCurrentConsoleFontEx");
            return -1;
        }

		// Get screen buffer info and check the maximum allowed window size. Return
		// error if exceeded, so user knows their dimensions/fontsize are too large
        platform::win::ConsoleScreenBufferInfo csbi;
		if (!screenBuffer->GetInfo(&csbi))
        {
			continuable("GetConsoleScreenBufferInfo");
            return -1;
        }

		if (m_nScreenHeight > csbi.maximumWindowSize.y)
        {
			continuable("Screen Height / Font Height Too Big");
            return -1;
        }

		if (m_nScreenWidth > csbi.maximumWindowSize.x)
        {
			continuable("Screen Width / Font Width Too Big");
            return -1;
        }

		// Set Physical Console Window Size
		m_rectWindow = { 0, 0, static_cast<short>(m_nScreenWidth - 1), static_cast<short>(m_nScreenHeight - 1) };

		if (!screenBuffer->SetWindowInfo(true, &m_rectWindow))
        {
			continuable("SetConsoleWindowInfo");
            return -1;
        }

		// Set flags to allow mouse input
		if (!m_Console.GetInput()->SetMode(EnableExtendedFlags | EnableWindowInput | EnableMouseInput))
        {
			continuable("SetConsoleMode");
            return -1;
        }

		// Allocate memory for screen buffer
		m_bufScreen = new qor::platform::win::CharInfo[m_nScreenWidth*m_nScreenHeight];
		memset(m_bufScreen, 0, sizeof(qor::platform::win::CharInfo) * m_nScreenWidth * m_nScreenHeight);

		m_Console.SetCtrlHandler((CtrlHandlerCallback)[](unsigned long)->int
			{
				return 1;
			}, true);
		return 1;
	}

	void GraphicalConsole::Present()
	{
		auto screenBuffer = m_Console.GetActiveScreenBuffer();
		platform::win::ConsoleScreenBufferInfoEx infoEx;
		m_Console.GetActiveScreenBuffer()->GetInfoEx(&infoEx);
		m_Console.GetActiveScreenBuffer()->WriteOutput(m_bufScreen, { (short)m_nScreenWidth, (short)m_nScreenHeight }, { 0,0 }, &m_rectWindow);
	}

	void GraphicalConsole::Draw(int x, int y, short c, short col)
	{
		if (x >= 0 && x < m_nScreenWidth && y >= 0 && y < m_nScreenHeight)
		{
			m_bufScreen[y * m_nScreenWidth + x].Char.UnicodeChar = c;
			m_bufScreen[y * m_nScreenWidth + x].attributes = col;
		}
	}

	void GraphicalConsole::Clip(int &x, int &y)
	{
		if (x < 0) x = 0;
		if (x >= m_nScreenWidth) x = m_nScreenWidth;
		if (y < 0) y = 0;
		if (y >= m_nScreenHeight) y = m_nScreenHeight;
	}

    void GraphicalConsole::Fill(int x1, int y1, int x2, int y2, short c, short col)
	{
		Clip(x1, y1);
		Clip(x2, y2);
		for (int x = x1; x < x2; x++)
			for (int y = y1; y < y2; y++)
				Draw(x, y, c, col);
	}

	void GraphicalConsole::DrawString(int x, int y, std::wstring c, short col)
	{
		for (size_t i = 0; i < c.size(); i++)
		{
			m_bufScreen[y * m_nScreenWidth + x + i].Char.UnicodeChar = c[i];
			m_bufScreen[y * m_nScreenWidth + x + i].attributes = col;
		}
	}

	void GraphicalConsole::DrawStringAlpha(int x, int y, std::wstring c, short col)
	{
		for (size_t i = 0; i < c.size(); i++)
		{
			if (c[i] != L' ')
			{
				m_bufScreen[y * m_nScreenWidth + x + i].Char.UnicodeChar = c[i];
				m_bufScreen[y * m_nScreenWidth + x + i].attributes = col;
			}
		}
	}

	void GraphicalConsole::DrawLine(int x1, int y1, int x2, int y2, short c, short col)
	{
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1; dy = y2 - y1;
		dx1 = abs(dx); dy1 = abs(dy);
		px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
		if (dy1 <= dx1)
		{
			if (dx >= 0)
				{ x = x1; y = y1; xe = x2; }
			else
				{ x = x2; y = y2; xe = x1;}

			Draw(x, y, c, col);
			
			for (i = 0; x<xe; i++)
			{
				x = x + 1;
				if (px<0)
					px = px + 2 * dy1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				Draw(x, y, c, col);
			}
		}
		else
		{
			if (dy >= 0)
				{ x = x1; y = y1; ye = y2; }
			else
				{ x = x2; y = y2; ye = y1; }

			Draw(x, y, c, col);

			for (i = 0; y<ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				Draw(x, y, c, col);
			}
		}
	}

	void GraphicalConsole::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c, short col)
	{
		DrawLine(x1, y1, x2, y2, c, col);
		DrawLine(x2, y2, x3, y3, c, col);
		DrawLine(x3, y3, x1, y1, c, col);
	}

	// https://www.avrfreaks.net/sites/default/files/triangles.c
	void GraphicalConsole::FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c, short col)
	{
		auto SWAP = [](int &x, int &y) { int t = x; x = y; y = t; };
		auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Draw(i, ny, c, col); };
		
		int t1x, t2x, y, minx, maxx, t1xp, t2xp;
		bool changed1 = false;
		bool changed2 = false;
		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;
		// Sort vertices
		if (y1>y2) { SWAP(y1, y2); SWAP(x1, x2); }
		if (y1>y3) { SWAP(y1, y3); SWAP(x1, x3); }
		if (y2>y3) { SWAP(y2, y3); SWAP(x2, x3); }

		t1x = t2x = x1; y = y1;   // Starting points
		dx1 = (int)(x2 - x1); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y2 - y1);

		dx2 = (int)(x3 - x1); if (dx2<0) { dx2 = -dx2; signx2 = -1; }
		else signx2 = 1;
		dy2 = (int)(y3 - y1);

		if (dy1 > dx1) {   // swap values
			SWAP(dx1, dy1);
			changed1 = true;
		}
		if (dy2 > dx2) {   // swap values
			SWAP(dy2, dx2);
			changed2 = true;
		}

		e2 = (int)(dx2 >> 1);
		// Flat top, just process the second half
		if (y1 == y2) goto next;
		e1 = (int)(dx1 >> 1);

		for (int i = 0; i < dx1;) {
			t1xp = 0; t2xp = 0;
			if (t1x<t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i<dx1) {
				i++;
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) t1xp = signx1;//t1x += signx1;
					else          goto next1;
				}
				if (changed1) break;
				else t1x += signx1;
			}
			// Move line
		next1:
			// process second line until y value is about to change
			while (1) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;//t2x += signx2;
					else          goto next2;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next2:
			if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
			if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
			drawline(minx, maxx, y);    // Draw line from min to max points found on the y
										 // Now increase y
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y == y2) break;

		}
	next:
		// Second half
		dx1 = (int)(x3 - x2); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y3 - y2);
		t1x = x2;

		if (dy1 > dx1) {   // swap values
			SWAP(dy1, dx1);
			changed1 = true;
		}
		else changed1 = false;

		e1 = (int)(dx1 >> 1);

		for (int i = 0; i <= dx1; i++) 
		{
			t1xp = 0; t2xp = 0;
			if (t1x<t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i<dx1) 
			{
				e1 += dy1;
				while (e1 >= dx1) 
				{
					e1 -= dx1;
					if (changed1) 
					{ t1xp = signx1; break; }//t1x += signx1;
					else
					goto next3;
				}
				if (changed1) break;
				else   	   	  t1x += signx1;
				if (i<dx1) i++;
			}
		next3:
			// process second line until y value is about to change
			while (t2x != x3) 
			{
				e2 += dy2;
				while (e2 >= dx2) 
				{
					e2 -= dx2;
					if (changed2) t2xp = signx2;
					else          goto next4;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next4:

			if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
			if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
			drawline(minx, maxx, y);   										
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y>y3) return;
		}
	}

	void GraphicalConsole::DrawCircle(int xc, int yc, int r, short c, short col)
	{
		int x = 0;
		int y = r;
		int p = 3 - 2 * r;
		if (!r) return;

		while (y >= x) // only formulate 1/8 of circle
		{
			Draw(xc - x, yc - y, c, col);//upper left left
			Draw(xc - y, yc - x, c, col);//upper upper left
			Draw(xc + y, yc - x, c, col);//upper upper right
			Draw(xc + x, yc - y, c, col);//upper right right
			Draw(xc - x, yc + y, c, col);//lower left left
			Draw(xc - y, yc + x, c, col);//lower lower left
			Draw(xc + y, yc + x, c, col);//lower lower right
			Draw(xc + x, yc + y, c, col);//lower right right
			if (p < 0) p += 4 * x++ + 6;
			else p += 4 * (x++ - y--) + 10;
		}
	}

    void GraphicalConsole::FillCircle(int xc, int yc, int r, short c, short col)
	{
		// Taken from wikipedia
		int x = 0;
		int y = r;
		int p = 3 - 2 * r;
		if (!r) return;

		auto drawline = [&](int sx, int ex, int ny)
		{
			for (int i = sx; i <= ex; i++)
				Draw(i, ny, c, col);
		};

		while (y >= x)
		{
			// Modified to draw scan-lines instead of edges
			drawline(xc - x, xc + x, yc - y);
			drawline(xc - y, xc + y, yc - x);
			drawline(xc - x, xc + x, yc + y);
			drawline(xc - y, xc + y, yc + x);
			if (p < 0) p += 4 * x++ + 6;
			else p += 4 * (x++ - y--) + 10;
		}
	};    
}}}//qor::ui::win