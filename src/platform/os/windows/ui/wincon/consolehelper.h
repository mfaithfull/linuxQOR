// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_UI_CONSOLEHELPER
#define QOR_PP_H_OS_WINDOWS_UI_CONSOLEHELPER

#include <string>

#include "src/qor/memory/reference/newref.h"
#include "src/qor/essentials/text/codepage/codepages.h"
#include "src/platform/os/windows/common/constants.h"
#include "src/platform/os/windows/common/structures.h"
#include "src/platform/os/windows/common/handles/handle.h"
#include "pseudoconsole.h"

//This is edge of the win namespace. No Windows specific types are allowed here.
namespace qor { namespace ui { namespace win {

	typedef int (qor_pp_compiler_stdcallconvention* CtrlHandlerCallback)(unsigned long ctrlType);

	//Input Mode flags:
	enum class ConsoleInputModeFlags : unsigned long
	{
		ProcessedInput						= 0x0001,//Some keys are processed by the system, at least CTRL+C
		LineInput							= 0x0002,
		EchoInput							= 0x0004,
		WindowInput							= 0x0008,
		MouseInput                  		= 0x0010,
		InsertMode                  		= 0x0020,
		QuickEditMode              			= 0x0040,
		ExtendedFlags               		= 0x0080,
		AutoPosition                		= 0x0100,
		VertualTerminalInput       			= 0x0200,
	};

	enum class ConsoleOutputModeFlags : unsigned long
	{
		ProcessedOutput						= 0x0001,//Backspace, tab, bell, carriage return, and line feed characters are processed
		WrapAtEOL							= 0x0002,
		VirtualTerminalProcessing			= 0x0004,
		DisableNewlineAutoReturn			= 0x0008,
		LVBGridWorldwide					= 0x0010,
	};

	class qor_pp_module_interface(QOR_WINCONSOLE) ConsoleHelper
	{
	public:

		enum StdHandles
		{
			hStdIn = 0,
			hStdOut,
			hStdErr
		};

		enum class AllocConsoleMode
		{
			Default    	= 0,
			NewWindow 	= 1,
			NoWindow  	= 2
		};

		enum class AllocConsoleResult
		{
    		NoConsole		= 0,
    		NewConsole		= 1,
    		ExistingConsole	= 2
		};

		bool AddAlias(std::string& source, std::string& target, std::string& exeName) const;
		bool Alloc(void) const;
		AllocConsoleResult AllocWithOptions(AllocConsoleMode mode, bool useShowWindow, unsigned short showWindowCommand) const;
		bool Attach(unsigned long processId) const;
		void ClosePseudo(void* hPC) const;

		void* CreateScreenBuffer(unsigned long desiredAccess = qor::platform::win::GENERICREAD | qor::platform::win::GENERICWRITE, unsigned long shareMode = platform::win::FILESHAREREAD | platform::win::FILESHAREWRITE, const qor::platform::win::SecurityAttributes* securityAttributes = nullptr, bool textMode = false) const;
		void* CreatePseudo(qor::platform::win::Coord size, void* hInput, void* hOutput, unsigned long flags) const;

		bool FillOutputAttribute(void* hConsoleOutput, unsigned short attribute, unsigned long length, qor::platform::win::Coord writeCoord, unsigned long& numberOfAttrsWritten) const;
		bool FillOutputCharacter(void* hConsoleOutput, char_t character, unsigned long length, qor::platform::win::Coord writeCoord, unsigned long& numberOfCharsWritten) const;
		bool FlushInputBuffer(void* hConsoleInput) const;
		bool Free(void) const;
		bool GenerateCtrlEvent(unsigned long ctrlEvent, unsigned long processGroupId) const;
		unsigned long GetAlias(std::string& source, std::string& target, std::string& exeName) const;
		unsigned long GetAliases(std::string& aliases, std::string& exeName) const;
		unsigned long GetAliasesLength(std::string& exeName) const;
		unsigned long GetAliasExes(std::string& nameBuffer) const;
		unsigned long GetAliasExesLength() const;
		unsigned int GetInputCodePage(void) const;
		bool GetDisplayMode(unsigned long& modeFlags) const;
		platform::win::Coord GetFontSize(void* hConsoleOutput, unsigned long nFont) const;
		bool GetHistoryInfo(qor::platform::win::ConsoleHistoryInfo& consoleHistoryInfo) const;
		bool GetMode(void* hConsoleHandle, unsigned long& mode) const;
		unsigned long GetOriginalTitle(std::string& consoleTitle) const;
		unsigned int GetOutputCodePage(void) const;
		unsigned long GetProcessList(std::vector< unsigned long >& processList) const;
		bool GetScreenBufferInfo(void* hConsoleOutput, platform::win::ConsoleScreenBufferInfo* consoleScreenBufferInfo) const;
		bool GetScreenBufferInfoEx(void* hConsoleOutput, platform::win::ConsoleScreenBufferInfoEx* consoleScreenBufferInfoEx) const;
		bool GetSelectionInfo(platform::win::ConsoleSelectionInfo& consoleSelectionInfo) const;
		unsigned long GetTitle(std::string& consoleTitle) const;
		void* GetWindow() const;
		bool GetCurrentFont(void* hConsoleOutput, bool maximumWindow, platform::win::ConsoleFontInfo* consoleCurrentFont) const;
		bool GetCurrentFontEx(void* hConsoleOutput, bool maximumWindow, platform::win::ConsoleFontInfoEx* consoleCurrentFontEx) const;
		platform::win::Coord GetLargestWindowSize(void* hConsoleOutput) const;
		bool GetNumberOfInputEvents(void* hConsoleInput, unsigned long& numberOfEvents) const;
		unsigned long GetNumberOfMouseButtons() const;
		platform::win::Handle GetStdHandle(StdHandles stdHandle) const;
		bool PeekInput(void* hConsoleInput, platform::win::InputRecord* buffer, unsigned long length, unsigned long& numberOfEventsRead) const;
		bool Read(void* hConsoleInput, void* buffer, unsigned long numberOfCharsToRead, unsigned long& numberOfCharsRead, platform::win::ConsoleReadControl* readcontrol) const;
		bool ReadInput(void* hConsoleInput, platform::win::InputRecord* buffer, unsigned long length, unsigned long& numberOfEventsRead) const;
		bool ReadInputEx(void* hConsoleInput, platform::win::InputRecord* buffer, unsigned long length, unsigned long& numberOfEventsRead, unsigned short flags) const;
		bool ReadOutput(void* hConsoleOutput, platform::win::CharInfo* buffer, platform::win::Coord bufferSize, platform::win::Coord bufferCoord, qor::platform::win::SmallRect* readRegion) const;
		bool ReadOutputAttribute(void* hConsoleOutput, unsigned short* attribute, unsigned long length, platform::win::Coord readCoord, unsigned long& numberOfAttrsRead) const;
		bool ReadOutputCharacter(void* hConsoleOutput, string_t characters, platform::win::Coord dwReadCoord, unsigned long& numberOfCharsRead) const;
		long ReleasePseudo(void* hPC) const;
		long ResizePseudo(void* hPC, platform::win::Coord size) const;
		bool ScrollScreenBuffer(void* hConsoleOutput, const qor::platform::win::SmallRect* scrollRectangle, const qor::platform::win::SmallRect* clipRectangle, platform::win::Coord destinationOrigin, const platform::win::CharInfo* fill) const;
		bool SetActiveScreenBuffer(void* hConsoleOutput) const;
		bool SetInputCodePage(unsigned int codePage) const;
		bool SetCtrlHandler(CtrlHandlerCallback handlerRoutine, bool add) const;
		bool SetCursorInfo(void* hConsoleOutput, const platform::win::ConsoleCursorInfo * consoleCursorInfo) const;
		bool SetCursorPosition(void* hConsoleOutput, platform::win::Coord cursorPosition) const;
		bool SetMode(void* hConsoleHandle, unsigned long mode) const;
		bool SetDisplayMode(void* hConsoleOutput, unsigned long flags, platform::win::Coord* newScreenBufferDimensions) const;
		bool SetHistoryInfo(platform::win::ConsoleHistoryInfo* consoleHistoryInfo) const;
		bool SetOutputCodePage(unsigned int codePageID) const;
		bool SetScreenBufferInfoEx(void* hConsoleOutput, platform::win::ConsoleScreenBufferInfoEx* consoleScreenBufferInfoEx) const;
		bool SetScreenBufferSize(void* hConsoleOutput, platform::win::Coord size) const;
		bool SetTextAttribute(void* hConsoleOutput, unsigned short attributes) const;
		bool SetTitle(const std::string& consoleTitle) const;
		bool SetWindowInfo(void* hConsoleOutput, bool absolute, const qor::platform::win::SmallRect* consoleWindow) const;
		bool SetCurrentFontEx(void* hConsoleOutput, bool maximumWindow, platform::win::ConsoleFontInfoEx* consoleCurrentFontEx) const;
		bool SetStdHandle(StdHandles stdHandle, platform::win::Handle & hHandle) const;
		bool Write(void* hConsoleOutput, const void* buffer, unsigned long numberOfCharsToWrite, unsigned long& numberOfCharsWritten) const;
		bool WriteInput(void* hConsoleInput, const platform::win::InputRecord* buffer, unsigned long length, unsigned long& numberOfEventsWritten) const;
		bool WriteOutput(void* hConsoleOutput, const platform::win::CharInfo* buffer, platform::win::Coord bufferSize, platform::win::Coord bufferCoord, qor::platform::win::SmallRect* writeRegion) const;
		bool WriteOutputAttribute(void* hConsoleOutput, const unsigned short* attribute, unsigned long length, platform::win::Coord writeCoord, unsigned long& numberOfAttrsWritten) const;
		bool WriteOutputCharacter(void* hConsoleOutput, const string_t character, unsigned long length, platform::win::Coord writeCoord, unsigned long& numberOfCharsWritten) const;

	};

}}}//qor::ui::win

#endif//QOR_PP_H_OS_WINDOWS_UI_CONSOLEHELPER
