// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_UI_CONSOLE_INPUT
#define QOR_PP_H_OS_WINDOWS_UI_CONSOLE_INPUT

#include "consolehelper.h"

namespace qor { namespace ui { namespace win {

	class qor_pp_module_interface(QOR_WINCONSOLE) ConsoleInput
	{
	public:

        ConsoleInput(void* handle);
        ~ConsoleInput();
        void* Handle();
        unsigned long GetMode();
        bool SetMode(unsigned long mode);
        unsigned int GetCodePage();
        bool SetCodePage(unsigned int codePage);
		unsigned long GetNumberOfEvents();
		unsigned long GetNumberOfMouseButtons();
		bool FlushInputBuffer();		
		bool PeekInput(platform::win::InputRecord* buffer, unsigned long length, unsigned long& numberOfEventsRead);
		bool Read(void* buffer, unsigned long numberOfCharsToRead, unsigned long& numberOfCharsRead, platform::win::ConsoleReadControl* readcontrol);
		bool ReadInput(platform::win::InputRecord* buffer, unsigned long length, unsigned long& numberOfEventsRead);
		bool ReadInputEx(platform::win::InputRecord* buffer, unsigned long length, unsigned long& numberOfEventsRead, unsigned short flags);
		bool WriteInput(const platform::win::InputRecord* buffer, unsigned long length, unsigned long& numberOfEventsWritten);

    private:
        ConsoleHelper m_helper;
        void* m_handle;
    };

}}}//qor::ui::win

#endif//QOR_PP_H_OS_WINDOWS_UI_CONSOLE_INPUT
