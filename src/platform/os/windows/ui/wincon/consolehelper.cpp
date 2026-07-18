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

	bool ConsoleHelper::AddAlias(std::string& source, std::string& target, std::string& exeName) const
	{		
		return Kernel32::AddConsoleAlias(to_tstring(source.c_str()).data(), to_tstring(target.c_str()).data(), to_tstring(exeName.c_str()).data());		
	}

	bool ConsoleHelper::Alloc() const
	{		
		return Kernel32::AllocConsole();		
	}

	ConsoleHelper::AllocConsoleResult ConsoleHelper::AllocWithOptions(ConsoleHelper::AllocConsoleMode mode, bool useShowWindow, unsigned short showWindowCommand) const
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

	bool ConsoleHelper::Attach(unsigned long processId) const
	{		
		return Kernel32::AttachConsole(static_cast<DWORD>(processId));
	}

	void ConsoleHelper::ClosePseudo(void* hPC) const
	{
		return Kernel32::ClosePseudoConsole((HPCON)(hPC));
	}

	void* ConsoleHelper::CreateScreenBuffer(unsigned long desiredAccess, unsigned long shareMode, const SecurityAttributes* securityAttributes, bool textMode) const
	{
		return Kernel32::CreateConsoleScreenBuffer(desiredAccess, shareMode, (const ::SECURITY_ATTRIBUTES*)securityAttributes, textMode ? CONSOLE_TEXTMODE_BUFFER : 0, nullptr);		
	}

	void* ConsoleHelper::CreatePseudo(qor::platform::win::Coord size, void* hInput, void* hOutput, unsigned long flags) const
	{	
		void* hPC;	
		Kernel32::CreatePseudoConsole(*(reinterpret_cast<COORD*>(&size)), hInput, hOutput, flags, &hPC);
		return hPC;
	}

	bool ConsoleHelper::FillOutputAttribute(void* hConsoleOutput, unsigned short attribute, unsigned long length, qor::platform::win::Coord writeCoord, unsigned long& numberOfAttrsWritten) const
	{
		return Kernel32::FillConsoleOutputAttribute(hConsoleOutput, attribute, length, *(reinterpret_cast<COORD*>(&writeCoord)), &numberOfAttrsWritten) ? true : false;
	}

	bool ConsoleHelper::FillOutputCharacter(void* hConsoleOutput, char_t character, unsigned long length, qor::platform::win::Coord writeCoord, unsigned long& numberOfCharsWritten) const
	{
		return Kernel32::FillConsoleOutputCharacter(hConsoleOutput, character, length, *(reinterpret_cast<COORD*>(&writeCoord)), &numberOfCharsWritten) ? true : false;
	}

	bool ConsoleHelper::FlushInputBuffer(void* hConsoleInput) const
	{
		return Kernel32::FlushConsoleInputBuffer(hConsoleInput) ? true : false;
	}


	bool ConsoleHelper::Free(void) const
	{
		return Kernel32::FreeConsole();
	}

	bool ConsoleHelper::GenerateCtrlEvent(unsigned long ctrlEvent, unsigned long processGroupId) const
	{
		return Kernel32::GenerateConsoleCtrlEvent(static_cast<DWORD>(ctrlEvent), static_cast<DWORD>(processGroupId));
	}

	unsigned long ConsoleHelper::GetAlias(std::string& source, std::string& target, std::string& exeName) const
	{
		return Kernel32::GetConsoleAlias(to_tstring(source.c_str()).data(), to_tstring(target.c_str()).data(), static_cast<DWORD>(target.size()), to_tstring(exeName.c_str()).data());
	}

	unsigned long ConsoleHelper::GetAliases(std::string& aliases, std::string& exeName) const
	{
		return Kernel32::GetConsoleAliases(to_tstring(aliases.c_str()).data(), static_cast<DWORD>(aliases.size()), to_tstring(exeName.c_str()).data());
	}

	unsigned long ConsoleHelper::GetAliasesLength(std::string& exeName) const
	{
		return Kernel32::GetConsoleAliasesLength(to_tstring(exeName.c_str()).data());
	}

	unsigned long ConsoleHelper::GetAliasExes(std::string& nameBuffer) const
	{
		return Kernel32::GetConsoleAliasExes(to_tstring(nameBuffer.c_str()).data(), static_cast<DWORD>(nameBuffer.size()));
	}

	unsigned long ConsoleHelper::GetAliasExesLength() const
	{
		return Kernel32::GetConsoleAliasExesLength();
	}

	unsigned int ConsoleHelper::GetInputCodePage(void) const
	{
		return Kernel32::GetConsoleCP();
		//TODO: Translate Windows Code Page to QOR CodePage
		//There is info here to be looked up - Computer\HKEY_CLASSES_ROOT\MIME\Database\Charset\iso-8859-1
		//but it's messy.
		//return Mib::ASCII;
	}

	bool ConsoleHelper::GetDisplayMode(unsigned long& modeFlags) const
	{
		return Kernel32::GetConsoleDisplayMode(&modeFlags);
	}

	platform::win::Coord ConsoleHelper::GetFontSize(void* hConsoleOutput, unsigned long nFont) const
	{
		COORD fontSize = Kernel32::GetConsoleFontSize(hConsoleOutput, nFont);
		return *(reinterpret_cast<platform::win::Coord*>(&fontSize));
	}

	bool ConsoleHelper::GetHistoryInfo(qor::platform::win::ConsoleHistoryInfo& consoleHistoryInfo) const
	{
		return Kernel32::GetConsoleHistoryInfo((PCONSOLE_HISTORY_INFO)(&consoleHistoryInfo));
	}

	bool ConsoleHelper::GetMode(void* hConsoleHandle, unsigned long& mode) const
	{
		return Kernel32::GetConsoleMode(hConsoleHandle, &mode) ? true : false;
	}

	unsigned long ConsoleHelper::GetOriginalTitle(std::string& consoleTitle) const
	{
		return Kernel32::GetConsoleOriginalTitle( (TCHAR*)(consoleTitle.data()), static_cast<DWORD>(consoleTitle.size() / sizeof(TCHAR)) );
	}

	unsigned int ConsoleHelper::GetOutputCodePage(void) const
	{
		return Kernel32::GetConsoleOutputCP();
	}

	unsigned long ConsoleHelper::GetProcessList(std::vector< unsigned long >& processList) const
	{
		if(processList.empty())
		{
			processList.resize(1);
		}
		unsigned long count = Kernel32::GetConsoleProcessList(reinterpret_cast<LPDWORD>(processList.data()), static_cast<DWORD>(processList.size()));
		if(count > processList.size())
		{
			processList.resize(count);
			count = Kernel32::GetConsoleProcessList(reinterpret_cast<LPDWORD>(processList.data()), static_cast<DWORD>(processList.size()));
		}
		return count;
	}

	bool ConsoleHelper::GetScreenBufferInfo(void* hConsoleOutput, platform::win::ConsoleScreenBufferInfo* consoleScreenBufferInfo) const
	{
		return Kernel32::GetConsoleScreenBufferInfo(hConsoleOutput, reinterpret_cast<PCONSOLE_SCREEN_BUFFER_INFO>(consoleScreenBufferInfo)) ? true : false;
	}

	bool ConsoleHelper::GetScreenBufferInfoEx(void* hConsoleOutput, platform::win::ConsoleScreenBufferInfoEx* consoleScreenBufferInfoEx) const
	{
		return Kernel32::GetConsoleScreenBufferInfoEx(hConsoleOutput, reinterpret_cast<PCONSOLE_SCREEN_BUFFER_INFOEX>(consoleScreenBufferInfoEx)) ? true : false;
	}

	bool ConsoleHelper::GetSelectionInfo(platform::win::ConsoleSelectionInfo& consoleSelectionInfo) const
	{
		return Kernel32::GetConsoleSelectionInfo(reinterpret_cast<PCONSOLE_SELECTION_INFO>(&consoleSelectionInfo)) ? true : false;
	}

	unsigned long ConsoleHelper::GetTitle(std::string& consoleTitle) const
	{
		return Kernel32::GetConsoleTitle(to_tstring(consoleTitle.c_str()).data(), static_cast<DWORD>(consoleTitle.size()));
	}

	void* ConsoleHelper::GetWindow() const
	{
		return Kernel32::GetConsoleWindow();
	}
	
	bool ConsoleHelper::GetCurrentFont(void* hConsoleOutput, bool maximumWindow, platform::win::ConsoleFontInfo* consoleCurrentFont) const
	{
		return Kernel32::GetCurrentConsoleFont(hConsoleOutput, maximumWindow ? TRUE : FALSE, reinterpret_cast<::PCONSOLE_FONT_INFO>(consoleCurrentFont)) ? true : false;
	}

	bool ConsoleHelper::GetCurrentFontEx(void* hConsoleOutput, bool maximumWindow, platform::win::ConsoleFontInfoEx* consoleCurrentFontEx) const
	{
		return Kernel32::GetCurrentConsoleFontEx(hConsoleOutput, maximumWindow ? TRUE : FALSE, reinterpret_cast< PCONSOLE_FONT_INFOEX >(consoleCurrentFontEx)) ? true : false;
	}

	platform::win::Coord ConsoleHelper::GetLargestWindowSize(void* hConsoleOutput) const
	{
		auto size = Kernel32::GetLargestConsoleWindowSize(hConsoleOutput);
		return *(reinterpret_cast<platform::win::Coord*>(&size));
	}
	
	bool ConsoleHelper::GetNumberOfInputEvents(void* hConsoleInput, unsigned long& numberOfEvents) const
	{
		return Kernel32::GetNumberOfConsoleInputEvents(hConsoleInput, &numberOfEvents) ? true : false;
	}

	unsigned long ConsoleHelper::GetNumberOfMouseButtons() const
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

	Handle ConsoleHelper::GetStdHandle(StdHandles nStdHandle) const
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
			//std.SetProtectFromClose(false);
			std.DontClose();
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

	bool ConsoleHelper::PeekInput(void* hConsoleInput, platform::win::InputRecord* buffer, unsigned long length, unsigned long& numberOfEventsRead) const
	{
		return Kernel32::PeekConsoleInput(hConsoleInput, reinterpret_cast<::PINPUT_RECORD>(buffer), length, &numberOfEventsRead) ? true : false;
	}

	bool ConsoleHelper::Read(void* hConsoleInput, void* buffer, unsigned long numberOfCharsToRead, unsigned long& numberOfCharsRead, platform::win::ConsoleReadControl* readcontrol) const
	{
		return Kernel32::ReadConsole(hConsoleInput, buffer, numberOfCharsToRead, &numberOfCharsRead, reinterpret_cast<::PCONSOLE_READCONSOLE_CONTROL>(readcontrol)) ? true : false;
	}

	bool ConsoleHelper::ReadInput(void* hConsoleInput, platform::win::InputRecord* buffer, unsigned long length, unsigned long& numberOfEventsRead) const
	{
		return Kernel32::ReadConsoleInput(hConsoleInput, reinterpret_cast<::PINPUT_RECORD>(buffer), length, &numberOfEventsRead) ? true : false;
	}
	bool ConsoleHelper::ReadInputEx(void* hConsoleInput, platform::win::InputRecord* buffer, unsigned long length, unsigned long& numberOfEventsRead, unsigned short flags) const
	{
		return Kernel32::ReadConsoleInputEx(hConsoleInput, reinterpret_cast<::PINPUT_RECORD>(buffer), length, &numberOfEventsRead, flags) ? true : false;
	}

	bool ConsoleHelper::ReadOutput(void* hConsoleOutput, platform::win::CharInfo* buffer, platform::win::Coord bufferSize, platform::win::Coord bufferCoord, qor::platform::win::SmallRect* readRegion) const
	{
		return Kernel32::ReadConsoleOutput(hConsoleOutput, reinterpret_cast<::PCHAR_INFO>(buffer), *(reinterpret_cast<COORD*>(&bufferSize)), *(reinterpret_cast<COORD*>(&bufferCoord)), reinterpret_cast<::PSMALL_RECT>(readRegion)) ? true : false;
	}

	bool ConsoleHelper::ReadOutputAttribute(void* hConsoleOutput, unsigned short* attribute, unsigned long length, platform::win::Coord readCoord, unsigned long& numberOfAttrsRead) const
	{
		return Kernel32::ReadConsoleOutputAttribute(hConsoleOutput, attribute, length, *(reinterpret_cast<COORD*>(&readCoord)), &numberOfAttrsRead) ? true : false;
	}

	bool  ConsoleHelper::ReadOutputCharacter(void* hConsoleOutput, string_t characters, platform::win::Coord dwReadCoord, unsigned long& numberOfCharsRead) const
	{
		return Kernel32::ReadConsoleOutputCharacter(hConsoleOutput, characters.data(), static_cast<unsigned long>(characters.size()), *(reinterpret_cast<COORD*>(&dwReadCoord)), &numberOfCharsRead) ? true : false;
	}

	long  ConsoleHelper::ReleasePseudo(void* hPC) const
	{
		return Kernel32::ReleasePseudoConsole((HPCON)(hPC));
	}

	long  ConsoleHelper::ResizePseudo(void* hPC, platform::win::Coord size) const
	{
		return Kernel32::ResizePseudoConsole((HPCON)(hPC), *(reinterpret_cast<COORD*>(&size)));
	}

	bool  ConsoleHelper::ScrollScreenBuffer(void* hConsoleOutput, const qor::platform::win::SmallRect* scrollRectangle, const qor::platform::win::SmallRect* clipRectangle, platform::win::Coord destinationOrigin, const platform::win::CharInfo* fill) const
	{
		return Kernel32::ScrollConsoleScreenBuffer(hConsoleOutput, reinterpret_cast<const ::SMALL_RECT*>(scrollRectangle), reinterpret_cast<const ::SMALL_RECT*>(clipRectangle), *(reinterpret_cast<COORD*>(&destinationOrigin)), reinterpret_cast<const ::CHAR_INFO*>(fill)) ? true : false;
	}

	bool ConsoleHelper::SetActiveScreenBuffer(void* hConsoleOutput) const
	{
		return Kernel32::SetConsoleActiveScreenBuffer(hConsoleOutput) ? true : false;
	}
	
	bool ConsoleHelper::SetInputCodePage(unsigned int codePage) const
	{
		return Kernel32::SetConsoleCP(codePage) ? true : false;
	}

	//bool ConsoleHelper::SetCP(const CCodePage& codePage);
	bool ConsoleHelper::SetCtrlHandler(CtrlHandlerCallback handlerRoutine, bool add) const
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

	bool ConsoleHelper::SetCursorInfo(void* hConsoleOutput, const platform::win::ConsoleCursorInfo* consoleCursorInfo) const
	{
		return Kernel32::SetConsoleCursorInfo(hConsoleOutput, reinterpret_cast<const CONSOLE_CURSOR_INFO *>(consoleCursorInfo)) ? true : false;
	}

	bool ConsoleHelper::SetCursorPosition(void* hConsoleOutput, platform::win::Coord cursorPosition) const
	{
		return Kernel32::SetConsoleCursorPosition(hConsoleOutput, *(reinterpret_cast<COORD*>(&cursorPosition))) ? true : false;
	}

	bool ConsoleHelper::SetMode(void* hConsoleHandle, unsigned long mode) const
	{
		return Kernel32::SetConsoleMode(hConsoleHandle, mode) ? true : false;
	}

	bool ConsoleHelper::SetDisplayMode(void* hConsoleOutput, unsigned long flags, platform::win::Coord* newScreenBufferDimensions) const
	{
		return Kernel32::SetConsoleDisplayMode(hConsoleOutput, flags, reinterpret_cast<::PCOORD>(newScreenBufferDimensions)) ? true : false;
	}

	bool ConsoleHelper::SetHistoryInfo(platform::win::ConsoleHistoryInfo* consoleHistoryInfo) const
	{
		return Kernel32::SetConsoleHistoryInfo(reinterpret_cast<::PCONSOLE_HISTORY_INFO>(consoleHistoryInfo)) ? true : false;
	}

	bool ConsoleHelper::SetOutputCodePage(unsigned int codePageID) const
	{
		return Kernel32::SetConsoleOutputCP(codePageID) ? true : false;
	}

	bool ConsoleHelper::SetScreenBufferInfoEx(void* hConsoleOutput, platform::win::ConsoleScreenBufferInfoEx* consoleScreenBufferInfoEx) const
	{
		return Kernel32::SetConsoleScreenBufferInfoEx(hConsoleOutput, reinterpret_cast<::PCONSOLE_SCREEN_BUFFER_INFOEX>(consoleScreenBufferInfoEx)) ? true : false;
	}

	bool ConsoleHelper::SetScreenBufferSize(void* hConsoleOutput, platform::win::Coord size) const
	{
		return Kernel32::SetConsoleScreenBufferSize(hConsoleOutput, *(reinterpret_cast<::COORD*>(&size))) ? true : false;
	}

	bool ConsoleHelper::SetTextAttribute(void* hConsoleOutput, unsigned short attributes) const
	{
		return Kernel32::SetConsoleTextAttribute(hConsoleOutput, attributes) ? true : false;
	}

	bool ConsoleHelper::SetTitle(const std::string& consoleTitle) const
	{
		return Kernel32::SetConsoleTitle(to_tstring(consoleTitle.c_str()).c_str());
	}

	bool ConsoleHelper::SetWindowInfo(void* hConsoleOutput, bool absolute, const qor::platform::win::SmallRect* consoleWindow) const
	{
		return Kernel32::SetConsoleWindowInfo(hConsoleOutput, absolute ? 1 : 0, reinterpret_cast<const SMALL_RECT*>(consoleWindow)) ? true : false;
	}

	bool ConsoleHelper::SetCurrentFontEx(void* hConsoleOutput, bool maximumWindow, platform::win::ConsoleFontInfoEx* consoleCurrentFontEx) const
	{
		return Kernel32::SetCurrentConsoleFontEx(hConsoleOutput, maximumWindow ? TRUE : FALSE, reinterpret_cast<PCONSOLE_FONT_INFOEX>(consoleCurrentFontEx)) ? true : false;
	}

	bool ConsoleHelper::SetStdHandle(StdHandles stdHandle, Handle& handle) const
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

	bool ConsoleHelper::Write(void* hConsoleOutput, const void* buffer, unsigned long numberOfCharsToWrite, unsigned long& numberOfCharsWritten) const
	{
		return Kernel32::WriteConsole(hConsoleOutput, buffer, numberOfCharsToWrite, &numberOfCharsWritten, nullptr) ? true : false;
	}

	bool ConsoleHelper::WriteInput(void* hConsoleInput, const platform::win::InputRecord* buffer, unsigned long length, unsigned long& numberOfEventsWritten) const
	{
		return Kernel32::WriteConsoleInput(hConsoleInput, reinterpret_cast<const ::INPUT_RECORD*>(buffer), length, &numberOfEventsWritten) ? true : false;
	}

	bool ConsoleHelper::WriteOutput(void* hConsoleOutput, const platform::win::CharInfo* buffer, platform::win::Coord bufferSize, platform::win::Coord bufferCoord, qor::platform::win::SmallRect* writeRegion) const
	{
		return Kernel32::WriteConsoleOutput(hConsoleOutput, reinterpret_cast<const ::CHAR_INFO*>(buffer), *(reinterpret_cast<COORD*>(&bufferSize)), *(reinterpret_cast<COORD*>(&bufferCoord)), reinterpret_cast<::PSMALL_RECT>(writeRegion)) ? true : false;
	}

	bool ConsoleHelper::WriteOutputAttribute(void* hConsoleOutput, const unsigned short* attribute, unsigned long length, platform::win::Coord writeCoord, unsigned long& numberOfAttrsWritten) const
	{
		return Kernel32::WriteConsoleOutputAttribute(hConsoleOutput, attribute, length, *(reinterpret_cast<COORD*>(&writeCoord)), &numberOfAttrsWritten) ? true : false;
	}

	bool ConsoleHelper::WriteOutputCharacter(void* hConsoleOutput, const string_t character, unsigned long length, platform::win::Coord writeCoord, unsigned long& numberOfCharsWritten) const
	{
		return Kernel32::WriteConsoleOutputCharacter(hConsoleOutput, character.data(), length, *(reinterpret_cast<COORD*>(&writeCoord)), &numberOfCharsWritten) ? true : false;
	}

}}}//qor::ui::win
