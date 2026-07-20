// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_UI_TERMCON
#define QOR_PP_H_OS_WINDOWS_UI_TERMCON

#include "src/framework/ui/terminal/terminal.h"
#include "src/platform/os/windows/ui/wincon/console.h"

//An actual Windows Terminal or a legacy Windows Console in Terminal Emulation Mode
namespace qor { namespace ui { namespace win {
    
    class qor_pp_module_interface(QOR_WINTERMINAL) Terminal : public qor::ui::Terminal
    {
    public:

        Terminal();
        ~Terminal() noexcept;

        virtual void Setup();
        virtual void Shutdown();
        virtual Dimensions Size();

        bool IsValid();
        
    protected:

        virtual Colour ComputeColourSupport();
        
    private:
        
        bool QueryVT();        

        ref_of<Console>::type m_console;
        bool m_validTerminal{false};

        unsigned long m_originalOutputMode{0};
        unsigned long m_originalInputMode{0};
        unsigned int m_codePage{0};
        unsigned int m_outputCodePage{0};

    };
}}}//qor::ui::win

#endif//QOR_PP_H_OS_WINDOWS_UI_TERMCON