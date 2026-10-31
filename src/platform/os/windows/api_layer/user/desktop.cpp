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

    //The "Window Station and Desktop functions" section (user32.h:399-417): 18 declarations, 18 definitions here,
    //in the header's order, and each definition matches its declaration parameter-for-parameter
    //(tmp/_tmp_desktop_sigcmp.ps1). Nine of the entry points have an SDK A/W family (winuser.h:1396-1447
    //CreateDesktop/CreateDesktopEx, :1461-1478 OpenDesktop, :1492-1507 EnumDesktops, :1583-1600 CreateWindowStation,
    //:1605-1620 OpenWindowStation, :1625-1638 EnumWindowStations, :1711-1730 GetUserObjectInformation,
    //:1735-1752 SetUserObjectInformation), so those nine wrappers spell the binding qor_pp_useswinapiAW and the other
    //nine (CloseDesktop, CloseWindowStation, GetProcessWindowStation, GetThreadDesktop, OpenInputDesktop,
    //SetProcessWindowStation, SetThreadDesktop, SwitchDesktop, EnumDesktopWindows - all declared directly, no A/W
    //family, no stem -> stemW macro) use the bare qor_pp_useswinapi. cl /EP on this TU resolves "OpenDesktopW",
    //"CloseDesktop", ... and all 18 names are in the export table and resolve live through the library's own
    //GetModuleHandleW/GetProcAddress step (tmp/_tmp_desktop_probe.cpp, which also shows GetProcessWindowStation and
    //GetThreadDesktop returning live non-null handles).
    //Handle contracts: OpenDesktopT/CreateDesktopT/CreateDesktopExT/OpenInputDesktop return an HDESK and
    //CreateWindowStationT/OpenWindowStationT an HWINSTA. Both families are released by CloseDesktop/CloseWindowStation
    //in this same section, NOT by Kernel32::CloseHandle; the handles are returned raw and nothing here is RAII-wrapped
    //(open item U2), so a caller that drops one leaks a desktop or window station object.
    //Thread contracts: GetThreadDesktop/SetThreadDesktop/SwitchDesktop/OpenInputDesktop act on the calling thread or the
    //process window station. SetThreadDesktop fails if the calling thread has already created a window on that
    //desktop or installed a hook there, and SwitchDesktop is per-process, not per-thread.
    //Callbacks: EnumDesktopsT/EnumWindowStationsT take the SDK's generic DESKTOPENUMPROC/WINSTAENUMPROC (winuser.h:220-227),
    //which are CALLBACK (stdcall) pointers. Because the bindings here are always the W form, the name the callback
    //receives is always a wide LPWSTR whatever the caller's TCHAR setting is.
    //Section coverage: within this section every SDK entry point in the winuser.h NODESKTOP / NOWINDOWSTATION regions
    //(:1391-1448, :1456-1546, :1548-1661) is wrapped. The two user-object security calls the SDK declares next to
    //GetUserObjectInformation/SetUserObjectInformation - SetUserObjectSecurity (:1671-1674) and GetUserObjectSecurity
    //(:1679-1684, exports 375 and 1DB) - are declared in user32.h but in a different section, "//Authorization
    //functions" :657-659, where they are the section's only two members and are declared WITHOUT static; neither has a
    //definition in any TU in the folder, so a caller of either gets LNK2019 (open item U4, and the non-static shape is
    //open item U18). UserHandleGrantAccess (:14925-14928, export 3D8), the third user-object call, is declared at
    //user32.h:259 in the "Process and Thread functions" section and defined in processthread.cpp:41, not here.
    //No wrapper in this TU has a Win32ErrorHandler _; (0/18, G5) - settled library policy.

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

    HDESK User32::CreateDesktopT(LPCTSTR lpszDesktop, LPCTSTR lpszDevice, ::LPDEVMODE pDevmode, DWORD dwFlags, ACCESS_MASK dwDesiredAccess, ::LPSECURITY_ATTRIBUTES lpsa)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CreateDesktop);
        return Library::Call< HDESK, LPCTSTR, LPCTSTR, ::LPDEVMODE, DWORD, ACCESS_MASK, ::LPSECURITY_ATTRIBUTES >(pFunc, lpszDesktop, lpszDevice, pDevmode, dwFlags, dwDesiredAccess, lpsa);
    }

    HDESK User32::CreateDesktopExT(LPCTSTR lpszDesktop, LPCTSTR lpszDevice, ::LPDEVMODE pDevmode, DWORD dwFlags, ACCESS_MASK dwDesiredAccess, ::LPSECURITY_ATTRIBUTES lpsa, ULONG ulHeapSize, PVOID pvoid)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CreateDesktopEx);
        return Library::Call< HDESK, LPCTSTR, LPCTSTR, ::LPDEVMODE, DWORD, ACCESS_MASK, ::LPSECURITY_ATTRIBUTES, ULONG, PVOID >(pFunc, lpszDesktop, lpszDevice, pDevmode, dwFlags, dwDesiredAccess, lpsa, ulHeapSize, pvoid);
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
        return Library::Call< HDESK, DWORD >(pFunc, dwThreadId);
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

    HWINSTA User32::OpenWindowStationT(LPCTSTR lpszWinSta, BOOL fInherit, ACCESS_MASK dwDesiredAccess)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, OpenWindowStation);
        return Library::Call< HWINSTA, LPCTSTR, BOOL, ACCESS_MASK >(pFunc, lpszWinSta, fInherit, dwDesiredAccess);
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
