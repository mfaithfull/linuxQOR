// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "termcon.h"
#include "src/qor/flyers/error/error.h"
#include "src/platform/os/windows/common/constants.h"

#include <io.h>
#include <fcntl.h>
#include <iostream>
#include "src/platform/os/windows/api_layer/kernel/kernel32.h"

using namespace qor::win::api;

qor_pp_module_provide(QOR_WINTERMINAL, Terminal)

namespace qor { namespace ui{ namespace win {

    Terminal::Terminal()
    {
        m_console = new_ref<Console>();
    }

    Terminal::~Terminal() noexcept = default;

    bool Terminal::IsValid()
    {
        return m_validTerminal;
    }

    void Terminal::Setup()
    {
        if(m_console->IsRedirected())
        {
            std::cout << "Console redirection detected." << std::endl;
            if(!QueryVT())
            {
                std::cout << "Reallocating a default terminal." << std::endl;
                m_console->Reallocate();
            }
        }

        auto output = m_console->GetActiveScreenBuffer();
        auto input = m_console->GetInput();

        m_outputCodePage = output->GetCodePage();
        m_codePage = input->GetCodePage();
        output->SetCodePage(CP_UTF8);
        input->SetCodePage(CP_UTF8);

        // Enable VT processing on stdout and stdin

        m_originalOutputMode = output->GetMode();
        unsigned long out_mode = m_originalOutputMode;
        out_mode |= static_cast<unsigned long>(ConsoleOutputModeFlags::VirtualTerminalProcessing);
        out_mode |= static_cast<unsigned long>(ConsoleOutputModeFlags::DisableNewlineAutoReturn);
        output->SetMode(out_mode);

        m_originalInputMode = input->GetMode();
        unsigned long in_mode = m_originalInputMode;
        in_mode |= static_cast<unsigned long>(ConsoleInputModeFlags::VertualTerminalInput);
        input->SetMode(in_mode);

        m_validTerminal = true;
    }

    void Terminal::Shutdown()
    {
        auto output = m_console->GetActiveScreenBuffer();
        auto input = m_console->GetInput();

        //Restore Console settings
        if(output.IsNotNull())
        {
            output->SetCodePage(m_outputCodePage);
            output->SetMode(m_originalOutputMode);
        }

        if(input.IsNotNull())
        {
            input->SetCodePage(m_codePage);
            input->SetMode(m_originalInputMode);
        }
    }

    bool Terminal::QueryVT()
    {        
        bool result = false;
        std::string reportCursorPosition = "\x1B[6n";
        m_console->WriteBytes(reportCursorPosition.data(), reportCursorPosition.size());
        std::string response;
        response.reserve(15);
        qor::platform::win::InputRecord input;
        unsigned long eventsRead = 0;
        unsigned long eventsTotal = 0;

        eventsRead = 0;
        m_console->GetInput()->Read(&input, 1, eventsRead, nullptr);
        if(input.EventType == KEY_EVENT)
        {
            response.append(eventsRead, input.Event.KeyEvent.uChar.AsciiChar);
        }
        eventsTotal += eventsRead;

        if(eventsTotal == 0)
        {
            std::cout << "...No response" << std::endl;
        }        
        response.resize(eventsTotal);
        if(response.find("\x1B[") != std::string::npos)
        {
            std::cout << "Response indicates a terminal." << std::endl;
            result = true;
        }
        return result;
    }

    Dimensions Terminal::Size()
    {
        platform::win::ConsoleScreenBufferInfo csbi;

        if(m_console->GetActiveScreenBuffer()->GetInfo(&csbi))        
        {
            return Dimensions{csbi.srWindow.right - csbi.srWindow.left + 1,
            csbi.srWindow.bottom - csbi.srWindow.top + 1};
        }        
        return Dimensions{80,24};
    }

    Terminal::Colour Terminal::ComputeColourSupport()
    {
        return Terminal::Colour::TrueColour;
    }

}}}//qor::ui::win