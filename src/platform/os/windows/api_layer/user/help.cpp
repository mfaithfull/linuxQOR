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

#undef WinHelp

namespace qor { namespace win { namespace api {

    BOOL User32::SetWindowContextHelpId(HWND hwnd, DWORD dwContextHelpId)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetWindowContextHelpId);
        return Library::Call<BOOL, HWND, DWORD>(pFunc, hwnd, dwContextHelpId);
    }

    DWORD User32::GetWindowContextHelpId(HWND hwnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetWindowContextHelpId);
        return Library::Call<DWORD, HWND>(pFunc, hwnd);
    }

    BOOL User32::WinHelp(HWND hWndMain, LPCTSTR lpszHelp, UINT uCommand, ULONG_PTR dwData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, WinHelp);
        return Library::Call<BOOL, HWND, LPCTSTR, UINT, ULONG_PTR>(pFunc, hWndMain, lpszHelp, uCommand, dwData);
    }

}}}//qor::win::api
