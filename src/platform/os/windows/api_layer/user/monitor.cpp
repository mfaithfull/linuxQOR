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

    BOOL User32::EnumDisplayMonitors(HDC hdc, LPCRECT lprcClip, ::MONITORENUMPROC lpfnEnum, LPARAM dwData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EnumDisplayMonitors);
        return Library::Call< BOOL, HDC, LPCRECT, ::MONITORENUMPROC, LPARAM>(pFunc, hdc, lprcClip, lpfnEnum, dwData);
    }

    BOOL User32::GetMonitorInfoT(HMONITOR hMonitor, ::LPMONITORINFO lpmi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetMonitorInfo);
        return Library::Call<BOOL, HMONITOR, ::LPMONITORINFO>(pFunc, hMonitor, lpmi);
    }

    HMONITOR User32::MonitorFromPoint(::POINT pt, DWORD dwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, MonitorFromPoint);
        return Library::Call<HMONITOR, ::POINT, DWORD>(pFunc, pt, dwFlags);
    }

    HMONITOR User32::MonitorFromRect(LPCRECT lprc, DWORD dwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, MonitorFromRect);
        return Library::Call<HMONITOR, LPCRECT, DWORD>(pFunc, lprc, dwFlags);
    }

    HMONITOR User32::MonitorFromWindow(HWND hwnd, DWORD dwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, MonitorFromWindow);
        return Library::Call<HMONITOR, HWND, DWORD>(pFunc, hwnd, dwFlags);
    }

}}}//qor::win::api
