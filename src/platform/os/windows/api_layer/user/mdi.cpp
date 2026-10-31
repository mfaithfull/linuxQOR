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

    HWND User32::CreateMDIWindowT(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HINSTANCE hInstance, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CreateMDIWindow);
        return Library::Call< HWND, LPCTSTR, LPCTSTR, DWORD, int, int, int, int, HWND, HINSTANCE, LPARAM >(pFunc, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hInstance, lParam);
    }

    LRESULT User32::DefFrameProcT(HWND hWnd, HWND hWndMDIClient, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DefFrameProc);
        return Library::Call< LRESULT, HWND, HWND, INT, WPARAM, LPARAM >(pFunc, hWnd, hWndMDIClient, uMsg, wParam, lParam);
    }

    LRESULT User32::DefMDIChildProcT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DefMDIChildProc);
        return Library::Call< LRESULT, HWND, UINT, WPARAM, LPARAM >(pFunc, hWnd, uMsg, wParam, lParam);
    }

    BOOL User32::TranslateMDISysAccel(HWND hWndClient, LPMSG lpMsg)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, TranslateMDISysAccel);
        return Library::Call< BOOL, HWND, LPMSG >(pFunc, hWndClient, lpMsg);
    }

}}}//qor::win::api
