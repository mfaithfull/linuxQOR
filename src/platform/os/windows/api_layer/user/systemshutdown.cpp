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

    BOOL User32::ExitWindowsEx(UINT uFlags, DWORD dwReason)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ExitWindowsEx);
        return Library::Call< BOOL, UINT, DWORD >(pFunc, uFlags, dwReason);
    }

    BOOL User32::LockWorkStation(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, LockWorkStation);
        return Library::Call< BOOL >(pFunc);
    }

    BOOL User32::ShutdownBlockReasonCreate(HWND hWnd, LPCWSTR pwszReason)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ShutdownBlockReasonCreate);
        return Library::Call< BOOL, HWND, LPCWSTR >(pFunc, hWnd, pwszReason);
    }

    BOOL WINAPI User32::ShutdownBlockReasonDestroy(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ShutdownBlockReasonDestroy);
        return Library::Call< BOOL, HWND >(pFunc, hWnd);
    }

    BOOL WINAPI User32::ShutdownBlockReasonQuery(HWND hWnd, LPWSTR pwszBuff, DWORD* pcchBuff)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ShutdownBlockReasonQuery);
        return Library::Call< BOOL, HWND, LPWSTR, DWORD* >(pFunc, hWnd, pwszBuff, pcchBuff);
    }

}}}//qor::win::api
