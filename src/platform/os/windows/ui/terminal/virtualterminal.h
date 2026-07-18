// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_UI_VIRTUAL_TERMINAL
#define QOR_PP_H_OS_WINDOWS_UI_VIRTUAL_TERMINAL

#include "src/platform/os/windows/common/structures.h"

namespace qor { namespace ui { namespace win {

	class qor_pp_module_interface(QOR_WINCONSOLE) VirtualTerminal
	{
	public:
        
        VirtualTerminal();        
        ~VirtualTerminal();

        void RI();//ESC M - Reverse index, inverse of \n
        void DECSC();//ESC 7 Save cursor in memory
        void DECSR();//ESC 8 Restore cursor from memory
        void CUU(int n);//ESC [ <n> A Cursor Up by <n>
        void CUD(int n);//ESC [ <n> B Cursor Down by <n>
        void CUF(int n);//ESC [ <n> C Cursor Forward (Right) by <n>
        void CUB(int n);//ESC [ <n> D Cursor Backward (Left) by <n>
        void CNL(int n);//ESC [ <n> E Cursor Down <n> lines
        void CPL(int n);//ESC [ <n> F Cursor Up <n> lines
        void CHA(int n);//ESC [ <n> G Cursor to absolute position n in current line
        void VPA(int n);//ESC [ <n> d Cursor to absolute position n in current column
        void CUP(int x, int y);//ESC [ <x> ; <y> H Cursor to x,y in viewport
        void HVP(int y, int x);//ESC [ <y> ; <x> f Cursor to x,y in viewport
        void ANSISYSSC();//ESC [ s Save Cursor
        void ANSISYSRC();//ESC [ u Restore Cursor
        void ATT160(bool);//ESC [ ? 12 h Start the cursor blinking
        void _ATT160();//ESC [ ? 12 l Stop the cursor blinking
        void DECTCEM(bool);//ESC [ ? 25 h Show Cursor
        void _DECTCEM();//ESC [ ? 25 l Hide Cursor

        void DECSCUSR(int n);//ESC [ <n> SP q Cursor shape 0 - 6
        void SU(int n);//ESC [ <n> S Scroll up by n
        void SD(int n);//ESC [ <n> T Scroll down by n
        void ICH(int n);//ESC [ <n> @ Insert n spaces, text shifts right
        void DCH(int n);//ESC [ <n> P Delete n characters pulling text in
        void ECH(int n);//ESC [ <n> X Erase n chars by overwrite with space
        void IL(int n);//ESC [ <n> L Insert n lines
        void DL(int n);//ESC [ <n> M Delete n lines from buffer
        void ED(int n);//ESC [ <n> J Replace all text in the viewport with space
        void EL(int n);//ESC [ <n> K Replace all text on the line with space
        void SGR(int n, ...);//ESC [ <n,n,n,n...> m Set the format
        
        //...
    private:

    };

}}}//qor::ui::win

#endif//QOR_PP_H_OS_WINDOWS_UI_VIRTUAL_TERMINAL
