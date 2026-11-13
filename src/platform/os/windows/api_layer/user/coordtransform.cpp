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

    //The "Coordinate Transform functions" section (user32.h:377-379): three wrappers, three bindings, in header order.
    //All three entry points are single user32.dll exports with no A/W family and no stem -> stemW macro (winuser.h
    //declares them directly at :9508-9513, :9515-9520 and :9556-9563, and the export table has no *A/*W form of any of
    //them), so under D13 the bare qor_pp_useswinapi is the correct spelling and cl /EP resolves the literal names
    //GetProcAddress receives: ClientToScreen (ordinal 4D), ScreenToClient (30C), MapWindowPoints (27F).
    //Before this session all three wrappers bound GetDCEx instead - one binding line copied into three bodies. Each
    //call therefore entered GetDCEx(HWND, HRGN, DWORD): lpPoint went in as hrgnClip, dwFlags was whatever the third
    //argument register happened to hold, the returned HDC came back truncated to BOOL/int and no coordinate was ever
    //transformed. The binding name was a real export, so no export-table or /EP check could see it; only comparing the
    //bound stem against the member it sits in does (folder-wide pre-scan P5, tmp/_tmp_wrong_stem_scan.ps1).
    //Contracts: lpPoint(s) are _Inout_ and caller-owned - the call rewrites the caller's POINTs in place and allocates
    //nothing. hWnd and hWndFrom/hWndTo are _In_/_In_opt_ respectively (winuser.h:9560-9561: a NULL endpoint means the
    //desktop window). MapWindowPoints returns the non-zero delta it applied to negative coordinates, so 0 is a normal
    //success, not a failure code; ClientToScreen/ScreenToClient return FALSE for an invalid hWnd.
    //Section coverage: these are the three coordinate transforms the header puts in this section. The rest of the SDK's
    //coordinate-conversion family (winuser.h:9522-9554, :9565-9576 - LogicalToPhysicalPoint, PhysicalToLogicalPoint, the
    //two *ForPerMonitorDPI variants, WindowFromPoint, WindowFromPhysicalPoint) is declared in other sections of
    //user32.h; the two point converters that are defined live in window.cpp and declare a void return where the SDK
    //returns BOOL (open item U18).

    //Return policy (U2, table row ClientToScreen / coordtransform.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "If the function succeeds, the return value is nonzero. If the
    //function fails, the return value is zero." - the answer this stem produces is the lpPoint the caller owns and the
    //call rewrites in place, so the BOOL is that status word alone and zero is reserved for failure; the session-27
    //soundness test (a checked-failure site is only sound where the SDK's failure value is not also a legal success
    //value) passes at every value, and BOOL + zero-means-failure takes the BoolCheck the library already has
    //(returncheck.h:94, Test at :99 is value ? true : false) - no new check class. 1 in-tree caller, re-read before
    //this token moved: ui/gui/controllers/windowcontroller.cpp:643 WindowController::ClientToScreen folds this BOOL
    //into its own bool with "? true : false", so the new throw replaces that caller's false path with a continuable
    //issue the Win32ErrorHandler above reports; the wrapper still forwards both arguments as the caller wrote them.
    BOOL User32::ClientToScreen(HWND hWnd, ::LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ClientToScreen);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, ::LPPOINT >(pFunc, hWnd, lpPoint);
        return bResult;
    }

    //Return policy (U2, table row ScreenToClient / coordtransform.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "If the function succeeds, the return value is nonzero. If the
    //function fails, the return value is zero." - the answer this stem produces is the lpPoint the caller owns and the
    //call rewrites in place, so the BOOL is that status word alone and zero is reserved for failure; the session-27
    //soundness test (a checked-failure site is only sound where the SDK's failure value is not also a legal success
    //value) passes at every value, and BOOL + zero-means-failure takes the BoolCheck the library already has
    //(returncheck.h:94, Test at :99 is value ? true : false) - no new check class. 1 in-tree caller, re-read before
    //this token moved: ui/gui/controllers/windowcontroller.cpp:648 WindowController::ScreenToClient folds this BOOL
    //into its own bool with "? true : false", so the new throw replaces that caller's false path with a continuable
    //issue the Win32ErrorHandler above reports; the wrapper still forwards both arguments as the caller wrote them.
    BOOL User32::ScreenToClient(HWND hWnd, ::LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ScreenToClient);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, ::LPPOINT >(pFunc, hWnd, lpPoint);
        return bResult;
    }

    //Return policy (U2, table row MapWindowPoints / coordtransform.cpp, 12 Nov 2026): the row reads UNCHECKED with no
    //check class, and the table's verbatim docQuote ends "If the function fails, the return value is zero. Call
    //SetLastError prior to calling this method to differentiate an error return value from a legitimate "0" return
    //value." - the page itself declines to separate the failure value from the answer, which is the delta it applied
    //to the caller's POINTs, so the session-27 test fails at zero and a check would report a legal answer as a
    //failure: this site keeps 0 CheckReturn and hands the delta over exactly as the caller wrote it. 1 in-tree
    //caller, re-read before any token could move: ui/gui/controllers/windowcontroller.cpp:653
    //WindowController::MapPoints returns this int as its own int, so a check there would have turned a 0-delta
    //success into a reported failure.
    int User32::MapWindowPoints(HWND hWndFrom, HWND hWndTo, ::LPPOINT lpPoints, UINT cPoints)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, MapWindowPoints);
        return Library::Call< int, HWND, HWND, ::LPPOINT, UINT >(pFunc, hWndFrom, hWndTo, lpPoints, cPoints);
    }

}}}//qor::win::api
