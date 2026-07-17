// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "consolehelper.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "src/platform/os/windows/api_layer/kernel/kernel32.h" //kernel32.h must be the first windows header as it's the primary inclusion point for windows.h
#include "src/platform/os/windows/common/stringconv.h"
#include "src/platform/os/windows/common/constants.h"

using namespace qor::win::api;
using namespace qor::platform::win;

namespace qor { namespace ui { namespace win {

	bool ConsoleHelper::AddAlias(std::string& source, std::string& target, std::string& exeName)
	{		
		return Kernel32::AddConsoleAlias(to_tstring(source.c_str()).data(), to_tstring(target.c_str()).data(), to_tstring(exeName.c_str()).data());		
	}

	bool ConsoleHelper::Alloc()
	{		
		return Kernel32::AllocConsole();		
	}

	ConsoleHelper::AllocConsoleResult ConsoleHelper::AllocWithOptions(ConsoleHelper::AllocConsoleMode mode, bool useShowWindow, unsigned short showWindowCommand)
	{
		ALLOC_CONSOLE_OPTIONS options
		{
    		static_cast<ALLOC_CONSOLE_MODE>(mode),
    		useShowWindow ? TRUE : FALSE,
    		showWindowCommand
		};
		ALLOC_CONSOLE_RESULT result;
		Kernel32::AllocConsoleWithOptions(&options, &result);
		return static_cast<AllocConsoleResult>(result);
	}

	bool ConsoleHelper::Attach(unsigned long processId)
	{		
		return Kernel32::AttachConsole(static_cast<DWORD>(processId));
	}

	void ConsoleHelper::ClosePseudo(void* hPC)
	{
		return Kernel32::ClosePseudoConsole((HPCON)(hPC));
	}

	ref_of<ConsoleScreenBuffer>::type ConsoleHelper::CreateScreenBuffer(unsigned long desiredAccess, unsigned long shareMode, const SecurityAttributes* securityAttributes, bool textMode)
	{
		void* handle = Kernel32::CreateConsoleScreenBuffer(desiredAccess, shareMode, (const ::SECURITY_ATTRIBUTES*)securityAttributes, textMode ? CONSOLE_TEXTMODE_BUFFER : 0, nullptr);
		return new_ref<ConsoleScreenBuffer>(handle);
	}

	ref_of<PseudoConsole>::type ConsoleHelper::CreatePseudo(qor::platform::win::Coord size, void* hInput, void* hOutput, unsigned long flags)
	{
		HPCON hPC = 0;
		long result = Kernel32::CreatePseudoConsole(*(reinterpret_cast<COORD*>(&size)), hInput, hOutput, flags, &hPC);
		return new_ref<PseudoConsole>(hPC);
	}

	bool ConsoleHelper::FillOutputAttribute(void* hConsoleOutput, unsigned short attribute, unsigned long length, qor::platform::win::Coord writeCoord, unsigned long& numberOfAttrsWritten)
	{
		return Kernel32::FillConsoleOutputAttribute(hConsoleOutput, attribute, length, *(reinterpret_cast<COORD*>(&writeCoord)), &numberOfAttrsWritten) ? true : false;
	}

	bool ConsoleHelper::FillOutputCharacter(void* hConsoleOutput, char_t character, unsigned long length, qor::platform::win::Coord writeCoord, unsigned long& numberOfCharsWritten)
	{
		return Kernel32::FillConsoleOutputCharacter(hConsoleOutput, character, length, *(reinterpret_cast<COORD*>(&writeCoord)), &numberOfCharsWritten) ? true : false;
	}

	bool ConsoleHelper::FlushInputBuffer(void* hConsoleInput)
	{
		return Kernel32::FlushConsoleInputBuffer(hConsoleInput) ? true : false;
	}


	bool ConsoleHelper::Free(void)
	{
		return Kernel32::FreeConsole();
	}

	bool ConsoleHelper::GenerateCtrlEvent(unsigned long ctrlEvent, unsigned long processGroupId)
	{
		return Kernel32::GenerateConsoleCtrlEvent(static_cast<DWORD>(ctrlEvent), static_cast<DWORD>(processGroupId));
	}

	unsigned long ConsoleHelper::GetAlias(std::string& source, std::string& target, std::string& exeName)
	{
		return Kernel32::GetConsoleAlias(to_tstring(source.c_str()).data(), to_tstring(target.c_str()).data(), static_cast<DWORD>(target.size()), to_tstring(exeName.c_str()).data());
	}

	unsigned long ConsoleHelper::GetAliases(std::string& aliases, std::string& exeName)
	{
		return Kernel32::GetConsoleAliases(to_tstring(aliases.c_str()).data(), static_cast<DWORD>(aliases.size()), to_tstring(exeName.c_str()).data());
	}

	unsigned long ConsoleHelper::GetAliasesLength(std::string& exeName)
	{
		return Kernel32::GetConsoleAliasesLength(to_tstring(exeName.c_str()).data());
	}

	unsigned long ConsoleHelper::GetAliasExes(std::string& nameBuffer)
	{
		return Kernel32::GetConsoleAliasExes(to_tstring(nameBuffer.c_str()).data(), static_cast<DWORD>(nameBuffer.size()));
	}

	unsigned long ConsoleHelper::GetAliasExesLength()
	{
		return Kernel32::GetConsoleAliasExesLength();
	}

	Mib ConsoleHelper::GetCP(void)
	{
		unsigned int windowsCP = Kernel32::GetConsoleCP();
		//TODO: Translate Windows Code Page to QOR CodePage
		//There is info here to be looked up - Computer\HKEY_CLASSES_ROOT\MIME\Database\Charset\iso-8859-1
		//but it's messy.
		return Mib::ASCII;
	}

	bool ConsoleHelper::GetDisplayMode(unsigned long& modeFlags)
	{
		return Kernel32::GetConsoleDisplayMode(&modeFlags);
	}

	bool ConsoleHelper::GetHistoryInfo(qor::platform::win::ConsoleHistoryInfo& consoleHistoryInfo)
	{
		return Kernel32::GetConsoleHistoryInfo((PCONSOLE_HISTORY_INFO)(&consoleHistoryInfo));
	}

	bool ConsoleHelper::GetMode(void* hConsoleHandle, unsigned long& mode)
	{
		return Kernel32::GetConsoleMode(hConsoleHandle, &mode) ? true : false;
	}

	unsigned long ConsoleHelper::GetOriginalTitle(std::string& consoleTitle)
	{
		return Kernel32::GetConsoleOriginalTitle(to_tstring(consoleTitle.c_str()).data(), static_cast<DWORD>(consoleTitle.size()));
	}

	//CCodePage ConsoleHelper::GetOutputCP(void);

	//unsigned long ConsoleHelper::GetProcessList(std::vector< unsigned long >& processList);

	bool ConsoleHelper::GetScreenBufferInfo(void* hConsoleOutput, platform::win::ConsoleScreenBufferInfo* consoleScreenBufferInfo)
	{
		return Kernel32::GetConsoleScreenBufferInfo(hConsoleOutput, reinterpret_cast<PCONSOLE_SCREEN_BUFFER_INFO>(consoleScreenBufferInfo)) ? true : false;
	}

	bool ConsoleHelper::GetScreenBufferInfoEx(void* hConsoleOutput, platform::win::ConsoleScreenBufferInfoEx* consoleScreenBufferInfoEx)
	{
		return Kernel32::GetConsoleScreenBufferInfoEx(hConsoleOutput, reinterpret_cast<PCONSOLE_SCREEN_BUFFER_INFOEX>(consoleScreenBufferInfoEx)) ? true : false;
	}

	//bool ConsoleHelper::GetSelectionInfo(ConsoleSelectionInfo& consoleSelectionInfo);

	unsigned long ConsoleHelper::GetTitle(std::string& consoleTitle)
	{
		return Kernel32::GetConsoleTitle(to_tstring(consoleTitle.c_str()).data(), static_cast<DWORD>(consoleTitle.size()));
	}
	//COSWindow::ref_type ConsoleHelper::GetWindow();
	
	bool ConsoleHelper::GetCurrentFont(void* hConsoleOutput, bool maximumWindow, platform::win::ConsoleFontInfo* consoleCurrentFont)
	{
		return Kernel32::GetCurrentConsoleFont(hConsoleOutput, maximumWindow ? TRUE : FALSE, reinterpret_cast<::PCONSOLE_FONT_INFO>(consoleCurrentFont)) ? true : false;
	}

	bool GetCurrentFontEx(void* hConsoleOutput, bool maximumWindow, platform::win::ConsoleFontInfoEx* consoleCurrentFontEx)
	{
		return Kernel32::GetCurrentConsoleFontEx(hConsoleOutput, maximumWindow ? TRUE : FALSE, reinterpret_cast< PCONSOLE_FONT_INFOEX >(consoleCurrentFontEx)) ? true : false;
	}

	unsigned long ConsoleHelper::GetNumberOfMouseButtons()
	{
		unsigned long NumberOfMouseButtons = 0;
		bool result = Kernel32::GetNumberOfConsoleMouseButtons(&NumberOfMouseButtons);
		if (result)
		{
			return static_cast<unsigned long>(NumberOfMouseButtons);
		}
		else
		{
			continuable("Call to Kernel32::GetNumberOfConsoleMouseButtons failed.");
		}
		return 0;
	}

	Handle ConsoleHelper::GetStdHandle(StdHandles nStdHandle)
	{
		qor_pp_ofcontext;
		HANDLE h = nullptr;
		DWORD handle = (DWORD)(-1);
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
		catch (Serious& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return Handle();
	}

	bool ConsoleHelper::SetActiveScreenBuffer(void* hConsoleOutput)
	{
		return Kernel32::SetConsoleActiveScreenBuffer(hConsoleOutput) ? true : false;
	}

	//bool ConsoleHelper::SetCP(const CCodePage& codePage);
	bool ConsoleHelper::SetCtrlHandler(CtrlHandlerCallback handlerRoutine, bool add)
	{
		qor_pp_ofcontext;
		bool result = false;
		try
		{
			result = Kernel32::SetConsoleCtrlHandler(handlerRoutine, add);
		}
		catch (Serious& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}

	bool ConsoleHelper::SetMode(void* hConsoleHandle, unsigned long mode)
	{
		return Kernel32::SetConsoleMode(hConsoleHandle, mode) ? true : false;
	}

	//bool ConsoleHelper::SetHistoryInfo(const ConsoleHistoryInfo& consoleHistoryInfo);
	//bool ConsoleHelper::SetOutputCP(const CCodePage& codePage);

	bool ConsoleHelper::SetScreenBufferSize(void* hConsoleOutput, platform::win::Coord size)
	{
		return Kernel32::SetConsoleScreenBufferSize(hConsoleOutput, *(reinterpret_cast<::COORD*>(&size))) ? true : false;
	}

	bool ConsoleHelper::SetTitle(std::string& consoleTitle)
	{
		return Kernel32::SetConsoleTitle(to_tstring(consoleTitle.c_str()).c_str());
	}

	bool ConsoleHelper::SetWindowInfo(void* hConsoleOutput, bool absolute, const qor::platform::win::SmallRect* consoleWindow)
	{
		return Kernel32::SetConsoleWindowInfo(hConsoleOutput, absolute ? 1 : 0, reinterpret_cast<const SMALL_RECT*>(consoleWindow)) ? true : false;
	}

	bool ConsoleHelper::SetCurrentFontEx(void* hConsoleOutput, bool maximumWindow, platform::win::ConsoleFontInfoEx* consoleCurrentFontEx)
	{
		return Kernel32::SetCurrentConsoleFontEx(hConsoleOutput, maximumWindow ? TRUE : FALSE, reinterpret_cast<PCONSOLE_FONT_INFOEX>(consoleCurrentFontEx)) ? true : false;
	}

	bool ConsoleHelper::SetStdHandle(StdHandles stdHandle, Handle& handle)
	{
		qor_pp_ofcontext;
		bool result = false;
		try
		{
			DWORD h = (DWORD)(-1);
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
		catch (Serious& error)
		{
			error.Catch();
		}
		catch (...)
		{
			serious("Unhandled exception");
		}
		return result;
	}

	bool ConsoleHelper::WriteOutput(void* hConsoleOutput, const platform::win::CharInfo* buffer, platform::win::Coord bufferSize, platform::win::Coord bufferCoord, qor::platform::win::SmallRect* writeRegion)
	{
		return Kernel32::WriteConsoleOutput(hConsoleOutput, reinterpret_cast<const ::CHAR_INFO*>(buffer), *(reinterpret_cast<COORD*>(&bufferSize)), *(reinterpret_cast<COORD*>(&bufferCoord)), reinterpret_cast<::PSMALL_RECT>(writeRegion)) ? true : false;
	}

}}}//qor::ui::win
