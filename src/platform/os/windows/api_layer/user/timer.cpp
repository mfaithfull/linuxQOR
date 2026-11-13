// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "../kernel/errorhandler.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //KillTimer and SetTimer are single user32.dll exports - winuser.h declares them directly with no A/W
    //family and no stem -> stemW macro, so the bare stem is the name GetProcAddress is given.

    //Return policy (U2, table row KillTimer / timer.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the
    //function fails, the return value is zero. To get extended error information, call GetLastError ." - the answer
    //here is the timer the caller already owns and names by uIDEvent, so the BOOL is that status word and zero is
    //reserved for failure; the session-27 soundness test (a checked-failure site is only sound where the SDK's failure
    //value is not also a legal success value) passes at every value, and BOOL + zero-means-failure takes the BoolCheck
    //the library already has (returncheck.h:94, Test at :99 is value ? true : false) - no new check class. 1 in-tree
    //caller, re-read before this token moved: ui/gui/controllers/windowcontroller.cpp:817 WindowController::KillTimer
    //returns this BOOL as its own bool, so the new throw replaces that caller's false path with a continuable issue
    //the Win32ErrorHandler above reports; the wrapper still forwards both arguments as the caller wrote them.
    BOOL User32::KillTimer(HWND hWnd, UINT_PTR uIDEvent)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, KillTimer);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, UINT_PTR>(pFunc, hWnd, uIDEvent);
        return bResult;
    }

    //Return policy (U2, table row SetTimer / timer.cpp, 12 Nov 2026): the row reads checked with checkClass CheckNonZero
    //< UINT_PTR >, and the table's verbatim docQuote is "Type: UINT_PTR If the function succeeds and the hWnd parameter
    //is NULL , the return value is an integer identifying the new timer. An application can pass this value to the
    //KillTimer function to destroy the timer. If the function succeeds and the hWnd parameter is not NULL , then the
    //return value is a nonzero integer. An application can pass the value of the nIDEvent parameter to the KillTimer
    //function to destroy the timer. If the function fails to create a timer, the return value is zero. To get extended
    //error information, call GetLastError ." - the page reserves exactly one value, zero, and only for "fails to create
    //a timer", while every success sentence gives the timer id, so the session-27 soundness test passes at every value
    //on the row's own reading (the note records the census resolving no sdkRetType here because winuser.h declares the
    //alias UPTR, which is why the class base is the published UINT_PTR this wrapper holds); that numeric row takes the
    //CheckNonZero template (returncheck.h:187, Test at :192 is (value == 0) ? false : true) the vocabulary already
    //offers - no new check class. 1 in-tree caller, re-read before this token moved: ui/gui/controllers/windowcontroller
    //cpp:822 WindowController::SetTimer hands this id straight to its own caller as an unsigned long long, so a caller
    //that tested the old zero for "no timer" now gets a continuable issue the Win32ErrorHandler above reports instead;
    //the wrapper still forwards all four arguments as the caller wrote them.
    UINT_PTR User32::SetTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetTimer);
        CheckReturn< UINT_PTR, CheckNonZero< UINT_PTR > >::TType uPtrResult = Library::Call<UINT_PTR, HWND, UINT_PTR, UINT, TIMERPROC>(pFunc, hWnd, nIDEvent, uElapse, lpTimerFunc);
        return uPtrResult;
    }

}}}//qor::win::api
