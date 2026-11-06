// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //The four WinEvent entry points are single user32.dll exports - winuser.h declares them directly with no
    //A/W family and no stem -> stemW macro, and user32.h does not #undef these stems, so the bare stem is the
    //name GetProcAddress is given.

    //The callback is invoked by the system with the CALLBACK (__stdcall) convention, so the caller must supply a
    //function pointer of that convention; WINEVENTPROC is the SDK type (winuser.h:13734).
    HWINEVENTHOOK User32::SetWinEventHook(UINT eventMin, UINT eventMax, HMODULE hmodWinEventProc, WINEVENTPROC pfnWinEventProc, DWORD idProcess, DWORD idThread, UINT dwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetWinEventHook);
        return Library::Call<HWINEVENTHOOK, UINT, UINT, HMODULE, WINEVENTPROC, DWORD, DWORD, UINT>(pFunc, eventMin, eventMax, hmodWinEventProc, pfnWinEventProc, idProcess, idThread, dwFlags);
    }

    BOOL User32::UnhookWinEvent(HWINEVENTHOOK hWinEventHook)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UnhookWinEvent);
        return Library::Call<BOOL, HWINEVENTHOOK>(pFunc, hWinEventHook);
    }

    BOOL User32::IsWinEventHookInstalled(DWORD event)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsWinEventHookInstalled);
        return Library::Call<BOOL, DWORD>(pFunc, event);
    }

    void User32::NotifyWinEvent(DWORD event, HWND hwnd, LONG idObject, LONG idChild)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, NotifyWinEvent);
        return Library::voidCall<DWORD, HWND, LONG, LONG>(pFunc, event, hwnd, idObject, idChild);
    }

}}}//qor::win::api
