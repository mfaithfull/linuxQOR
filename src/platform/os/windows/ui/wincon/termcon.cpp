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

namespace qor { namespace ui{ namespace win {

    TermCon::TermCon()
    {
        m_console = new_ref<Console>();
        bool success = false;// will be set to true once we're talking to a terminal

        if(m_console->IsRedirected())
        {
            std::cout << "Console redirection detected." << std::endl;
            success = QueryVT();
            if(!success)
            {
                std::cout << "Reallocating a default console." << std::endl;
                m_console->Reallocate();
                success = SetupLocalTerminal();
            }
        }
        else
        {
            success = SetupLocalTerminal();
        }
        m_validTerminal = success;
    }

    TermCon::~TermCon() = default;

    bool TermCon::IsValid()
    {
        return m_validTerminal;
    }

    bool TermCon::SetupLocalTerminal()
    {
        std::cout << "Determining local Console capabilities." << std::endl;
        bool success = false;
        //We have a local 'terminal' of some sort.
        unsigned long mode = m_console->GetActiveScreenBuffer()->GetMode();
        if(mode & static_cast<unsigned long>(ConsoleOutputModeFlags::VirtualTerminalProcessing))
        {
            //Looks like we're talking to a real terminal, all is well
            std::cout << "Console is a terminal." << std::endl;
            success = true;
        }
        else
        {
            mode |= static_cast<unsigned long>(ConsoleOutputModeFlags::VirtualTerminalProcessing);
            mode |= static_cast<unsigned long>(ConsoleOutputModeFlags::DisableNewlineAutoReturn);
            if(m_console->GetActiveScreenBuffer()->SetMode(mode))
            {
                mode = m_console->GetActiveScreenBuffer()->GetMode();
                if(mode & static_cast<unsigned long>(ConsoleOutputModeFlags::VirtualTerminalProcessing))
                {
                    //We're talking to a legacy console, now in Terminal Mode
                    std::cout << "Legacy console in terminal mode." << std::endl;
                    success = true;
                }
            }                
            else//Possible ghost redirection, i.e. undetectable redirection
            {
                std::cout << "Unknown device." << std::endl;
                success = QueryVT();
            }
        }
        return success;
    }

    bool TermCon::QueryVT()
    {
        std::cout << "Sending terminal query string..." << std::endl;
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

    void TermCon::NoCurrentConsole()
    {
        m_helper.Free();
        m_helper.Alloc();
        RebindCrtHandlesToStdHandles();
    }

	void TermCon::RebindCrtHandlesToStdHandles()
	{		
        {
		    FILE* dummyFile;
			freopen_s(&dummyFile, "CONIN$", "r+", stdin);
            platform::win::Handle hStdIn(Kernel32::CreateFileW(L"CONIN$", GENERIC_READ | GENERIC_WRITE,  FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL));
            m_helper.SetStdHandle(ConsoleHelper::hStdIn,hStdIn);
            hStdIn.Drop();
		}
		
		{
			FILE* dummyFile;
			freopen_s(&dummyFile, "CONOUT$", "w+", stdout);
            platform::win::Handle hStdOut(Kernel32::CreateFileW(L"CONOUT$", GENERIC_READ | GENERIC_WRITE,  FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL));
            m_helper.SetStdHandle(ConsoleHelper::hStdOut,hStdOut);
            hStdOut.Drop();
		}
		
		{
			FILE* dummyFile;
			freopen_s(&dummyFile, "CONOUT$", "w+", stderr);
		}

		std::wcin.clear();
		std::cin.clear();
		std::wcout.clear();
		std::cout.clear();
		std::wclog.clear();
		std::clog.clear();
		std::wcerr.clear();
		std::cerr.clear();

	}

}}}//qor::ui::win