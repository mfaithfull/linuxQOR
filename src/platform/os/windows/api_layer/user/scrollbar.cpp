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

    BOOL User32::EnableScrollBar(HWND hWnd, UINT wSBFlags, UINT wArrows)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(User32,EnableScrollBar);
        return Library::Call<BOOL, HWND, UINT, UINT>(pFunc, hWnd, wSBFlags, wArrows);
    }

    BOOL User32::GetScrollBarInfo(HWND hwnd, LONG idObject, ::PSCROLLBARINFO psbi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(User32,GetScrollBarInfo);
        return Library::Call<BOOL, HWND, LONG, ::PSCROLLBARINFO>(pFunc, hwnd, idObject, psbi);
    }

    BOOL User32::GetScrollInfo(HWND hwnd, int fnBar, ::LPSCROLLINFO lpsi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(User32,GetScrollInfo);
        return Library::Call<BOOL, HWND, int, ::LPSCROLLINFO>(pFunc, hwnd, fnBar, lpsi);
    }

    int User32::GetScrollPos(HWND hWnd, int nBar)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(User32,GetScrollPos);
        return Library::Call<int, HWND, int>(pFunc, hWnd, nBar);
    }

    BOOL User32::GetScrollRange(HWND hWnd, int nBar, LPINT lpMinPos, LPINT lpMaxPos)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(User32,GetScrollRange);
        return Library::Call<BOOL, HWND, int, LPINT, LPINT>(pFunc, hWnd, nBar, lpMinPos, lpMaxPos);
    }

    BOOL User32::ScrollDC(HDC hDC, int dx, int dy, const ::RECT *lprcScroll, const ::RECT *lprcClip, HRGN hrgnUpdate, ::LPRECT lprcUpdate)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(User32,ScrollDC);
        return Library::Call<BOOL, HDC, int, int, const ::RECT*, const ::RECT*, HRGN, ::LPRECT>(
            pFunc, hDC, dx, dy, lprcScroll, lprcClip, hrgnUpdate, lprcUpdate);
    }

    BOOL User32::ScrollWindow(HWND hWnd, int XAmount, int YAmount, const ::RECT* lpRect, const ::RECT* lpClipRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(User32,ScrollWindow);
        return Library::Call<BOOL, HWND, int, int, const ::RECT*, const ::RECT*>(pFunc, hWnd, XAmount, YAmount, lpRect, lpClipRect);
    }

    int User32::ScrollWindowEx(HWND hWnd, int dx, int dy, const ::RECT* prcScroll, const ::RECT* prcClip, HRGN hrgnUpdate, ::LPRECT prcUpdate, UINT flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(User32,ScrollWindowEx);
        return Library::Call<int, HWND, int, int, const ::RECT*, const ::RECT*, HRGN, ::LPRECT, UINT>(pFunc,
            hWnd, dx, dy, prcScroll, prcClip, hrgnUpdate, prcUpdate, flags);
    }

    int User32::SetScrollInfo(HWND hwnd, int fnBar, ::LPCSCROLLINFO lpsi, BOOL fRedraw)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(User32,SetScrollInfo);
        return Library::Call<int, HWND, int, ::LPCSCROLLINFO, BOOL>(pFunc, hwnd, fnBar, lpsi, fRedraw);
    }

    int User32::SetScrollPos(HWND hWnd, int nBar, int nPos, BOOL bRedraw)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(User32,SetScrollPos);
        return Library::Call<int, HWND, int, int, BOOL>(pFunc, hWnd, nBar, nPos, bRedraw);
    }

    BOOL User32::SetScrollRange(HWND hWnd, int nBar, int nMinPos, int nMaxPos, BOOL bRedraw)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(User32,SetScrollRange);
        return Library::Call<BOOL, HWND, int, int, int, BOOL>(pFunc, hWnd, nBar, nMinPos, nMaxPos, bRedraw);
    }

    BOOL User32::ShowScrollBar(HWND hWnd, int wBar, BOOL bShow)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(User32,ShowScrollBar);
        return Library::Call<BOOL, HWND, int, BOOL>(pFunc, hWnd, wBar, bShow);
    }

}}}//qor::win::api
