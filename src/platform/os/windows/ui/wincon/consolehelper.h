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
#include "consolescreenbuffer.h"
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

		bool AddAlias(std::string& source, std::string& target, std::string& exeName);
		bool Alloc(void);
		AllocConsoleResult AllocWithOptions(AllocConsoleMode mode, bool useShowWindow, unsigned short showWindowCommand);
		bool Attach(unsigned long processId);
		void ClosePseudo(void* hPC);
		ref_of<ConsoleScreenBuffer>::type CreateScreenBuffer(unsigned long desiredAccess = qor::platform::win::GENERICREAD | qor::platform::win::GENERICWRITE, unsigned long shareMode = platform::win::FILESHAREREAD | platform::win::FILESHAREWRITE, const qor::platform::win::SecurityAttributes* securityAttributes = nullptr, bool textMode = false);
		ref_of<PseudoConsole>::type CreatePseudo(qor::platform::win::Coord size, void* hInput, void* hOutput, unsigned long flags);
		bool FillOutputAttribute(void* hConsoleOutput, unsigned short attribute, unsigned long length, qor::platform::win::Coord writeCoord, unsigned long& numberOfAttrsWritten);
		bool FillOutputCharacter(void* hConsoleOutput, char_t character, unsigned long length, qor::platform::win::Coord writeCoord, unsigned long& numberOfCharsWritten);
		bool FlushInputBuffer(void* hConsoleInput);
		bool Free(void);
		bool GenerateCtrlEvent(unsigned long ctrlEvent, unsigned long processGroupId);
		unsigned long GetAlias(std::string& source, std::string& target, std::string& exeName);
		unsigned long GetAliases(std::string& aliases, std::string& exeName);
		unsigned long GetAliasesLength(std::string& exeName);
		unsigned long GetAliasExes(std::string& nameBuffer);
		unsigned long GetAliasExesLength();
		Mib GetCP(void);
		bool GetDisplayMode(unsigned long& modeFlags);
		bool GetHistoryInfo(qor::platform::win::ConsoleHistoryInfo& consoleHistoryInfo);
		bool GetMode(void* hConsoleHandle, unsigned long& mode);
		unsigned long GetOriginalTitle(std::string& consoleTitle);
		//CCodePage GetOutputCP(void);
		//unsigned long GetProcessList(std::vector< unsigned long >& processList);
		bool GetScreenBufferInfo(void* hConsoleOutput, platform::win::ConsoleScreenBufferInfo* consoleScreenBufferInfo);
		bool GetScreenBufferInfoEx(void* hConsoleOutput, platform::win::ConsoleScreenBufferInfoEx* consoleScreenBufferInfoEx);
		//bool GetSelectionInfo(ConsoleSelectionInfo& consoleSelectionInfo);
		unsigned long GetTitle(std::string& consoleTitle);
		//COSWindow::ref_type GetWindow();
		bool GetCurrentFont(void* hConsoleOutput, bool maximumWindow, platform::win::ConsoleFontInfo* consoleCurrentFont);
		bool GetCurrentFontEx(void* hConsoleOutput, bool maximumWindow, platform::win::ConsoleFontInfoEx* consoleCurrentFontEx);

		unsigned long GetNumberOfMouseButtons();
		platform::win::Handle GetStdHandle(StdHandles stdHandle);
		bool SetActiveScreenBuffer(void* hConsoleOutput);
		//bool SetCP(const CCodePage& codePage);
		bool SetCtrlHandler(CtrlHandlerCallback handlerRoutine, bool add);
		bool SetMode(void* hConsoleHandle, unsigned long mode);
		//bool SetHistoryInfo(const ConsoleHistoryInfo& consoleHistoryInfo);
		//bool SetOutputCP(const CCodePage& codePage);
		bool SetScreenBufferSize(void* hConsoleOutput, platform::win::Coord size);
		bool SetTitle(std::string& consoleTitle);
		bool SetWindowInfo(void* hConsoleOutput, bool absolute, const qor::platform::win::SmallRect* consoleWindow);
		bool SetCurrentFontEx(void* hConsoleOutput, bool maximumWindow, platform::win::ConsoleFontInfoEx* consoleCurrentFontEx);
		bool SetStdHandle(StdHandles stdHandle, platform::win::Handle & hHandle);
		bool WriteOutput(void* hConsoleOutput, const platform::win::CharInfo* buffer, platform::win::Coord bufferSize, platform::win::Coord bufferCoord, qor::platform::win::SmallRect* writeRegion);
	};

}}}//qor::ui::win

#endif//QOR_PP_H_OS_WINDOWS_UI_CONSOLEHELPER
