// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_UI_CONSOLE_SCREENBUFFER
#define QOR_PP_H_OS_WINDOWS_UI_CONSOLE_SCREENBUFFER

#include "consolehelper.h"

namespace qor { namespace ui { namespace win {

	class qor_pp_module_interface(QOR_WINCONSOLE) ConsoleScreenBuffer
	{
	public:

        ConsoleScreenBuffer(void* handle);
        ~ConsoleScreenBuffer();
        void* Handle() const;
        unsigned long GetMode() const;
        bool SetMode(unsigned long mode) const;
        unsigned int GetCodePage() const;
        bool SetCodePage(unsigned int codePage) const;
        bool SetDisplayMode(unsigned long flags, platform::win::Coord* newScreenBufferDimensions) const;
		bool FillOutputAttribute(unsigned short attribute, unsigned long length, qor::platform::win::Coord writeCoord, unsigned long& numberOfAttrsWritten) const;
		bool FillOutputCharacter(char_t character, unsigned long length, qor::platform::win::Coord writeCoord, unsigned long& numberOfCharsWritten) const;
		platform::win::Coord GetFontSize(unsigned long nFont) const;
		bool GetCurrentFont(bool maximumWindow, platform::win::ConsoleFontInfo* consoleCurrentFont) const;
		bool GetCurrentFontEx(bool maximumWindow, platform::win::ConsoleFontInfoEx* consoleCurrentFontEx) const;
		bool GetInfo(platform::win::ConsoleScreenBufferInfo* consoleScreenBufferInfo) const;
		bool GetInfoEx(platform::win::ConsoleScreenBufferInfoEx* consoleScreenBufferInfoEx) const;
        bool Scroll(const qor::platform::win::SmallRect* scrollRectangle, const qor::platform::win::SmallRect* clipRectangle, platform::win::Coord destinationOrigin, const platform::win::CharInfo* fill) const;
		bool SetInfoEx(platform::win::ConsoleScreenBufferInfoEx* consoleScreenBufferInfoEx) const;
		bool SetSize(platform::win::Coord size) const;
		bool SetTextAttribute(unsigned short attributes) const;
		bool SetCurrentFontEx(bool maximumWindow, platform::win::ConsoleFontInfoEx* consoleCurrentFontEx) const;
		bool Write(const void* buffer, unsigned long numberOfCharsToWrite, unsigned long& numberOfCharsWritten) const;
		bool WriteOutput(const platform::win::CharInfo* buffer, platform::win::Coord bufferSize, platform::win::Coord bufferCoord, qor::platform::win::SmallRect* writeRegion) const;
		bool WriteOutputAttribute(const unsigned short* attribute, unsigned long length, platform::win::Coord writeCoord, unsigned long& numberOfAttrsWritten) const;
		bool WriteOutputCharacter(const string_t character, unsigned long length, platform::win::Coord writeCoord, unsigned long& numberOfCharsWritten) const;
		bool ReadOutput(platform::win::CharInfo* buffer, platform::win::Coord bufferSize, platform::win::Coord bufferCoord, qor::platform::win::SmallRect* readRegion) const;
		bool ReadOutputAttribute(unsigned short* attribute, unsigned long length, platform::win::Coord readCoord, unsigned long& numberOfAttrsRead) const;
		bool ReadOutputCharacter(string_t characters, platform::win::Coord dwReadCoord, unsigned long& numberOfCharsRead) const;
        platform::win::Coord GetLargestWindowSize() const;
        bool SetWindowInfo(bool absolute, const qor::platform::win::SmallRect* consoleWindow) const;
        bool SetCusrsorInfo(const platform::win::ConsoleCursorInfo* consoleCursorInfo) const;
        bool SetCursorPosition(platform::win::Coord cursorPosition) const;

    private:
        ConsoleHelper m_helper;
        void* m_handle;
    };

}}}//qor::ui::win

#endif//QOR_PP_H_OS_WINDOWS_UI_CONSOLE_SCREENBUFFER
