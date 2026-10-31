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

    BOOL User32::DragDetect(HWND hwnd, ::POINT pt)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DragDetect);
        return Library::Call< BOOL, HWND, ::POINT >(pFunc, hwnd, pt);
    }

    HWND User32::GetCapture(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetCapture);
        return Library::Call< HWND >(pFunc);
    }

    UINT User32::GetDoubleClickTime(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetDoubleClickTime);
        return Library::Call< UINT >(pFunc);
    }

    int User32::GetMouseMovePointsEx(UINT cbSize, LPMOUSEMOVEPOINT lppt, LPMOUSEMOVEPOINT lpptBuf, int nBufPoints, DWORD resolution)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetMouseMovePointsEx);
        return Library::Call< int, UINT, LPMOUSEMOVEPOINT, LPMOUSEMOVEPOINT, int, DWORD >(pFunc, cbSize, lppt, lpptBuf, nBufPoints, resolution);
    }

    VOID User32::mouse_event(DWORD dwFlags, DWORD dx, DWORD dy, DWORD dwData, ULONG_PTR dwExtraInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, mouse_event);
        return Library::voidCall< DWORD, DWORD, DWORD, DWORD, ULONG_PTR >(pFunc, dwFlags, dx, dy, dwData, dwExtraInfo);
    }

    BOOL User32::ReleaseCapture(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, ReleaseCapture);
        return Library::Call< BOOL >(pFunc);
    }

    HWND User32::SetCapture(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SetCapture);
        return Library::Call< HWND, HWND >(pFunc, hWnd);
    }

    BOOL User32::SetDoubleClickTime(UINT uInterval)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SetDoubleClickTime);
        return Library::Call< BOOL, UINT >(pFunc, uInterval);
    }

    BOOL User32::SwapMouseButton(BOOL fSwap)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SwapMouseButton);
        return Library::Call< BOOL, BOOL >(pFunc, fSwap);
    }

    BOOL User32::TrackMouseEvent(LPTRACKMOUSEEVENT lpEventTrack)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, LPTRACKMOUSEEVENT);
        return Library::Call< BOOL >(pFunc, lpEventTrack);
    }

}}}//qor::win::api
