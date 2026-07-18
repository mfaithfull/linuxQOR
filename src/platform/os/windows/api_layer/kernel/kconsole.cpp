// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "errorhandler.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "kernel32.h" //kernel32.h must be the first windows header as it's the primary inclusion point for windows.h
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

	BOOL Kernel32::AddConsoleAlias(LPTSTR Source, LPTSTR Target, LPTSTR ExeName)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;		
		qor_pp_useswinapi( kernel32,AddConsoleAlias);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call< BOOL, LPTSTR, LPTSTR, LPTSTR >(pFunc, Source, Target, ExeName);
		return result;
	}

	BOOL Kernel32::AllocConsole(void)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,AllocConsole);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL>(pFunc);
		return result;
	}

	HRESULT Kernel32::AllocConsoleWithOptions( PALLOC_CONSOLE_OPTIONS allocOptions, PALLOC_CONSOLE_RESULT result)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,AllocConsoleWithOptions);
		CheckReturn< HRESULT, LongSuccessCheck >::TType hresult = Library::Call<HRESULT, PALLOC_CONSOLE_OPTIONS, PALLOC_CONSOLE_RESULT>(pFunc, allocOptions, result);
		return hresult;
	}

	BOOL Kernel32::AttachConsole(DWORD dwProcessId)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,AttachConsole);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, DWORD>(pFunc, dwProcessId);
		return result;
	}

	void Kernel32::ClosePseudoConsole(HPCON hPC)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,ClosePseudoConsole);
		Library::voidCall<HPCON>(pFunc, hPC);
	}

	HANDLE Kernel32::CreateConsoleScreenBuffer(DWORD dwDesiredAccess, DWORD dwShareMode, const ::SECURITY_ATTRIBUTES* lpSecurityAttributes, DWORD dwFlags, void* lpScreenBufferData)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,CreateConsoleScreenBuffer);
		CheckReturn< HANDLE, HandleCheck >::TType h = Library::Call<HANDLE, DWORD, DWORD, const ::SECURITY_ATTRIBUTES*, DWORD, void*>(pFunc, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwFlags, lpScreenBufferData);
		return h;
	}

	HRESULT Kernel32::CreatePseudoConsole(COORD size, HANDLE hInput, HANDLE hOutput, DWORD dwFlags, HPCON* phPC)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,CreatePseudoConsole);
		CheckReturn< HRESULT, LongSuccessCheck >::TType h = Library::Call<HRESULT, COORD, HANDLE, HANDLE, DWORD, HPCON*>(pFunc, size, hInput, hOutput, dwFlags, phPC);
		return h;
	}

	BOOL Kernel32::FillConsoleOutputAttribute(HANDLE hConsoleOutput, WORD wAttribute, DWORD nLength, ::COORD dwWriteCoord, LPDWORD lpNumberOfAttrsWritten)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,FillConsoleOutputAttribute);
		CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HANDLE, WORD, DWORD, ::COORD, LPDWORD>(pFunc, hConsoleOutput, wAttribute, nLength, dwWriteCoord, lpNumberOfAttrsWritten);
		return bResult;
	}

	BOOL Kernel32::FillConsoleOutputCharacter(HANDLE hConsoleOutput, TCHAR cCharacter, DWORD nLength, ::COORD dwWriteCoord, LPDWORD lpNumberOfCharsWritten)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,FillConsoleoutputCharacter);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, TCHAR, DWORD, ::COORD, LPDWORD>(pFunc, hConsoleOutput, cCharacter, nLength, dwWriteCoord, lpNumberOfCharsWritten);
		return result;
	}

	BOOL Kernel32::FlushConsoleInputBuffer(HANDLE hConsoleInput)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,FlushConsoleInputBuffer);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE>(pFunc, hConsoleInput);
		return result;
	}

	BOOL Kernel32::FreeConsole(void)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,FreeConsole);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL>(pFunc);
		return result;
	}

	BOOL Kernel32::GenerateConsoleCtrlEvent(DWORD dwCtrlEvent, DWORD dwProcessGroupId)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GenerateConsoleCtrlEvent);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, DWORD, DWORD>(pFunc, dwCtrlEvent, dwProcessGroupId);
		return result;
	}

	DWORD Kernel32::GetConsoleAlias(LPTSTR lpSource, LPTSTR lpTargetBuffer, DWORD TargetBufferLength, LPTSTR lpExeName)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi( kernel32,GetConsoleAlias);
		 CheckReturn< DWORD, CheckNonZero< DWORD> >::TType result = Library::Call< DWORD, LPTSTR, LPTSTR, DWORD, LPTSTR >(pFunc, lpSource, lpTargetBuffer, TargetBufferLength, lpExeName);
		return result;
	}

	DWORD Kernel32::GetConsoleAliases(LPTSTR lpAliasBuffer, DWORD AliasBufferLength, LPTSTR lpExeName)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetConsoleAliases);
		CheckReturn< DWORD, CheckNonZero< DWORD> >::TType result = Library::Call< DWORD, LPTSTR, DWORD, LPTSTR >(pFunc, lpAliasBuffer, AliasBufferLength, lpExeName);
		return result;
	}

	DWORD Kernel32::GetConsoleAliasesLength(LPTSTR lpExeName)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetConsoleAliasesLength);
		return Library::Call< DWORD, LPTSTR >(pFunc, lpExeName);
	}

	DWORD Kernel32::GetConsoleAliasExes(LPTSTR lpExeNameBuffer, DWORD ExeNameBufferLength)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetConsoleAliasExes);
		CheckReturn< DWORD, CheckNonZero< DWORD> >::TType result = Library::Call< DWORD, LPTSTR, DWORD >(pFunc, lpExeNameBuffer, ExeNameBufferLength);
		return result;
	}

	DWORD Kernel32::GetConsoleAliasExesLength()
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetConsoleAliasExesLength);
		return Library::Call< DWORD >(pFunc);
	}

	UINT Kernel32::GetConsoleCP(void)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetConsoleCP);
		return Library::Call<UINT>(pFunc);
	}

	BOOL Kernel32::GetConsoleCursorInfo(HANDLE hConsoleOutput, ::PCONSOLE_CURSOR_INFO lpConsoleCursorInfo)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetConsoleCursorInfo);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, ::PCONSOLE_CURSOR_INFO>(pFunc, hConsoleOutput, lpConsoleCursorInfo);
		return result;
	}

	BOOL Kernel32::GetConsoleDisplayMode(LPDWORD lpModeFlags)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32, GetConsoleDisplayMode);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, LPDWORD>(pFunc, lpModeFlags);
		return result;
	}

	::COORD Kernel32::GetConsoleFontSize(HANDLE hConsoleOutput, DWORD nFont)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetConsoleFontSize);
		::COORD Result = Library::Call< COORD, HANDLE, DWORD >(pFunc, hConsoleOutput, nFont);
		if (Result.X == 0 && Result.Y == 0)
		{
			continuable("Windows API error: No data returned");
		}
		return Result;
	}

	BOOL Kernel32::GetConsoleHistoryInfo(::PCONSOLE_HISTORY_INFO lpConsoleHistoryInfo)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetConsoleHistoryInfo);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call< BOOL, ::PCONSOLE_HISTORY_INFO >(pFunc, lpConsoleHistoryInfo);
		return result;
	}

	BOOL Kernel32::GetConsoleMode(HANDLE hConsoleHandle, LPDWORD lpMode)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetConsoleMode);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, LPDWORD>(pFunc, hConsoleHandle, lpMode);
		return result;
	}

	DWORD Kernel32::GetConsoleOriginalTitle(LPTSTR lpConsoleTitle, DWORD nSize)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetConsoleOriginalTitle);
		DWORD result = Library::Call< DWORD, LPTSTR, DWORD >(pFunc, lpConsoleTitle, nSize);
		if (result == 0)
		{
			DWORD error = Kernel32::GetLastError();
			if (error == ERROR_SUCCESS)
			{
				continuable("Windows API Buffer not big enough");
			}
			else
			{
				continuable("Windows API error");
			}
		}
		return result;
	}

	UINT Kernel32::GetConsoleOutputCP(void)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetConsoleOutputCP);
		return Library::Call<UINT>(pFunc);
	}

	DWORD Kernel32::GetConsoleProcessList(LPDWORD lpdwProcessList, DWORD dwProcessCount)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetConsoleProcessList);
		CheckReturn< DWORD, CheckNonZero< DWORD> >::TType result = Library::Call<DWORD, LPDWORD, DWORD>(pFunc, lpdwProcessList, dwProcessCount);
		return result;
	}

	BOOL Kernel32::GetConsoleScreenBufferInfo(HANDLE hConsoleOutput, ::PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetConsoleScreenBufferInfo);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, ::PCONSOLE_SCREEN_BUFFER_INFO>(pFunc, hConsoleOutput, lpConsoleScreenBufferInfo);
		return result;
	}

	BOOL Kernel32::GetConsoleScreenBufferInfoEx(HANDLE hConsoleOutput, ::PCONSOLE_SCREEN_BUFFER_INFOEX lpConsoleScreenBufferInfoEx)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;		
		qor_pp_useswinapi(kernel32,GetConsoleScreenBufferInfoEx);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call< BOOL, HANDLE, ::PCONSOLE_SCREEN_BUFFER_INFOEX >(pFunc, hConsoleOutput, lpConsoleScreenBufferInfoEx);
		return result;
	}

	BOOL Kernel32::GetConsoleSelectionInfo(::PCONSOLE_SELECTION_INFO lpConsoleSelectionInfo)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetConsoleSelectionInfo);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call< BOOL, ::PCONSOLE_SELECTION_INFO >(pFunc, lpConsoleSelectionInfo);
		return result;
	}

	DWORD Kernel32::GetConsoleTitle(LPTSTR lpConsoleTitle, DWORD nSize)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetConsoleTitle);
		DWORD result = Library::Call<DWORD, LPTSTR, DWORD>(pFunc, lpConsoleTitle, nSize);
		if (result == 0)
		{
			DWORD error = Kernel32::GetLastError();
			if (error == ERROR_SUCCESS)
			{
				continuable("Windows API Buffer not big enough");
			}
			else
			{
				continuable("Windows API error");
			}
		}
		return result;
	}

	HWND Kernel32::GetConsoleWindow()
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetConsoleWindow);
		CheckReturn< HWND, CheckNonZero< HWND> >::TType hWnd = Library::Call<HWND>(pFunc);
		return hWnd;
	}

	BOOL Kernel32::GetCurrentConsoleFont(HANDLE hConsoleOutput, BOOL bMaximumWindow, ::PCONSOLE_FONT_INFO lpConsoleCurrentFont)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetCurrentConsoleFont);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call< BOOL, HANDLE, BOOL, ::PCONSOLE_FONT_INFO >(pFunc, hConsoleOutput, bMaximumWindow, lpConsoleCurrentFont);
		return result;
	}

	BOOL Kernel32::GetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, ::PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;		
		qor_pp_useswinapi(kernel32,GetCurrentConsoleFontEx);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call< BOOL, HANDLE, BOOL, ::PCONSOLE_FONT_INFOEX >(pFunc, hConsoleOutput, bMaximumWindow, lpConsoleCurrentFontEx);
		return result;
	}

	::COORD Kernel32::GetLargestConsoleWindowSize(HANDLE hConsoleOutput)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetLargestConsoleWindowSize);
		::COORD result = Library::Call<::COORD, HANDLE>(pFunc, hConsoleOutput);
		if (result.X == 0 && result.Y == 0)
		{
			continuable("Windows API error: No data returned");
		}
		return result;
	}

	BOOL Kernel32::GetNumberOfConsoleInputEvents(HANDLE hConsoleInput, LPDWORD lpcNumberOfEvents)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetNumberOfConsoleInputEvents);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, LPDWORD>(pFunc, hConsoleInput, lpcNumberOfEvents);
		return result;
	}

	BOOL Kernel32::GetNumberOfConsoleMouseButtons(LPDWORD lpNumberOfMouseButtons)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetNumberOfConsoleMouseButtons);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, LPDWORD>(pFunc, lpNumberOfMouseButtons);
		return result;
	}

	HANDLE Kernel32::GetStdHandle(DWORD nStdHandle)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,GetStdHandle);
		HANDLE h = Library::Call<HANDLE, DWORD>(pFunc, nStdHandle);
		if (h == INVALID_HANDLE_VALUE)
		{
			continuable("Windows API error: No data returned");
		}
#ifndef _WINDOWS	//It's normal for this to fail if there is no Console
		else if (h == 0 && (nStdHandle != (unsigned long)-11))
		{
			continuable("Windows API error: No data returned");
		}
#endif
		return h;
	}

	BOOL Kernel32::PeekConsoleInput(HANDLE hConsoleInput, ::PINPUT_RECORD lpBuffer, DWORD nLength, LPDWORD lpNumberOfEventsRead)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,PeekConsoleInput);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, ::PINPUT_RECORD, DWORD, LPDWORD>(pFunc, hConsoleInput, lpBuffer, nLength, lpNumberOfEventsRead);
		return result;
	}

	BOOL Kernel32::ReadConsole(HANDLE hConsoleInput, void* lpBuffer, DWORD nNumberOfCharsToRead, LPDWORD lpNumberOfCharsRead, ::PCONSOLE_READCONSOLE_CONTROL pReadcontrol)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,ReadConsole);
		return Library::Call<BOOL, HANDLE, void*, DWORD, LPDWORD, ::PCONSOLE_READCONSOLE_CONTROL>(pFunc, hConsoleInput, lpBuffer, nNumberOfCharsToRead, lpNumberOfCharsRead, pReadcontrol);
	}

	BOOL Kernel32::ReadConsoleInput(HANDLE hConsoleInput, ::PINPUT_RECORD lpBuffer, DWORD nLength, LPDWORD lpNumberOfEventsRead)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,ReadConsoleInput);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, ::PINPUT_RECORD, DWORD, LPDWORD>(pFunc, hConsoleInput, lpBuffer, nLength, lpNumberOfEventsRead);
		return result;
	}

	BOOL Kernel32::ReadConsoleInputEx(HANDLE hConsoleInput, ::PINPUT_RECORD lpBuffer, DWORD nLength, LPDWORD lpNumberOfEventsRead, USHORT wFlags)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapiAW(kernel32,ReadConsoleInputEx);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, ::PINPUT_RECORD, DWORD, LPDWORD, USHORT>(pFunc, hConsoleInput, lpBuffer, nLength, lpNumberOfEventsRead, wFlags);
		return result;
	}

	BOOL Kernel32::ReadConsoleOutput(HANDLE hConsoleOutput, ::PCHAR_INFO lpBuffer, ::COORD dwBufferSize, ::COORD dwBufferCoord, ::PSMALL_RECT lpReadRegion)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,ReadConsoleOutput);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, ::PCHAR_INFO, ::COORD, ::COORD, ::PSMALL_RECT>(pFunc, hConsoleOutput, lpBuffer, dwBufferSize, dwBufferCoord, lpReadRegion);
		return result;
	}

	BOOL Kernel32::ReadConsoleOutputAttribute(HANDLE hConsoleOutput, LPWORD lpAttribute, DWORD nLength, ::COORD dwReadCoord, LPDWORD lpNumberOfAttrsRead)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,ReadConsoleOutputAttribute);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, LPWORD, DWORD, ::COORD, LPDWORD>(pFunc, hConsoleOutput, lpAttribute, nLength, dwReadCoord, lpNumberOfAttrsRead);
		return result;
	}

	BOOL Kernel32::ReadConsoleOutputCharacter(HANDLE hConsoleOutput, LPTSTR lpCharacter, DWORD nLength, ::COORD dwReadCoord, LPDWORD lpNumberOfCharsRead)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,ReadConsoleOutputCharacter);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, LPTSTR, DWORD, ::COORD, LPDWORD>(pFunc, hConsoleOutput, lpCharacter, nLength, dwReadCoord, lpNumberOfCharsRead);
		return result;
	}

	HRESULT Kernel32::ReleasePseudoConsole(HPCON hPC)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,ReleasePseudoConsole);
		CheckReturn< HRESULT, LongSuccessCheck >::TType result = Library::Call<HRESULT, HPCON>(pFunc, hPC);
		return result;
	}

	HRESULT Kernel32::ResizePseudoConsole(HPCON hPC, COORD size)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,ResizePseudoConsole);
		CheckReturn< HRESULT, LongSuccessCheck >::TType result = Library::Call<HRESULT, HPCON, COORD>(pFunc, hPC, size);
		return result;
	}

	BOOL Kernel32::ScrollConsoleScreenBuffer(HANDLE hConsoleOutput, const ::SMALL_RECT* lpScrollRectangle, const ::SMALL_RECT* lpClipRectangle, ::COORD dwDestinationOrigin, const ::CHAR_INFO* lpFill)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,ScrollConsoleScreenBuffer);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, const ::SMALL_RECT*, const ::SMALL_RECT*, ::COORD, const ::CHAR_INFO*>(pFunc, hConsoleOutput, lpScrollRectangle, lpClipRectangle, dwDestinationOrigin, lpFill);
		return result;
	}

	BOOL Kernel32::SetConsoleActiveScreenBuffer(HANDLE hConsoleOutput)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,SetConsoleActiveScreenBuffer);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE>(pFunc, hConsoleOutput);
		return result;
	}

	BOOL Kernel32::SetConsoleCP(UINT wCodePageID)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,SetConsoleCP);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL,UINT>(pFunc, wCodePageID);
		return result;
	}

	BOOL Kernel32::SetConsoleCtrlHandler(PHANDLER_ROUTINE HandlerRoutine, BOOL Add)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,SetConsoleCtrlHandler);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, PHANDLER_ROUTINE, BOOL>(pFunc, HandlerRoutine, Add);
		return result;
	}

	BOOL Kernel32::SetConsoleCursorInfo(HANDLE hConsoleOutput, const ::CONSOLE_CURSOR_INFO* lpConsoleCursorInfo)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,SetConsoleCursorInfo);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, const ::CONSOLE_CURSOR_INFO*>(pFunc, hConsoleOutput, lpConsoleCursorInfo);
		return result;
	}

	BOOL Kernel32::SetConsoleCursorPosition(HANDLE hConsoleOutput, ::COORD dwCursorPosition)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,SetConsoleCursorPosition);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, ::COORD>(pFunc, hConsoleOutput, dwCursorPosition);
		return result;
	}

	BOOL Kernel32::SetConsoleDisplayMode(HANDLE hConsoleOutput, DWORD dwFlags, ::PCOORD lpNewScreenBufferDimensions)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,SetConsoleDisplayMode);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, DWORD, ::PCOORD>(pFunc, hConsoleOutput, dwFlags, lpNewScreenBufferDimensions);
		return result;
	}

	BOOL Kernel32::SetConsoleHistoryInfo(::PCONSOLE_HISTORY_INFO lpConsoleHistoryInfo)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,SetConsoleHistoryInfo);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call< BOOL, ::PCONSOLE_HISTORY_INFO >(pFunc, lpConsoleHistoryInfo);
		return result;
	}

	BOOL Kernel32::SetConsoleMode(HANDLE hConsoleHandle, DWORD dwMode)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,SetConsoleMode);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, DWORD>(pFunc, hConsoleHandle, dwMode);
		return result;
	}

	BOOL Kernel32::SetConsoleOutputCP(UINT wCodePageID)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,SetConsoleOutputCP);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, UINT>(pFunc, wCodePageID);
		return result;
	}

	BOOL Kernel32::SetConsoleScreenBufferInfoEx(HANDLE hConsoleOutput, ::PCONSOLE_SCREEN_BUFFER_INFOEX lpConsoleScreenBufferInfoEx)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,SetConsoleScreenBufferInfoEx);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, ::PCONSOLE_SCREEN_BUFFER_INFOEX>(pFunc, hConsoleOutput, lpConsoleScreenBufferInfoEx);
		return result;
	}

	BOOL Kernel32::SetConsoleScreenBufferSize(HANDLE hConsoleOutput, ::COORD dwSize)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,SetConsoleScreenBufferSize);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, ::COORD>(pFunc, hConsoleOutput, dwSize);
		return result;
	}

	BOOL Kernel32::SetConsoleTextAttribute(HANDLE hConsoleOutput, WORD wAttributes)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,SetConsoleTextAttribute);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, WORD>(pFunc, hConsoleOutput, wAttributes);
		return result;
	}

	BOOL Kernel32::SetConsoleTitle(LPCTSTR lpConsoleTitle)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,SetConsoleTitle);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, LPCTSTR>(pFunc, lpConsoleTitle);
		return result;
	}

	BOOL Kernel32::SetConsoleWindowInfo(HANDLE hConsoleOutput, BOOL bAbsolute, const ::SMALL_RECT* lpConsoleWindow)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,SetConsoleWindowInfo);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, BOOL, const ::SMALL_RECT*>(pFunc, hConsoleOutput, bAbsolute, lpConsoleWindow);
		return result;
	}

	BOOL Kernel32::SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, ::PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,SetCurrentConsoleFontEx);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call< BOOL, HANDLE, BOOL, ::PCONSOLE_FONT_INFOEX >(pFunc, hConsoleOutput, bMaximumWindow, lpConsoleCurrentFontEx);
		return result;
	}

	BOOL Kernel32::SetStdHandle(DWORD nStdHandle, HANDLE hHandle)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,SetStdHandle);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, DWORD, HANDLE>(pFunc, nStdHandle, hHandle);
		return result;
	}

	BOOL Kernel32::WriteConsoleA(HANDLE hConsoleOutput, const VOID* lpBuffer, DWORD nNumberOfCharsToWrite, LPDWORD lpNumberOfCharsWritten, void* lpReserved)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,WriteConsoleA);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, const void*, DWORD, LPDWORD, void*>(pFunc, hConsoleOutput, lpBuffer, nNumberOfCharsToWrite, lpNumberOfCharsWritten, lpReserved);
		return result;
	}

	BOOL Kernel32::WriteConsoleW(HANDLE hConsoleOutput, const VOID* lpBuffer, DWORD nNumberOfCharsToWrite, LPDWORD lpNumberOfCharsWritten, void* lpReserved)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,WriteConsoleW);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, const VOID*, DWORD, LPDWORD, void*>(pFunc, hConsoleOutput, lpBuffer, nNumberOfCharsToWrite, lpNumberOfCharsWritten, lpReserved);
		return result;
	}

	BOOL Kernel32::WriteConsoleInput(HANDLE hConsoleInput, const ::INPUT_RECORD* lpBuffer, DWORD nLength, LPDWORD lpNumberOfEventsWritten)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,WriteConsoleInput);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, const ::INPUT_RECORD*, DWORD, LPDWORD>(pFunc, hConsoleInput, lpBuffer, nLength, lpNumberOfEventsWritten);
		return result;
	}

	BOOL Kernel32::WriteConsoleOutput(HANDLE hConsoleOutput, const ::CHAR_INFO* lpBuffer, ::COORD dwBufferSize, ::COORD dwBufferCoord, ::PSMALL_RECT lpWriteRegion)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,WriteConsoleOutput);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, const ::CHAR_INFO*, ::COORD, COORD, ::PSMALL_RECT>(pFunc, hConsoleOutput, lpBuffer, dwBufferSize, dwBufferCoord, lpWriteRegion);
		return result;
	}

	BOOL Kernel32::WriteConsoleOutputAttribute(HANDLE hConsoleOutput, const WORD* lpAttribute, DWORD nLength, ::COORD dwWriteCoord, LPDWORD lpNumberOfAttrsWritten)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,WriteConsoleOutputAttribute);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, const WORD*, DWORD, ::COORD, LPDWORD>(pFunc, hConsoleOutput, lpAttribute, nLength, dwWriteCoord, lpNumberOfAttrsWritten);
		return result;
	}

	BOOL Kernel32::WriteConsoleOutputCharacter(HANDLE hConsoleOutput, LPCTSTR lpCharacter, DWORD nLength, ::COORD dwWriteCoord, LPDWORD lpNumberOfCharsWritten)
	{
		qor_pp_fcontext;
		Win32ErrorHandler _;
		qor_pp_useswinapi(kernel32,WriteConsoleOutputCharacter);
		CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, LPCTSTR, DWORD, ::COORD, LPDWORD>(pFunc, hConsoleOutput, lpCharacter, nLength, dwWriteCoord, lpNumberOfCharsWritten);
		return result;
	}

	DWORD Kernel32::CtrlRoutine(LPVOID lpParameter)
	{
		qor_pp_fcontext;		
		DWORD dwResult = 0;
#if ( _WIN32_WINNT >= 0x0600 )
		qor_pp_unref(lpParameter);
		dwResult = 0;//::CtrlRoutine( lpParameter );
#	else
		qor_pp_unref(lpParameter);
#	endif
		return dwResult;
	}

}}}//qor::win::api
