// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "console.h"
#include "src/qor/flyers/error/error.h"

#include "src/platform/os/windows/api_layer/kernel/kernel32.h"
#include <io.h>
#include <fcntl.h>
#include <iostream>

using namespace qor::win::api;

//Implement this trivial function so the linker will pull in this library to fulfil the ImplementsIConsole requirement. 
namespace qor { bool qor_pp_module_interface(QOR_WINCONSOLE) ImplementsIConsole() {return true;} }//qor

namespace qor { namespace ui{ namespace win {

	Console::Console() : m_redirected(false), m_allocated(false), m_outFile(nullptr), m_inFile(nullptr), m_errFile(nullptr)
	{
		m_inFile = m_helper.GetStdHandle(ConsoleHelper::hStdIn).Use();
		if (m_inFile == nullptr || m_inFile == Invalid_Handle_Value)
		{
			Reallocate();
		}

		m_outFile = m_helper.GetStdHandle(ConsoleHelper::hStdOut).Use();
		m_errFile = m_outFile;		
		m_redirected = ProbeIsRedirected();				
	}

	bool Console::ProbeIsRedirected()
	{
		bool handleProtection = false;
		auto fileType = 6;
		try
		{
			fileType = Kernel32::GetFileType(m_inFile);
		}
		catch(Serious& error)
		{
			error.Catch();
			std::cerr << error.what().Content() << '\n';
			handleProtection = true;
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			handleProtection = true;
		}
		if(handleProtection)
		{
			//Use handle protection trick to make the handle accessible
			//Microsoft are likely use hidden attributes on the handle to hide it from us as 'invalid'. These get reset
			//when we ourselves protect/unprotect the handle from being closed.
			HANDLE h = Kernel32::GetStdHandle(qor::platform::win::Std_Input_Handle);
			qor::platform::win::Handle stdinhandle(h);
			stdinhandle.SetProtectFromClose(false);
			stdinhandle.Drop();
			fileType = Kernel32::GetFileType(m_inFile);
		}

		if(fileType != FILE_TYPE_CHAR)
		{
			return true;
		}
		return false;
	}

	void Console::Setup()
	{
		auto hstdin = m_helper.GetStdHandle(ConsoleHelper::hStdIn);
		m_inFile = hstdin.Use();
		if (hstdin.IsNull() || hstdin.IsInvalid() /*|| (Kernel32::GetFileType(hstdin.Use()) != FILE_TYPE_CHAR)*/ )
		{
			m_helper.Free();
			m_helper.Alloc();
			m_allocated = true;
			ResetIn();
		}

		auto hstdout = m_helper.GetStdHandle(ConsoleHelper::hStdOut);
		m_outFile = hstdout.Use();
		if (hstdout.IsNull() || hstdout.IsInvalid() /*|| (Kernel32::GetFileType(hstdin.Use()) != FILE_TYPE_CHAR)*/)
		{
			if (!m_allocated)
			{
				m_helper.Free();
				m_helper.Alloc();
				ResetIn();
				ResetErr();
			}
			ResetOut();
		}			

		auto fileType = Kernel32::GetFileType(m_inFile);

		if ((m_inFile != nullptr) && (m_inFile != Invalid_Handle_Value) && fileType == FILE_TYPE_CHAR )
		{
			DWORD mode = 0;
			Kernel32::GetConsoleMode(m_inFile, &mode);
			mode &= ~(ENABLE_LINE_INPUT);
			Kernel32::SetConsoleMode(m_inFile, mode);			
		}
		if(fileType != FILE_TYPE_CHAR)
		{
			m_redirected = true;
		}
		Kernel32::SetConsoleOutputCP(65001);
	}

	Console::~Console()
	{
		if (m_allocated)
		{
			m_helper.Free();
		}
	}

	bool Console::IsRedirected()
	{
		return m_redirected;
	}

	void Console::SetOut(int fileHandle)
	{
		if (m_outFile != nullptr)
		{
			fclose(reinterpret_cast<FILE*>(m_outFile));
			m_outFile = nullptr;
		}
		m_outFile = _fdopen(fileHandle, "r+");
	}

	void Console::SetIn(int fileHandle)
	{
		if (m_inFile != nullptr)
		{
			fclose(reinterpret_cast<FILE*>(m_inFile));
		}
		m_inFile = _fdopen(fileHandle, "r+");
		m_redirected = true;
	}

	void Console::SetErr(int fileHandle)
	{
		if (m_errFile != nullptr)
		{
			fclose(reinterpret_cast<FILE*>(m_errFile));
		}
		m_errFile = _fdopen(fileHandle, "r+");
	}

	void Console::ResetOut()
	{
		//close any existing output stream
		if (!m_redirected && m_outFile != nullptr)
		{
			fclose(reinterpret_cast<FILE*>(m_outFile));
			m_outFile = nullptr;
		}
		//open default windows stdout handle
		m_outFile = _fsopen("CONOUT$", "w+", _SH_DENYNO);
	}

	void Console::ResetIn()
	{
		//close any existing input stream
		if (!m_redirected && m_inFile != nullptr)
		{
			fclose(reinterpret_cast<FILE*>(m_inFile));
			m_inFile = nullptr;
		}
		//open default windows stdin handle
		m_inFile = _fsopen("CONIN$", "r+", _SH_DENYNO);
	}

	void Console::ResetErr()
	{
		//close any existing error stream
		if (!m_redirected && m_errFile != nullptr)
		{
			fclose(reinterpret_cast<FILE*>(m_errFile));
			m_errFile = nullptr;
		}
		m_errFile = _fsopen("CONOUT$", "w+", _SH_DENYNO);
	}

	void Console::Reallocate()
	{
		HANDLE h = Kernel32::GetStdHandle(qor::platform::win::Std_Input_Handle);
		qor::platform::win::Handle stdinhandle(h);
		stdinhandle.SetProtectFromClose(false);
		stdinhandle.Drop();

		h = Kernel32::GetStdHandle(qor::platform::win::Std_Output_Handle);
		qor::platform::win::Handle stdoutHandle(h);
		stdoutHandle.SetProtectFromClose(false);
		stdoutHandle.Drop();

		bool free = m_helper.Free();
		bool alloc = m_helper.Alloc(); 
		m_allocated = true;

		BindCrtHandlesToStdHandles(true, true, true);
		m_redirected = false;						
	}

	// Source - https://stackoverflow.com/a/25927081
	// Posted by Roger Sanders, modified by community. See post 'Timeline' for change history
	// Retrieved 2026-07-14, License - CC BY-SA 3.0
	void Console::BindCrtHandlesToStdHandles(bool bindStdIn, bool bindStdOut, bool bindStdErr)
	{		
		if (bindStdIn)
		{
			FILE* dummyFile;
			freopen_s(&dummyFile, "CONIN$", "r+", stdin);
		}
		if (bindStdOut)
		{
			FILE* dummyFile;
			freopen_s(&dummyFile, "CONOUT$", "w+", stdout);
		}
		if (bindStdErr)
		{
			FILE* dummyFile;
			freopen_s(&dummyFile, "CONOUT$", "w+", stderr);
		}

		// Redirect unbuffered stdin from the current standard input handle
		if (bindStdIn)
		{
			m_inFile = Kernel32::CreateFileW(L"CONIN$", GENERIC_READ | GENERIC_WRITE,  FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			//platform::win::Handle stdInHandle(m_inFile);
			//m_helper.SetStdHandle(ConsoleHelper::hStdIn, stdInHandle);
			//stdInHandle.Drop();
		}

		// Redirect unbuffered stdout to the current standard output handle
		if (bindStdOut)
		{
			m_outFile = Kernel32::CreateFileW(L"CONOUT$", GENERIC_READ | GENERIC_WRITE,  FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			//platform::win::Handle stdOutHandle(m_outFile);
			//m_helper.SetStdHandle(ConsoleHelper::hStdOut, stdOutHandle);
			//stdOutHandle.Drop();
		}

		// Redirect unbuffered stderr to the current standard error handle
		if (bindStdErr)
		{
			m_errFile = m_outFile;
			//platform::win::Handle stdErrHandle(m_errFile);
			//m_helper.SetStdHandle(ConsoleHelper::hStdErr, stdErrHandle);
			//stdErrHandle.Drop();
		}

		// Clear the error state for each of the C++ standard stream objects. We need to do this, as attempts to access the
		// standard streams before they refer to a valid target will cause the iostream objects to enter an error state. In
		// versions of Visual Studio after 2005, this seems to always occur during startup regardless of whether anything
		// has been read from or written to the targets or not.
		if (bindStdIn)
		{
			std::wcin.clear();
			std::cin.clear();
		}
		if (bindStdOut)
		{
			std::wcout.clear();
			std::cout.clear();
			std::wclog.clear();
			std::clog.clear();
		}
		if (bindStdErr)
		{
			std::wcerr.clear();
			std::cerr.clear();
		}
	}

	string_t Console::ReadLine()
	{
		string_t str;
		DWORD charsRead = 0;
		bool result = false;
		str.reserve(1024);
		str.resize(1024);
		if (!m_redirected)
		{
			DWORD mode = 0;
			auto hstdin = m_helper.GetStdHandle(ConsoleHelper::hStdIn);
			Kernel32::GetConsoleMode(hstdin.Use(), &mode);
			CONSOLE_READCONSOLE_CONTROL readControl;
			readControl.nLength = sizeof(CONSOLE_READCONSOLE_CONTROL);
			readControl.nInitialChars = 0;
			readControl.dwCtrlWakeupMask = 0x0D;
			readControl.dwControlKeyState = 0;
			do
			{
				DWORD newCharsRead = 0;				
				result = Kernel32::ReadConsole(m_inFile, str.data() + charsRead, 1023, &newCharsRead, &readControl);
				if (result && newCharsRead > 0)
				{
					charsRead += newCharsRead;
					if (mode & ENABLE_ECHO_INPUT)
					{	//echo is broken with line input turned off so we have to do it ourselves
						WriteChar(str.data()[charsRead - 1]);
					}
					if (charsRead >= 1023 || str.data()[charsRead - 1] == '\r')
					{
						str.resize(charsRead-1);
						return str;
					}
				}
			} while (result);
			return str;
		}
		else
		{
			result = Kernel32::ReadFile(m_inFile, str.data(), 1023, &charsRead, nullptr);
			str.resize(charsRead);
			return str;			
		}
		return string_t();
	}

	char_t Console::ReadChar()//get next key press including key combo stuff - echo by default except it doesn't
	{
		bool result = false;
		char_t c{0};
		DWORD charsRead = 0;
		if (!m_redirected)
		{						
			CONSOLE_READCONSOLE_CONTROL readControl;
			readControl.nLength = sizeof(CONSOLE_READCONSOLE_CONTROL);
			readControl.nInitialChars = 0;
			readControl.dwCtrlWakeupMask = 0x0D;
			readControl.dwControlKeyState = 0;
			result = Kernel32::ReadConsole(m_inFile, &c, 1, &charsRead, &readControl);
		}
		else
		{
			result = Kernel32::ReadFile(m_inFile, &c, 1, &charsRead, nullptr);
			if (result && charsRead == 1)
			{
				//Assume this redirected console is in line mode and we can't really just read a single char.
				//Dispose of the rest of the line for now although we should really buffer it locally.
				auto waste = ReadLine();
			}
		}
		if (result && charsRead == 1)
		{			
			return c;
		}
		return 0;
	}

	void Console::WriteChar(char_t c)
	{
		DWORD charsWritten = 0;		
		bool result = m_redirected ?
			result = Kernel32::WriteFile(m_outFile, &c, 1, &charsWritten, nullptr) :
			result = Kernel32::WriteConsole(m_outFile, &c, 1, &charsWritten, nullptr);

		if (!(result && charsWritten == 1))
		{
			continuable("Failed to write character to {} Console.", m_redirected ? "redirected" : "local");
		}
	}

	void Console::WriteLine(const string_t& output)
	{
		DWORD charsWritten = 0;
		bool result = m_redirected ? 
			Kernel32::WriteFile(m_outFile, output.data(), static_cast<DWORD>(output.size()), &charsWritten, nullptr) :
			Kernel32::WriteConsole(m_outFile, output.data(), static_cast<DWORD>(output.size()), &charsWritten, nullptr);
		
		if (!(result && charsWritten == output.size()))
		{
			continuable("Failed to write line to {} Console.", m_redirected ? "redirected" : "local");
		}
		else
		{
			WriteChar(L'\n');
		}
	}

	unsigned long Console::WriteBytes(char* data, size_t byteCount)
	{
		unsigned long bytesWritten = 0;
		bool result = m_redirected ?
			Kernel32::WriteFile(m_outFile, data, static_cast<DWORD>(byteCount), &bytesWritten, nullptr) :
			Kernel32::WriteConsoleA(m_outFile, data, static_cast<DWORD>(byteCount), &bytesWritten, nullptr);
		return bytesWritten;
	}

	unsigned long Console::GetDisplayMode()
	{
		unsigned long mode = 0;
		if(!m_helper.GetDisplayMode(mode))
		{
			continuable("Failed to get Console display mode.");
		}
		return mode;
	}

	void* Console::OutputFile()
	{
		return m_outFile;
	}

	void* Console::InputFile()
	{
		return m_inFile;
	}

	ref_of<ConsoleScreenBuffer>::type Console::CreateScreenBuffer(unsigned long desiredAccess, unsigned long shareMode, const qor::platform::win::SecurityAttributes* securityAttributes, bool textMode)
	{
		return new_ref<ConsoleScreenBuffer>(m_helper.CreateScreenBuffer(desiredAccess, shareMode, securityAttributes, textMode), true);
	}

	ref_of<PseudoConsole>::type Console::CreatePseudo(qor::platform::win::Coord size, unsigned long flags)
	{
		return new_ref<PseudoConsole>(m_helper.CreatePseudo(size, m_inFile, m_outFile, flags));
	}

	ref_of<ConsoleScreenBuffer>::type Console::GetActiveScreenBuffer()
	{
		return new_ref<ConsoleScreenBuffer>(m_outFile);
	}
		
	bool Console::SetActiveScreenBuffer(ref_of<ConsoleScreenBuffer>::type screenBuffer)
	{
		return m_helper.SetActiveScreenBuffer(screenBuffer->Handle());
		m_outFile = screenBuffer->Handle();
	}

	ref_of<ConsoleInput>::type Console::GetInput()
	{
		return new_ref<ConsoleInput>(m_inFile);
	}

	bool Console::GenerateCtrlCEvent()
	{
		return m_helper.GenerateCtrlEvent(0, 0);
	}

	bool Console::GenerateCtrlBreakEvent(unsigned long processGroupId)
	{
		return m_helper.GenerateCtrlEvent(1, processGroupId);
	}

	bool Console::SetCtrlHandler(CtrlHandlerCallback handlerRoutine, bool add)
	{
		return m_helper.SetCtrlHandler(handlerRoutine, add);
	}

	bool Console::AddAlias(std::string& source, std::string& target, std::string& exeName)
	{
		return m_helper.AddAlias(source, target, exeName);
	}

	unsigned long Console::GetAlias(std::string& source, std::string& target, std::string& exeName)
	{
		return m_helper.GetAlias(source, target, exeName);
	}

	unsigned long Console::GetAliases(std::string& aliases, std::string& exeName)
	{
		return m_helper.GetAliases(aliases, exeName);
	}

	unsigned long Console::GetAliasesLength(std::string& exeName)
	{
		return m_helper.GetAliasesLength(exeName);
	}

	unsigned long Console::GetAliasExes(std::string& nameBuffer)
	{
		return m_helper.GetAliasExes(nameBuffer);
	}

	unsigned long Console::GetAliasExesLength()
	{
		return m_helper.GetAliasExesLength();
	}

	qor::platform::win::ConsoleHistoryInfo Console::GetHistoryInfo()
	{
		qor::platform::win::ConsoleHistoryInfo consoleHistoryInfo;
		if (!m_helper.GetHistoryInfo(consoleHistoryInfo))
		{
			continuable("Failed to get Console history info.");
		}
		return consoleHistoryInfo;
	}

	bool Console::SetHistoryInfo(platform::win::ConsoleHistoryInfo* consoleHistoryInfo)
	{
		return m_helper.SetHistoryInfo(consoleHistoryInfo);
	}

	std::string Console::GetOriginalTitle()
	{
		std::string consoleTitle;
		consoleTitle.reserve(1024);
		unsigned long result = m_helper.GetOriginalTitle(consoleTitle);
		consoleTitle.resize(result);
		return consoleTitle;
	}

	std::string Console::GetTitle()
	{
		std::string consoleTitle;
		consoleTitle.reserve(1024);
		unsigned long result = m_helper.GetTitle(consoleTitle);
		consoleTitle.resize(result);
		return consoleTitle;
	}

	bool Console::SetTitle(const std::string& consoleTitle)
	{
		return m_helper.SetTitle(consoleTitle);
	}

	std::vector< unsigned long > Console::GetProcessList()
	{
		std::vector< unsigned long > processList;
		unsigned long result = m_helper.GetProcessList(processList);
		processList.resize(result);
		return processList;
	}

	void* Console::GetWindow()
	{
		return m_helper.GetWindow();
	}

    platform::win::ConsoleSelectionInfo Console::GetSelectionInfo()
    {
        platform::win::ConsoleSelectionInfo consoleSelectionInfo;
        if (!m_helper.GetSelectionInfo(consoleSelectionInfo))
        {
            continuable("Failed to get Console selection info.");
        }
        return consoleSelectionInfo;
    }

}}}//qor::ui::win
