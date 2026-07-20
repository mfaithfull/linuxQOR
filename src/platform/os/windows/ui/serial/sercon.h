// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_UI_SERCON
#define QOR_PP_H_OS_WINDOWS_UI_SERCON

#include "src/framework/ui/serial/serialconsole.h"
#include "src/platform/os/windows/ui/wincon/console.h"

qor_pp_module_will_provide(QOR_WINSERCON, SerialConsole)

namespace qor { namespace ui { namespace win {
    
    class qor_pp_module_interface(QOR_WINSERCON) SerCon : public qor::ui::SerialConsole
    {
    public:

        SerCon();
        ~SerCon() noexcept;

        virtual void Setup();
        virtual void Shutdown();
        virtual string_t ReadLine();
        virtual void WriteLine(const string_t& line);
        virtual char_t ReadChar();
        virtual void WriteChar(char_t c);

    private:
        
        ref_of<Console>::type m_console;

        unsigned int m_codePage{0};
        unsigned int m_outputCodePage{0};

    };
}}}//qor::ui::win

#endif//QOR_PP_H_OS_WINDOWS_UI_SERCON