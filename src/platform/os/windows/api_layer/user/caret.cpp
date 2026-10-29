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

    BOOL User32::CreateCaret(HWND hWnd, HBITMAP hBitmap, int nWidth, int nHeight)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CreateCaret);
        return Library::Call< BOOL, HWND, HBITMAP, int, int >(pFunc, hWnd, hBitmap, nWidth, nHeight);
    }

    BOOL User32::DestroyCaret(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DestroyCaret);
        return Library::Call< BOOL >(pFunc);
    }

    UINT User32::GetCaretBlinkTime(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetCaretBlinkTime);
        return Library::Call< UINT >(pFunc);
    }

    BOOL User32::GetCaretPos(LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetCaretPos);
        return Library::Call< BOOL, LPPOINT >(pFunc, lpPoint);
    }

    BOOL User32::HideCaret(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, HideCaret);
        return Library::Call< BOOL, HWND >(pFunc, hWnd);
    }

    BOOL User32::SetCaretBlinkTime(UINT uMSeconds)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetCaretBlinkTime);
        return Library::Call< BOOL, UINT >(pFunc, uMSeconds);
    }

    BOOL User32::SetCaretPos(int X, int Y)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetCaretPos);
        return Library::Call< BOOL, int, int >(pFunc, X, Y);
    }

    BOOL User32::ShowCaret(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ShowCaret);
        return Library::Call< BOOL, HWND >(pFunc, hWnd);
    }

}}}//qor::win::api
