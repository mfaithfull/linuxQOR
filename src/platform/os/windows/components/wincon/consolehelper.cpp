// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/interception/functioncontext.h"
#include "src/qor/error/error.h"
#include "consolehelper.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "src/platform/os/windows/winqapi/kernel/kernel32.h" //kernel32.h must be the first windows header as it's the primary inclusion point for windows.h
#include "src/platform/os/windows/common/constants.h"

using namespace qor::winapi;

namespace qor { namespace nsWindows {

	ConsoleHelper::ConsoleHelper()
	{

	}

	ConsoleHelper::~ConsoleHelper()
	{

	}

	bool ConsoleHelper::AddAlias(std::string& source, std::string& target, std::string& exeName)
	{
		qor_pp_ofcontext;
		bool result = false;
		try
		{
			result = Kernel32::AddConsoleAlias(source.data(), target.data(), exeName.data());
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}

	bool ConsoleHelper::Alloc(void)
	{
		qor_pp_ofcontext;
		bool result = false;
		try
		{
			result = Kernel32::AllocConsole();
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}

	bool ConsoleHelper::Attach(unsigned long processId)
	{
		qor_pp_ofcontext;
		bool result = false;
		try
		{
			result = Kernel32::AttachConsole(static_cast<DWORD>(processId));
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}

	bool ConsoleHelper::Free(void)
	{
		qor_pp_ofcontext;
		bool result = false;
		try
		{
			result = Kernel32::FreeConsole();
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}

	bool ConsoleHelper::GenerateCtrlEvent(unsigned long ctrlEvent, unsigned long processGroupId)
	{
		qor_pp_ofcontext;
		bool result = false;
		try
		{
			result = Kernel32::GenerateConsoleCtrlEvent(static_cast<DWORD>(ctrlEvent), static_cast<DWORD>(processGroupId));
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}

	unsigned long ConsoleHelper::GetAlias(std::string& source, std::string& target, std::string& exeName)
	{
		qor_pp_ofcontext;
		unsigned long result = 0;
		try
		{
			result = Kernel32::GetConsoleAlias(source.data(), target.data(), static_cast<DWORD>(target.size()), exeName.data());
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}

	unsigned long ConsoleHelper::GetAliases(std::string& aliases, std::string& exeName)
	{
		qor_pp_ofcontext;
		unsigned long result = 0;
		try
		{
			result = Kernel32::GetConsoleAliases(aliases.data(), static_cast<DWORD>(aliases.size()), exeName.data());
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}

	unsigned long ConsoleHelper::GetAliasesLength(std::string& exeName)
	{
		qor_pp_ofcontext;
		unsigned long result = 0;
		try
		{
			result = Kernel32::GetConsoleAliasesLength(exeName.data());
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}

	unsigned long ConsoleHelper::GetAliasExes(std::string& nameBuffer)
	{
		qor_pp_ofcontext;
		unsigned long result = 0;
		try
		{
			result = Kernel32::GetConsoleAliasExes(nameBuffer.data(), static_cast<DWORD>(nameBuffer.size()));
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}

	unsigned long ConsoleHelper::GetAliasExesLength()
	{
		qor_pp_ofcontext;
		unsigned long result = 0;
		try
		{
			result = Kernel32::GetConsoleAliasExesLength();
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}

	//CCodePage ConsoleHelper::GetCP(void);

	bool ConsoleHelper::GetDisplayMode(unsigned long& modeFlags)
	{
		qor_pp_ofcontext;
		bool result = false;
		try
		{
			result = Kernel32::GetConsoleDisplayMode(&modeFlags);
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}

	bool ConsoleHelper::GetHistoryInfo(ConsoleHistoryInfo& consoleHistoryInfo)
	{
		qor_pp_ofcontext;
		bool result = false;
		try
		{
			//TODO:
			//result = Kernel32::GetConsoleDisplayMode(&modeFlags);
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}

	unsigned long ConsoleHelper::GetOriginalTitle(std::string& consoleTitle)
	{
		qor_pp_ofcontext;
		unsigned long result = 0;
		try
		{
			result = Kernel32::GetConsoleOriginalTitle(consoleTitle.data(), static_cast<DWORD>(consoleTitle.size()));
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}

	//CCodePage ConsoleHelper::GetOutputCP(void);

	//unsigned long ConsoleHelper::GetProcessList(std::vector< unsigned long >& processList);

	//bool ConsoleHelper::GetSelectionInfo(ConsoleSelectionInfo& consoleSelectionInfo);

	unsigned long ConsoleHelper::GetTitle(std::string& consoleTitle)
	{
		qor_pp_ofcontext;
		unsigned long result = 0;
		try
		{
			result = Kernel32::GetConsoleTitle(consoleTitle.data(), static_cast<DWORD>(consoleTitle.size()));
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}
	//COSWindow::ref_type ConsoleHelper::GetWindow();
	
	unsigned long ConsoleHelper::GetNumberOfMouseButtons()
	{
		qor_pp_ofcontext;
		bool result = false;
		try
		{
			DWORD NumberOfMouseButtons = 0;
			result = Kernel32::GetNumberOfConsoleMouseButtons(&NumberOfMouseButtons);
			if (result)
			{
				return static_cast<unsigned long>(NumberOfMouseButtons);
			}
			else
			{
				continuable("Call to Kernel32::GetNumberOfConsoleMouseButtons failed.");
			}
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exeption");
		}
		return 0;
	}

	Handle ConsoleHelper::GetStdHandle(StdHandles nStdHandle)
	{
		qor_pp_ofcontext;
		HANDLE h = nullptr;
		DWORD handle = -1;
		try 
		{
			switch(nStdHandle)
			{
			case hStdIn:
				handle = Std_Input_Handle;
				break;
			case hStdOut:
				handle = Std_Output_Handle;
				break;
			case hStdErr:
				handle = Std_Error_Handle;
				break;
			}
			h = Kernel32::GetStdHandle(handle);
			Handle std(h);
			std.SetProtectFromClose(true);
			return std;
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return Handle();
	}
	//bool ConsoleHelper::SetCP(const CCodePage& codePage);
	bool ConsoleHelper::SetCtrlHandler(CtrlHandlerCallback handlerRoutine, bool add)
	{
		qor_pp_ofcontext;
		bool result;
		try
		{
			result = Kernel32::SetConsoleCtrlHandler(handlerRoutine, add);
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}
	//bool ConsoleHelper::SetHistoryInfo(const ConsoleHistoryInfo& consoleHistoryInfo);
	//bool ConsoleHelper::SetOutputCP(const CCodePage& codePage);
	bool ConsoleHelper::SetTitle(std::string& consoleTitle)
	{
		qor_pp_ofcontext;
		bool result = false;
		try
		{
			result = Kernel32::SetConsoleTitle(consoleTitle.c_str());
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}

	bool ConsoleHelper::SetStdHandle(StdHandles stdHandle, Handle& handle)
	{
		qor_pp_ofcontext;
		bool result = false;
		try
		{
			DWORD h = -1;
			switch (stdHandle)
			{
			case hStdIn:
				h = Std_Input_Handle;
				break;
			case hStdOut:
				h = Std_Output_Handle;
				break;
			case hStdErr:
				h = Std_Error_Handle;
				break;
			}
			result = Kernel32::SetStdHandle(h, handle.Use());
		}
		catch (const Error& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}

}}//qor::winapi
