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

    HDESK User32::OpenDesktopT(LPCTSTR lpszDesktop, DWORD dwFlags, BOOL fInherit, ACCESS_MASK dwDesiredAccess)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, OpenDesktop);
        return Library::Call< HDESK, LPCTSTR, DWORD, BOOL, ACCESS_MASK >(pFunc, lpszDesktop, dwFlags, fInherit, dwDesiredAccess);
    }

    BOOL User32::CloseDesktop(HDESK hDesktop)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CloseDesktop);
        return Library::Call< BOOL, HDESK >(pFunc, hDesktop);
    }

    HDESK User32::CreateDesktopT(LPCTSTR lpszDesktop, LPCTSTR lpszDevice, ::LPDEVMODE pDevMode, DWORD dwFlags, ACCESS_MASK dwDesiredAccess, ::LPSECURITY_ATTRIBUTES lpsa)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CreateDesktop);
        return Library::Call< HDESK, LPCTSTR, LPCTSTR, ::LPDEVMODE, DWORD, ACCESS_MASK, ::LPSECURITY_ATTRIBUTES >(pFunc, lpszDesktop, lpszDevice, pDevMode, dwFlags, dwDesiredAccess, lpsa);
    }

    HDESK User32::CreateDesktopExT(LPCTSTR lpszDesktop, LPCTSTR lpszDevice, ::LPDEVMODE pDevmode, DWORD dwFlags, ACCESS_MASK dwDesiredAccess, ::LPSECURITY_ATTRIBUTES lpsa, ULONG ulHeapSize, PVOID pvoid)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CreateDesktopEx);
        return Library::Call< HDESK, LPCTSTR, LPCTSTR, ::LPDEVMODE, DWORD, ACCESS_MASK, ::LPSECURITY_ATTRIBUTES, ULONG, PVOID >(pFunc, lpszDesktop, lpszDevice, pDevmode, dwFlags, dwDesiredAccess, lpsa, ulHeapSize, pvoid );
    }

    BOOL User32::EnumDesktopsT(HWINSTA hwinsta, DESKTOPENUMPROC lpEnumFunc, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, EnumDesktops);
        return Library::Call< BOOL, HWINSTA, DESKTOPENUMPROC, LPARAM >(pFunc, hwinsta, lpEnumFunc, lParam);
    }

    HWINSTA User32::CreateWindowStationT(LPCTSTR lpwinsta, DWORD dwFlags, ACCESS_MASK dwDesiredAccess, ::LPSECURITY_ATTRIBUTES lpsa)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CreateWindowStation);
        return Library::Call< HWINSTA, LPCTSTR, DWORD, ACCESS_MASK, ::LPSECURITY_ATTRIBUTES >(pFunc, lpwinsta, dwFlags, dwDesiredAccess, lpsa);
    }

    BOOL User32::CloseWindowStation(HWINSTA hWinSta)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CloseWindowStation);
        return Library::Call< BOOL, HWINSTA >(pFunc, hWinSta);
    }

    BOOL User32::EnumWindowStationsT(WINSTAENUMPROC lpEnumFunc, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, EnumWindowStations);
        return Library::Call< BOOL, WINSTAENUMPROC, LPARAM >(pFunc, lpEnumFunc, lParam);
    }

    HWINSTA User32::GetProcessWindowStation(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetProcessWindowStation);
        return Library::Call< HWINSTA >(pFunc);
    }

    HDESK User32::GetThreadDesktop(DWORD dwThreadId)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetThreadDesktop);
        return Library::Call< HDESK >(pFunc, dwThreadId );
    }

    BOOL User32::GetUserObjectInformationT(HANDLE hObj, int nIndex, PVOID pvInfo, DWORD nLength, LPDWORD lpnLengthNeeded)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetUserObjectInformation);
        return Library::Call< BOOL, HANDLE, int, PVOID, DWORD, LPDWORD >(pFunc, hObj, nIndex, pvInfo, nLength, lpnLengthNeeded);
    }

    HDESK User32::OpenInputDesktop(DWORD dwFlags, BOOL fInherit, ACCESS_MASK dwDesiredAccess)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, OpenInputDesktop);
        return Library::Call< HDESK, DWORD, BOOL, ACCESS_MASK >(pFunc, dwFlags, fInherit, dwDesiredAccess);
    }

    HWINSTA User32::OpenWindowStationT(LPTSTR lpszWinSta, BOOL fInherit, ACCESS_MASK dwDesiredAccess)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, OpenWindowStation);
        return Library::Call< HWINSTA >(pFunc, lpszWinSta, fInherit, dwDesiredAccess);
    }

    BOOL User32::SetProcessWindowStation(HWINSTA hWinSta)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetProcessWindowStation);
        return Library::Call< BOOL, HWINSTA >(pFunc, hWinSta);
    }

    BOOL User32::SetThreadDesktop(HDESK hDesktop)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetThreadDesktop);
        return Library::Call< BOOL, HDESK >(pFunc, hDesktop);
    }

    BOOL User32::SetUserObjectInformationT(HANDLE hObj, int nIndex, PVOID pvInfo, DWORD nLength)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SetUserObjectInformation);
        return Library::Call< BOOL, HANDLE, int, PVOID, DWORD >(pFunc, hObj, nIndex, pvInfo, nLength);
    }

    BOOL User32::SwitchDesktop(HDESK hDesktop)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SwitchDesktop);
        return Library::Call< BOOL, HDESK >(pFunc, hDesktop);
    }

    BOOL User32::EnumDesktopWindows(HDESK hDesktop, ::WNDENUMPROC lpfn, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EnumDesktopWindows);
        return Library::Call< BOOL, HDESK, ::WNDENUMPROC, LPARAM >(pFunc, hDesktop, lpfn, lParam);
    }

}}}//qor::win::api
