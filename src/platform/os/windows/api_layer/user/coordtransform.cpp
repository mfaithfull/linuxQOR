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

    BOOL User32::ClientToScreen(HWND hWnd, ::LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ClientToScreen);
        return Library::Call< BOOL, HWND, ::LPPOINT >(pFunc, hWnd, lpPoint);
    }

    BOOL User32::ScreenToClient(HWND hWnd, ::LPPOINT lpPoint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ScreenToClient);
        return Library::Call< BOOL, HWND, ::LPPOINT >(pFunc, hWnd, lpPoint);
    }

    int User32::MapWindowPoints(HWND hWndFrom, HWND hWndTo, ::LPPOINT lpPoints, UINT cPoints)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, MapWindowPoints);
        return Library::Call< int, HWND, HWND, ::LPPOINT, UINT >(pFunc, hWndFrom, hWndTo, lpPoints, cPoints);
    }

}}}//qor::win::api
