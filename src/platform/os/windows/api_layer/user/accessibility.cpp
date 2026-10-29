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

    BOOL User32::KillTimer(HWND hWnd, UINT_PTR uIDEvent)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, KillTimer);
        return Library::Call<BOOL, HWND, UINT_PTR>( pFunc, hWnd, uIDEvent);
    }

    UINT_PTR User32::SetTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetTimer);
        return Library::Call<UINT_PTR, HWND, UINT_PTR, UINT, TIMERPROC>(pFunc, hWnd, nIDEvent, uElapse, lpTimerFunc);
    }

}}}//qor::win::api
