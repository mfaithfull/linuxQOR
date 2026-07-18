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
            success = QueryVT();
            if(!success)
            {
                m_console->Reallocate();
                success = SetupLocalTerminal();
            }
        }
        else
        {
            success = SetupLocalTerminal();
        }
    }

    TermCon::~TermCon() = default;

    bool TermCon::SetupLocalTerminal()
    {
        bool success = false;
        //We have a local 'terminal' of some sort.
        unsigned long mode = m_console->GetActiveScreenBuffer()->GetMode();
        if(mode & static_cast<unsigned long>(ConsoleOutputModeFlags::VirtualTerminalProcessing))
        {
            //Looks like we're talking to a real terminal, all is well
            success = true;
        }
        else
        {
            mode |= static_cast<unsigned long>(ConsoleOutputModeFlags::VirtualTerminalProcessing);
            if(m_console->GetActiveScreenBuffer()->SetMode(mode))
            {
                mode = m_console->GetActiveScreenBuffer()->GetMode();
                if(mode & static_cast<unsigned long>(ConsoleOutputModeFlags::VirtualTerminalProcessing))
                {
                    //We're talking to a legacy console, now in Terminal Mode
                    success = true;
                }
            }                
            else//Possible ghost redirection, i.e. undetectable redirection
            {
                success = QueryVT();
            }
        }
        return success;
    }

    bool TermCon::QueryVT()
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
        
        response.resize(eventsTotal);
        if(response.find("\x1B[") != std::string::npos)
        {
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
			freopen_s(&dummyFile, "CONIN$", "r", stdin);
		}
		
		{
			FILE* dummyFile;
			freopen_s(&dummyFile, "CONOUT$", "w", stdout);
		}
		
		{
			FILE* dummyFile;
			freopen_s(&dummyFile, "CONOUT$", "w", stderr);
		}

		//m_inFile = Kernel32::CreateFileW(L"CONIN$", GENERIC_READ | GENERIC_WRITE,  FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		//m_outFile = Kernel32::CreateFileW(L"CONOUT$", GENERIC_READ | GENERIC_WRITE,  FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		//m_errFile = m_outFile;

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