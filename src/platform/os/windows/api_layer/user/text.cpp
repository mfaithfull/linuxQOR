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

    int User32::DrawTextT(HDC hDC, LPCTSTR lpString, int nCount, ::LPRECT lpRect, UINT uFormat)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DrawText);
        return Library::Call< int, HDC, LPCTSTR, int, ::LPRECT, UINT >(pFunc, hDC, lpString, nCount, lpRect, uFormat);
    }

    int User32::DrawTextExT(HDC hdc, LPTSTR lpchText, int cchText, ::LPRECT lprc, UINT dwDTFormat, ::LPDRAWTEXTPARAMS lpDTParams)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DrawTextEx);
        return Library::Call< int, HDC, LPTSTR, int, ::LPRECT, UINT, ::LPDRAWTEXTPARAMS >(pFunc, hdc, lpchText, cchText, lprc, dwDTFormat, lpDTParams);
    }

    DWORD User32::GetTabbedTextExtentT(HDC hDC, LPCTSTR lpString, int nCount, int nTabPositions, CONST LPINT lpnTabStopPositions)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetTabbedTextExtent);
        return Library::Call< DWORD, HDC, LPCTSTR, int, int, CONST LPINT >(pFunc, hDC, lpString, nCount, nTabPositions, lpnTabStopPositions);
    }

    LONG User32::TabbedTextOutT(HDC hDC, int X, int Y, LPCTSTR lpString, int nCount, int nTabPositions, CONST::LPINT lpnTabStopPositions, int nTabOrigin)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, TabbedTextOut);
        return Library::Call< LONG, HDC, int, int, LPCTSTR, int, int, CONST ::LPINT, int >(pFunc, hDC, X, Y, lpString, nCount, nTabPositions, lpnTabStopPositions, nTabOrigin);
    }

}}}//qor::win::api
