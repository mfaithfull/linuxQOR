// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/flyers/error/error.h"
#include "consoleinput.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "src/platform/os/windows/api_layer/kernel/kernel32.h" //kernel32.h must be the first windows header as it's the primary inclusion point for windows.h
#include "src/platform/os/windows/common/stringconv.h"
#include "src/platform/os/windows/common/constants.h"
#include "src/platform/os/windows/common/structures.h"

using namespace qor::win::api;
using namespace qor::platform::win;

namespace qor { namespace ui { namespace win {

    ConsoleInput::ConsoleInput(void* handle) : m_handle(handle){ }

    ConsoleInput::~ConsoleInput(){ }

    void* ConsoleInput::Handle()
    {
        return m_handle;
    }

	unsigned long ConsoleInput::GetMode()
	{
		unsigned long mode = 0;		
		if(!m_helper.GetMode(m_handle, mode))
		{
			continuable("Failed to get Console input mode.");
		}
		return mode;
	}

    bool ConsoleInput::SetMode(unsigned long mode)
    {
        return m_helper.SetMode(m_handle, mode);
    }

    unsigned int ConsoleInput::GetCodePage()
    {
        return m_helper.GetInputCodePage();
    }

    bool ConsoleInput::SetCodePage(unsigned int codePage)
    {
        return m_helper.SetInputCodePage(codePage);
    }

    unsigned long ConsoleInput::GetNumberOfEvents()
    {
        unsigned long numberOfEvents = 0;
        if (!m_helper.GetNumberOfInputEvents(m_handle, numberOfEvents))
        {
            continuable("Call to Kernel32::GetNumberOfConsoleInputEvents failed.");
        }
        return numberOfEvents;
    }

    unsigned long ConsoleInput::GetNumberOfMouseButtons()
    {
        return m_helper.GetNumberOfMouseButtons();
    }

    bool ConsoleInput::FlushInputBuffer()
    {
        return m_helper.FlushInputBuffer(m_handle) ? true : false;
    }

    bool ConsoleInput::PeekInput(platform::win::InputRecord* buffer, unsigned long length, unsigned long& numberOfEventsRead)
    {
        return m_helper.PeekInput(m_handle, buffer, length, numberOfEventsRead) ? true : false;
    }

    bool ConsoleInput::Read(void* buffer, unsigned long numberOfCharsToRead, unsigned long& numberOfCharsRead, platform::win::ConsoleReadControl* readcontrol)
    {
        return m_helper.Read(m_handle, buffer, numberOfCharsToRead, numberOfCharsRead, readcontrol) ? true : false;
    }

    bool ConsoleInput::ReadInput(platform::win::InputRecord* buffer, unsigned long length, unsigned long& numberOfEventsRead)
    {
        return m_helper.ReadInput(m_handle, buffer, length, numberOfEventsRead) ? true : false;
    }

    bool ConsoleInput::ReadInputEx(platform::win::InputRecord* buffer, unsigned long length, unsigned long& numberOfEventsRead, unsigned short flags)
    {
        return m_helper.ReadInputEx(m_handle, buffer, length, numberOfEventsRead, flags) ? true : false;
    }

    bool ConsoleInput::WriteInput(const platform::win::InputRecord* buffer, unsigned long length, unsigned long& numberOfEventsWritten)
    {
        return m_helper.WriteInput(m_handle, buffer, length, numberOfEventsWritten) ? true : false;
    }

}}}//qor::ui::win