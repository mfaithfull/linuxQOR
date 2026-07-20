// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "sercon.h"
#include "src/qor/flyers/error/error.h"
#include "src/platform/os/windows/common/constants.h"

#include <io.h>
#include <fcntl.h>
#include <iostream>
#include "src/platform/os/windows/api_layer/kernel/kernel32.h"

using namespace qor::win::api;

qor_pp_module_provide(QOR_WINSERCON, SerialConsole)

namespace qor { namespace ui{ namespace win {

    SerCon::SerCon()
    {
        m_console = new_ref<Console>();
    }

    SerCon::~SerCon() noexcept = default;

    void SerCon::Setup()
    {
        auto output = m_console->GetActiveScreenBuffer();
        auto input = m_console->GetInput();

        m_outputCodePage = output->GetCodePage();
        m_codePage = input->GetCodePage();
        output->SetCodePage(CP_UTF8);
        input->SetCodePage(CP_UTF8);
    }

    void SerCon::Shutdown()
    {
        auto output = m_console->GetActiveScreenBuffer();
        auto input = m_console->GetInput();

        //Restore Console settings
        if(output.IsNotNull())
        {
            output->SetCodePage(m_outputCodePage);
        }

        if(input.IsNotNull())
        {
            input->SetCodePage(m_codePage);
        }
    }

    string_t SerCon::ReadLine()
    {
        return m_console->ReadLine();
    }

    void SerCon::WriteLine(const string_t& line)
    {
        std::wcout << line << std::endl;
        //m_console->WriteLine(line);
    }

    char_t SerCon::ReadChar()
    {
        return m_console->ReadChar();
    }

    void SerCon::WriteChar(char_t c)
    {
        m_console->WriteChar(c);
    }


}}}//qor::ui::win