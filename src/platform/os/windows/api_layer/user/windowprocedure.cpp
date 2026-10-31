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

    LRESULT User32::CallWindowProcT(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CallWindowProc);
        return Library::Call<LRESULT,WNDPROC, HWND, UINT, WPARAM, LPARAM>(pFunc, lpPrevWndFunc, hWnd, Msg, wParam, lParam);
    }

    LRESULT User32::DefWindowProcT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DefWindowProc);
        return Library::Call<LRESULT, HWND, UINT, WPARAM, LPARAM>(pFunc, hWnd, Msg, wParam, lParam);
    }

}}}//qor::win::api
