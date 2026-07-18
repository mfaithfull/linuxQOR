// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_UI_TERMCON
#define QOR_PP_H_OS_WINDOWS_UI_TERMCON

#include "console.h"

//An actual Windows Terminal or a legacy Windows Console in Terminal Emulation Mode
namespace qor { namespace ui { namespace win {

    //TODO: This should provide a general ITerminal interface in common with Linux/etc terminals
    class qor_pp_module_interface(QOR_WINCONSOLE) TermCon
    {
    public:

        TermCon();
        ~TermCon();

        bool IsValid();
        
    private:

        bool SetupLocalTerminal();
        bool QueryVT();
        void NoCurrentConsole();
        void RebindCrtHandlesToStdHandles();

        ref_of<Console>::type m_console;
        ConsoleHelper m_helper;
        bool m_validTerminal;
    };
}}}//qor::ui::win

#endif//QOR_PP_H_OS_WINDOWS_UI_TERMCON