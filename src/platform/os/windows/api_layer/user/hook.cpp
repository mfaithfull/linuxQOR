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

    BOOL User32::CallMsgFilterT(LPMSG lpMsg, int nCode)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CallMsgFilter);
        return Library::Call< BOOL, LPMSG, int >(pFunc, lpMsg, nCode);
    }

    LRESULT User32::CallNextHookEx(HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CallNextHookEx);
        return Library::Call< LRESULT, HHOOK, int, WPARAM, LPARAM >(pFunc, hhk, nCode, wParam, lParam);
    }

    HHOOK User32::SetWindowsHookExT(int idHook, HOOKPROC lpfn, HINSTANCE hMod, DWORD dwThreadId)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SetWindowsHookEx);
        return Library::Call< HHOOK, int, HOOKPROC, HINSTANCE, DWORD >(pFunc, idHook, lpfn, hMod, dwThreadId);
    }

    BOOL User32::UnhookWindowsHookEx(HHOOK hhk)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UnhookWindowsHookEx);
        return Library::Call< BOOL, HHOOK >(pFunc, hhk);
    }

}}}//qor::win::api
