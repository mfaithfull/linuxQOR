// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_UI_CONSOLE
#define QOR_PP_H_OS_WINDOWS_UI_CONSOLE

#include "src/components/ui/console/iconsole.h"
#include "consolehelper.h"
#include "consoleinput.h"
#include "consolescreenbuffer.h"

namespace qor
{
	bool qor_pp_module_interface(QOR_WINCONSOLE) ImplementsIConsole();
}

namespace qor { namespace ui { namespace win {

	class qor_pp_module_interface(QOR_WINCONSOLE) Console : public components::IConsole
	{
	public:

		Console();
		~Console();
		bool IsRedirected();
		void Reallocate();
		void SetOut(int fileHandle);
		void SetIn(int fileHandle);
		void SetErr(int fileHandle);
		void ResetOut();
		void ResetIn();
		void ResetErr();

		//IConsole implementation
		virtual void WriteLine(const string_t& output);
		virtual string_t ReadLine();
		virtual char_t ReadChar();
		virtual void WriteChar(char_t c);

		//bool Alloc(void);
		//AllocConsoleResult AllocWithOptions(AllocConsoleMode mode, bool useShowWindow, unsigned short showWindowCommand);
		//bool Attach(unsigned long processId);

		ref_of<ConsoleScreenBuffer>::type CreateScreenBuffer(unsigned long desiredAccess = qor::platform::win::GENERICREAD | qor::platform::win::GENERICWRITE, unsigned long shareMode = platform::win::FILESHAREREAD | platform::win::FILESHAREWRITE, const qor::platform::win::SecurityAttributes* securityAttributes = nullptr, bool textMode = false);
		ref_of<PseudoConsole>::type CreatePseudo(qor::platform::win::Coord size, unsigned long flags);
		ref_of<ConsoleScreenBuffer>::type GetActiveScreenBuffer();
		bool SetActiveScreenBuffer(ref_of<ConsoleScreenBuffer>::type screenBuffer);
		ref_of<ConsoleInput>::type GetInput();
		unsigned long GetDisplayMode();		
		void* OutputFile();
		void* InputFile();
		bool GenerateCtrlCEvent();
		bool GenerateCtrlBreakEvent(unsigned long processGroupId);
		bool SetCtrlHandler(CtrlHandlerCallback handlerRoutine, bool add);
		bool AddAlias(std::string& source, std::string& target, std::string& exeName);
		unsigned long GetAlias(std::string& source, std::string& target, std::string& exeName);
		unsigned long GetAliases(std::string& aliases, std::string& exeName);
		unsigned long GetAliasesLength(std::string& exeName);
		unsigned long GetAliasExes(std::string& nameBuffer);
		unsigned long GetAliasExesLength();
		qor::platform::win::ConsoleHistoryInfo GetHistoryInfo();		
		bool SetHistoryInfo(platform::win::ConsoleHistoryInfo* consoleHistoryInfo);
		std::string GetOriginalTitle();
		std::string GetTitle();
		bool SetTitle(const std::string& consoleTitle);
		std::vector< unsigned long > GetProcessList();
		void* GetWindow();
		platform::win::ConsoleSelectionInfo GetSelectionInfo();
		
	private:

		void BindCrtHandlesToStdHandles(bool bindStdIn, bool bindStdOut, bool bindStdErr);

		bool m_redirected{false};
		bool m_allocated{false};
		ConsoleHelper m_helper;
		void* m_outFile{nullptr};
		void* m_inFile{nullptr};
		void* m_errFile{nullptr};
		
	};
}}}//qor::ui::win

#endif//QOR_PP_H_OS_WINDOWS_UI_CONSOLE
