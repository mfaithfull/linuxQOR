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

    BOOL User32::ClientToScreen(HWND hWnd, ::LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDCEx);
        return Library::Call< BOOL, HWND, ::LPPOINT >(pFunc, hWnd, lpPoint);
    }

    BOOL User32::ScreenToClient(HWND hWnd, ::LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDCEx);
        return Library::Call< BOOL, HWND, ::LPPOINT >(pFunc, hWnd, lpPoint);
    }

    int User32::MapWindowPoints(HWND hWndFrom, HWND hWndTo, ::LPPOINT lpPoints, UINT cPoints)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDCEx);
        return Library::Call< int, HWND, HWND, ::LPPOINT, UINT >(pFunc, hWndFrom, hWndTo, lpPoints, cPoints);
    }

}}}//qor::win::api
