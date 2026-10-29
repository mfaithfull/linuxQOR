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

    HWINEVENTHOOK User32::SetWinEventHook(UINT eventMin, UINT eventMax, HMODULE hmodWinEventProc, WINEVENTPROC lpfnWinEventProc, DWORD idProcess, DWORD idThread, UINT dwflags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetWinEventHook);
        return Library::Call<HWINEVENTHOOK, UINT, UINT, HMODULE, WINEVENTPROC, DWORD, DWORD, UINT>(pFunc, eventMin, eventMax, hmodWinEventProc, lpfnWinEventProc, idProcess, idThread, dwflags);
    }

    BOOL User32::UnhookWinEvent(HWINEVENTHOOK hWinEventHook)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UnhookWinEvent);
        return Library::Call<BOOL, HWINEVENTHOOK>(pFunc, hWinEventHook);
    }

    BOOL User32::IsWinEventHookInstalled(DWORD dwEvent)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsWinEventHookInstalled);
        return Library::Call<BOOL, DWORD>(pFunc, dwEvent);
    }

    //NotifyWinEvent is a single user32.dll export - winuser.h declares it directly with no A/W family and no
    //stem -> stemW macro, so the bare stem is the name GetProcAddress is given.

    void User32::NotifyWinEvent(DWORD dwEvent, HWND hwnd, LONG idObject, LONG idChild)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, NotifyWinEvent);
        return Library::voidCall<DWORD, HWND, LONG, LONG>(pFunc, dwEvent, hwnd, idObject, idChild);
    }

}}}//qor::win::api
