// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    LONG User32::ChangeDisplaySettingsT(::LPDEVMODE lpDevMode, DWORD dwflags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, ChangeDisplaySettings);
        return Library::Call< LONG, ::LPDEVMODE, DWORD >(pFunc, lpDevMode, dwflags);
    }

    LONG User32::ChangeDisplaySettingsExT(LPCTSTR lpszDeviceName, ::LPDEVMODE lpDevMode, HWND hwnd, DWORD dwflags, LPVOID lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, ChangeDisplaySettingsEx);
        return Library::Call< LONG, LPCTSTR, ::LPDEVMODE, HWND, DWORD, LPVOID >(pFunc, lpszDeviceName, lpDevMode, hwnd, dwflags, lParam);
    }

    BOOL User32::EnumDisplayDevicesT(LPCTSTR lpDevice, DWORD iDevNum, ::PDISPLAY_DEVICE lpDisplayDevice, DWORD dwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, EnumDisplayDevices);
        return Library::Call< BOOL, LPCTSTR, DWORD, ::PDISPLAY_DEVICE, DWORD >(pFunc, lpDevice, iDevNum, lpDisplayDevice, dwFlags);
    }

    BOOL User32::EnumDisplaySettingsT(LPCTSTR lpszDeviceName, DWORD iModeNum, ::LPDEVMODE lpDevMode)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, EnumDisplaySettings);
        return Library::Call< BOOL, LPCTSTR, DWORD, ::LPDEVMODE >(pFunc, lpszDeviceName, iModeNum, lpDevMode);
    }

    BOOL User32::EnumDisplaySettingsExT(LPCTSTR lpszDeviceName, DWORD iModeNum, ::LPDEVMODE lpDevMode, DWORD dwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, EnumDisplaySettingsEx);
        return Library::Call< BOOL, LPCTSTR, DWORD, ::LPDEVMODE, DWORD >(pFunc, lpszDeviceName, iModeNum, lpDevMode, dwFlags);
    }

    HDC User32::GetDC(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDC);
        return Library::Call< HDC, HWND >(pFunc, hWnd );
    }

    HDC User32::GetDCEx(HWND hWnd, HRGN hrgnClip, DWORD flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDCEx);
        return Library::Call< HDC, HWND, HRGN, DWORD >(pFunc, hWnd, hrgnClip, flags);
    }

    int User32::ReleaseDC(HWND hWnd, HDC hDC)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ReleaseDC);
        return Library::Call< int, HWND, HDC >(pFunc, hWnd, hDC);
    }


}}}//qor::win::api
