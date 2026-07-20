// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "consolescreenbuffer.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "src/platform/os/windows/api_layer/kernel/kernel32.h" //kernel32.h must be the first windows header as it's the primary inclusion point for windows.h
#include "src/platform/os/windows/common/stringconv.h"
#include "src/platform/os/windows/common/constants.h"
#include "src/platform/os/windows/common/structures.h"

using namespace qor::win::api;
using namespace qor::platform::win;

namespace qor { namespace ui { namespace win {

    ConsoleScreenBuffer::ConsoleScreenBuffer(void* handle, bool owner) : m_handle(handle), m_owner(owner){ }

    ConsoleScreenBuffer::~ConsoleScreenBuffer()
    {
        if(m_owner)
        {
            Kernel32::CloseHandle(m_handle);
        }
    }
    
    void* ConsoleScreenBuffer::Handle() const
    {
        return m_handle;
    }

	unsigned long ConsoleScreenBuffer::GetMode() const
	{
		unsigned long mode = 0;		
		if(!m_helper.GetMode(m_handle, mode))
		{
			continuable("Failed to get Console input mode.");
		}
		return mode;
	}

    bool ConsoleScreenBuffer::SetMode(unsigned long mode) const
    {
        return m_helper.SetMode(m_handle, mode);
    }

    unsigned int ConsoleScreenBuffer::GetCodePage() const
    {
        return m_helper.GetOutputCodePage();
    }

    bool ConsoleScreenBuffer::SetCodePage(unsigned int codePage) const
    {
        return m_helper.SetOutputCodePage(codePage);
    }

    bool ConsoleScreenBuffer::SetDisplayMode(unsigned long flags, platform::win::Coord* newScreenBufferDimensions) const
    {
        return m_helper.SetDisplayMode(m_handle, flags, newScreenBufferDimensions);
    }

    bool ConsoleScreenBuffer::FillOutputAttribute(unsigned short attribute, unsigned long length, qor::platform::win::Coord writeCoord, unsigned long& numberOfAttrsWritten) const
    {
        return m_helper.FillOutputAttribute(m_handle, attribute, length, writeCoord, numberOfAttrsWritten);
    }

    bool ConsoleScreenBuffer::FillOutputCharacter(char_t character, unsigned long length, qor::platform::win::Coord writeCoord, unsigned long& numberOfCharsWritten) const
    {
        return m_helper.FillOutputCharacter(m_handle, character, length, writeCoord, numberOfCharsWritten);
    }

    platform::win::Coord ConsoleScreenBuffer::GetFontSize(unsigned long nFont) const
    {
        return m_helper.GetFontSize(m_handle, nFont);
    }

    bool ConsoleScreenBuffer::GetCurrentFont(bool maximumWindow, platform::win::ConsoleFontInfo* currentFont) const
    {
        return m_helper.GetCurrentFont(m_handle, maximumWindow, currentFont);
    }

    bool ConsoleScreenBuffer::GetCurrentFontEx(bool maximumWindow, platform::win::ConsoleFontInfoEx* currentFontEx) const
    {
        return m_helper.GetCurrentFontEx(m_handle, maximumWindow, currentFontEx);
    }

    bool ConsoleScreenBuffer::GetInfo(platform::win::ConsoleScreenBufferInfo* screenBufferInfo) const
    {
        return m_helper.GetScreenBufferInfo(m_handle, screenBufferInfo);
    }

    bool ConsoleScreenBuffer::GetInfoEx(platform::win::ConsoleScreenBufferInfoEx* screenBufferInfoEx) const
    {
        memset(screenBufferInfoEx, 0, sizeof(platform::win::ConsoleScreenBufferInfoEx));
        screenBufferInfoEx->cbSize = sizeof(platform::win::ConsoleScreenBufferInfoEx);
        return m_helper.GetScreenBufferInfoEx(m_handle, screenBufferInfoEx);
    }

    bool ConsoleScreenBuffer::Scroll(const qor::platform::win::SmallRect* scrollRectangle, const qor::platform::win::SmallRect* clipRectangle, platform::win::Coord destinationOrigin, const platform::win::CharInfo* fill) const
    {
        return m_helper.ScrollScreenBuffer(m_handle, scrollRectangle, clipRectangle, destinationOrigin, fill);
    }

    bool ConsoleScreenBuffer::SetInfoEx(platform::win::ConsoleScreenBufferInfoEx* screenBufferInfoEx) const
    {
        return m_helper.SetScreenBufferInfoEx(m_handle, screenBufferInfoEx);
    }

    bool ConsoleScreenBuffer::SetSize(platform::win::Coord size) const
    {
        return m_helper.SetScreenBufferSize(m_handle, size);
    }

    bool ConsoleScreenBuffer::SetTextAttribute(unsigned short attributes) const
    {
        return m_helper.SetTextAttribute(m_handle, attributes);
    }

    bool ConsoleScreenBuffer::SetCurrentFontEx(bool maximumWindow, platform::win::ConsoleFontInfoEx* currentFontEx) const
    {
        return m_helper.SetCurrentFontEx(m_handle, maximumWindow, currentFontEx);
    }

    bool ConsoleScreenBuffer::Write(const void* buffer, unsigned long numberOfCharsToWrite, unsigned long& numberOfCharsWritten) const
    {
        return m_helper.Write(m_handle, buffer, numberOfCharsToWrite, numberOfCharsWritten);
    }

    bool ConsoleScreenBuffer::WriteOutput(const platform::win::CharInfo* buffer, platform::win::Coord bufferSize, platform::win::Coord bufferCoord, qor::platform::win::SmallRect* writeRegion) const
    {
        return m_helper.WriteOutput(m_handle, buffer, bufferSize, bufferCoord, writeRegion);
    }

    bool ConsoleScreenBuffer::WriteOutputAttribute(const unsigned short* attribute, unsigned long length, platform::win::Coord writeCoord, unsigned long& numberOfAttrsWritten) const
    {
        return m_helper.WriteOutputAttribute(m_handle, attribute, length, writeCoord, numberOfAttrsWritten);
    }

    bool ConsoleScreenBuffer::WriteOutputCharacter(const string_t character, unsigned long length, platform::win::Coord writeCoord, unsigned long& numberOfCharsWritten) const
    {
        return m_helper.WriteOutputCharacter(m_handle, character, length, writeCoord, numberOfCharsWritten);
    }

    bool ConsoleScreenBuffer::ReadOutput(platform::win::CharInfo* buffer, platform::win::Coord bufferSize, platform::win::Coord bufferCoord, qor::platform::win::SmallRect* readRegion) const
    {
        return m_helper.ReadOutput(m_handle, buffer, bufferSize, bufferCoord, readRegion);
    }

    bool ConsoleScreenBuffer::ReadOutputAttribute(unsigned short* attribute, unsigned long length, platform::win::Coord readCoord, unsigned long& numberOfAttrsRead) const
    {
        return m_helper.ReadOutputAttribute(m_handle, attribute, length, readCoord, numberOfAttrsRead);
    }

    bool ConsoleScreenBuffer::ReadOutputCharacter(string_t characters, platform::win::Coord readCoord, unsigned long& numberOfCharsRead) const
    {
        return m_helper.ReadOutputCharacter(m_handle, characters, readCoord, numberOfCharsRead);
    }

    platform::win::Coord ConsoleScreenBuffer::GetLargestWindowSize() const
    {
        return m_helper.GetLargestWindowSize(m_handle);
    }

    bool ConsoleScreenBuffer::SetWindowInfo(bool absolute, const qor::platform::win::SmallRect* consoleWindow) const
    {
        return m_helper.SetWindowInfo(m_handle, absolute, consoleWindow);
    }

    bool ConsoleScreenBuffer::SetCusrsorInfo(const platform::win::ConsoleCursorInfo* consoleCursorInfo) const
    {
        return m_helper.SetCursorInfo(m_handle, consoleCursorInfo);
    }

    bool ConsoleScreenBuffer::SetCursorPosition(platform::win::Coord cursorPosition) const
    {
        return m_helper.SetCursorPosition(m_handle, cursorPosition);
    }
    
}}}//qor::ui::win