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
    //Twenty-five wrappers - the whole "user32.h:262 //Painting functions" section: declared :263-:287, all static,
    //defined here in header order (def :86 BeginPaint .. :254 WindowFromDC). 25/25 declarations pair with a definition
    //here and 0 parameter names diverge from the header, so this TU contributes 0 U4 (declared, never defined),
    //0 U11 (definition unqualified), 0 U13 (defined, non-static) and 0 U18 (return type contradicts the SDK)
    //instances - tmp/_tmp_paint_pre_s26.txt, tmp/_tmp_paint_ret_s26.txt.
    //
    //Binding (D13): 23 of the 25 stems are single exports with no A/W family anywhere, so the bare
    //qor_pp_useswinapi at :89, :96, :103, :110, :117, :124, :138, :145, :152, :159, :166, :173, :180, :194, :201,
    //:208, :215, :222, :229, :236, :243, :250 and :257 is the only spelling that can bind them - user32.dll
    //exports BeginPaint (11 / 0002E9D0), DrawAnimatedRects (CD / 0002EC50), DrawCaption (CE / 0007E860),
    //DrawEdge (D1 / 00095AC0), DrawFocusRect (D2 / 00003100), DrawFrameControl (D4 / 00095F30), EndPaint
    //(F4 / 0002ED80), ExcludeUpdateRgn (10E / 0002EDA0), GetUpdateRect (1D6 / 0001D9D0), GetUpdateRgn
    //(1D7 / 0008E530), GetWindowDC (1E2 / 0002F270), GetWindowRgn (1F3 / 00002990), GetWindowRgnBox
    //(1F4 / 0001A750), InvalidateRect (224 / 0002F420), InvalidateRgn (225 / 0002F430), LockWindowUpdate
    //(26B / 0002F510), PaintDesktop (2A6 / 0007E940), RedrawWindow (2DB / 0002F7F0), SetWindowRgn
    //(383 / 0001C590), UpdateWindow (3D4 / 0001D0A0), ValidateRect (3DF / 0002FF00), ValidateRgn (3E0 /
    //00037790) and WindowFromDC (3EE / 0002FF40) - tmp/_tmp_user32_exports.txt - and winuser.h declares each
    //exactly once with no #define for the stem (tmp/_tmp_paint_class_s26.ps1: macro=False 23/23, 0 P6-shaped,
    //0 /EP literals absent from the export table). Bare is therefore load-bearing, not stylistic.
    //
    //DrawState (member :128) and GrayString (member :184) are the section's only A/W families and the only two
    //members here that carry the folder's T suffix. Their bare names are NOT exported (DrawStateA D9 / 00079EA0,
    //DrawStateW DA / 00079FC0, GrayStringA 1FD / 0008E710, GrayStringW 1FE / 0008E770), so the
    //qor_pp_useswinapiAW at :131 and :187 is load-bearing and cl /EP emits the literals
    //"DrawStateW" and "GrayStringW"
    //(tmp/_tmp_paint_pp_pre_s26.txt). A bare spelling of those two would resolve only because winuser.h:8371-8375
    //and :8431-:8435 rewrite the stem before it is stringized (G1) - the P2 shape, which this TU already
    //avoids. The folder census counts all 25 bindings here as resolvable (428 bindings, 1 unresolvable, none in
    //this TU - tmp/_tmp_p1census_pre_s26.txt) and the wrong-stem scan has no painting.cpp row
    //(tmp/_tmp_wrongstem_s26.txt: 3 flags, all the known-benign cursor.cpp and resource.cpp ones).
    //
    //Signatures: 25 members against 27 SDK prototypes (DrawStateA/W and GrayStringA/W each pair with one
    //member), every return agreeing: BeginPaint winuser.h:8619 HDC, DrawAnimatedRects :3096 BOOL, DrawCaption
    //:3077 BOOL, DrawEdge :2984 BOOL, DrawFocusRect :9722 BOOL, DrawFrameControl :3047 BOOL, DrawStateA :8406 /
    //DrawStateW :8420 BOOL, EndPaint :8626 BOOL, ExcludeUpdateRgn :8682 int, GetUpdateRect :8633 BOOL,
    //GetUpdateRgn :8641 int, GetWindowDC :8606 HDC, GetWindowRgn :8664 int, GetWindowRgnBox :8673 int,
    //GrayStringA :8348 / GrayStringW :8361 BOOL, InvalidateRect :8689 BOOL, InvalidateRgn :8704 BOOL,
    //LockWindowUpdate :8760 BOOL, PaintDesktop :8522 BOOL, RedrawWindow :8720 BOOL, SetWindowRgn :8649 int,
    //UpdateWindow :8502 BOOL, ValidateRect :8697 BOOL, ValidateRgn :8712 BOOL, WindowFromDC :8563 HWND
    //(tmp/_tmp_paint_ret_s26.ps1 - the SDK puts the return type on its own line, so a declaration-line
    //instrument cannot see it: session 22's caveat). Two members used to rename SDK parameters (DrawCaption's
    //lprc / uFlags against the SDK's lprect / flags, DrawStateT's hbr / lpOutputFunc / fuFlags against hbrFore /
    //qfnCallBack / uFlags, BeginPaint's hwnd against the SDK's hWnd); all six were renamed to the SDK spelling by
    //the folder-wide parameter-name alignment of 6 Nov 2026 (U18 step 4), which is also what showed that the
    //folder's -eq-based name compare had been blind to the case-only ones like hwnd/hWnd all along.
    //
    //The two *T members are the only pair in the folder where the A/W choice is invisible in the types:
    //DrawStateA and DrawStateW declare the same opaque LPARAM lData / WPARAM wData and GrayStringA/W the same
    //lpData / nCount, so what is drawn travels inside an integer and the binding, not the signature, picks the
    //width (here W). Recorded against U26; nothing in this TU can be changed for it.
    //
    //U16 (version guards): 8 of the 27 SDK prototypes - 7 of the 25 members - sit behind a version guard: DrawEdge
    //:2984, DrawFrameControl :3047, DrawCaption :3077 and DrawAnimatedRects :3096 under one
    //#if(WINVER >= 0x0400) at :2923, DrawStateA/W under :8380, PaintDesktop :8522 under :8518 and GetWindowRgnBox
    //:8673 under #if(_WIN32_WINNT >= 0x0501) at :8668 - the item's first 0x0501 guard. The other 19 prototypes
    //have only the file-wide #ifndef NOUSER at :64 in front of them; all 27 are inside a
    //#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) region and none is PC_APP-only, and NOUSER cannot be
    //defined for this TU anyway because its parameter types (HWND, HDC, LPRECT, PAINTSTRUCT, DRAWSTATEPROC,
    //GRAYSTRINGPROC) come from those same headers (tmp/_tmp_paint_guards_s26.ps1).
    //
    //Call-site idiom: 25/25 carry qor_pp_fcontext; and 25/25 spell explicit Call<> template arguments
    //(0 argument-deducing sites; folder 425 explicit / 3 deducing) and all 25 use the tight Call< spelling, so
    //the spacing is internally uniform and left alone (tmp/_tmp_u14_pre_s26.txt). No Win32ErrorHandler and no
    //CheckReturn here, per the folder's settled return-check policy (U1/U2): BeginPaint, GetWindowDC and
    //WindowFromDC return handles and ExcludeUpdateRgn, GetUpdateRgn, GetWindowRgn, GetWindowRgnBox and
    //SetWindowRgn return counts, so a NULL or 0 cannot be told apart from a swallowed SEH fault (U7).
    //Session 26's only source edits are whitespace: the GetUpdateRect call at :153 spaced none of its arguments
    //while the other 24 sites do, and the ValidateRect call at :244 had a space before the closing paren. The
    //25 binding literals are unchanged (tmp/_tmp_paint_pp_pre_s26.txt vs tmp/_tmp_paint_pp_post_s26.txt).


    //Return policy (U2, table row BeginPaint / painting.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HDC > - docQuote "If the function succeeds, the return value is the handle to a display device
    //context for the specified window. If the function fails, the return value is NULL , indicating that no display
    //device context is available." so NULL is reserved for a failed call and the DC handle the caller wraps is never
    //legitimately NULL from this call; sdkRetType HDC so the base is HDC and CheckNonZeroHandle (returncheck.h:137,
    //template< typename THandle > : public AutoErrorCheck< THandle >) is already in the vocabulary - no new check class.
    //AUDIT: the table counts 1 in-tree caller for the exact stem and the site-level read found two spellings of it -
    //ui/gui/controllers/windowcontroller.cpp:520 (qualified) and ui/gui/painting/paintsession.cpp:41 (unqualified under
    //using namespace qor::win::api), both feeding the returned HDC straight to PerThread::DeviceContextFromHandle
    //without testing it - so with the check landed a NULL DC now raises a continuable issue the Win32ErrorHandler above
    //reports rather than travelling silently into a DeviceContext whose HWND member is 0 (U22).
    HDC User32::BeginPaint(HWND hWnd, ::LPPAINTSTRUCT lpPaint)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, BeginPaint);
        CheckReturn< HDC, CheckNonZeroHandle< HDC > >::TType hResult = Library::Call<HDC, HWND, ::LPPAINTSTRUCT>(pFunc, hWnd, lpPaint);
        return hResult;
    }

    //Return policy (U2, table row DrawAnimatedRects / painting.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - docQuote "If the function succeeds, the return value is nonzero. If the function fails, the return
    //value is zero." is the whole Return value section and the page names no legal zero, so zero is reserved for a
    //failed draw; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. AUDIT: 1 in-tree
    //caller, ui/gui/controllers/windowcontroller.cpp:527, which spells the wrapper's BOOL as
    //return User32::DrawAnimatedRects (...) ? true : false inside WindowController::DrawAnimatedRects - the caller
    //already reads zero as a plain false, so landing the check makes a failed animated draw raise a continuable
    //issue the Win32ErrorHandler above reports before that false is returned.
    BOOL User32::DrawAnimatedRects(HWND hwnd, int idAni, const::RECT* lprcFrom, const::RECT* lprcTo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DrawAnimatedRects);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, int, const ::RECT*, const ::RECT*>(pFunc, hwnd, idAni, lprcFrom, lprcTo);
        return bResult;
    }

    //Return policy (U2, table row DrawCaption / painting.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the DrawAnimatedRects row's docQuote word for word ("If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero."), so zero is reserved for a failed caption draw and the
    //caption's rectangle and flags are parameters; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new
    //check class. AUDIT: 1 in-tree caller, ui/gui/controllers/windowcontroller.cpp:532, the same
    //... ? true : false idiom inside WindowController::DrawCaption, so a failed caption draw now raises a continuable
    //issue the Win32ErrorHandler above reports before that false is returned.
    BOOL User32::DrawCaption(HWND hwnd, HDC hdc, ::LPCRECT lprect, UINT flags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DrawCaption);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, HDC, ::LPCRECT, UINT>(pFunc, hwnd, hdc, lprect, flags);
        return bResult;
    }

    //Return policy (U2, table row DrawEdge / painting.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the DrawAnimatedRects row's docQuote word for word ("If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero."), so zero is reserved for a failed edge draw and the
    //edge and grfFlags are parameters; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new check class.
    //0 in-tree callers for the exact stem (D14 published surface), so nothing built today reaches this site and the row
    //is change-safe; G6 keeps the check live on every call, so an edge draw that cannot run now raises a continuable
    //issue the Win32ErrorHandler above reports rather than travelling as a silent zero.
    BOOL User32::DrawEdge(HDC hdc, ::LPRECT qrc, UINT edge, UINT grfFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DrawEdge);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HDC, ::LPRECT, UINT, UINT>(pFunc, hdc, qrc, edge, grfFlags);
        return bResult;
    }

    //Return policy (U2, table row DrawFocusRect / painting.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the DrawAnimatedRects row's docQuote word for word ("If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero."), so zero is reserved for a failed focus draw and the
    //rectangle is a parameter; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. 0 in-tree
    //callers for the exact stem (D14 published surface), so nothing built today reaches this site and the row is change
    //safe; G6 keeps the check live on every call, so a focus draw that cannot run now raises a continuable issue the
    //Win32ErrorHandler above reports rather than travelling as a silent zero.
    BOOL User32::DrawFocusRect(HDC hDC, const::RECT* lprc)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DrawFocusRect);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HDC, const ::RECT*>(pFunc, hDC, lprc);
        return bResult;
    }

    //Return policy (U2, table row DrawFrameControl / painting.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the DrawAnimatedRects row's docQuote word for word ("If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero."), so zero is reserved for a failed frame draw and the
    //rectangle, uType and uState are parameters; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new
    //check class. 0 in-tree callers for the exact stem (D14 published surface), so nothing built today reaches this
    //site and the row is change-safe; G6 keeps the check live on every call, so a frame draw that cannot run now raises
    //a continuable issue the Win32ErrorHandler above reports rather than travelling as a silent zero.
    BOOL User32::DrawFrameControl(HDC hdc, ::LPRECT lprc, UINT uType, UINT uState)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DrawFrameControl);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HDC, ::LPRECT, UINT, UINT>(pFunc, hdc, lprc, uType, uState);
        return bResult;
    }

    //Return policy (U2, table row DrawState / painting.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the DrawAnimatedRects row's docQuote word for word ("If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero."), so zero is reserved for a failed state draw and what
    //is drawn travels inside the opaque lData / wData pair, the callback and the geometry being parameters; the row's
    //sdkRetType is NO-PROTOTYPE (the DrawStateA/W family the SDK declares as a #define pair), so the base is the
    //published BOOL and BoolCheck (returncheck.h:94) covers it - no new check class. 0 in-tree callers for the exact
    //stem (D14 published surface), so nothing built today reaches this site and the row is change-safe; G6 keeps the
    //check live on every call, so a state draw that cannot run now raises a continuable issue the Win32ErrorHandler
    //above reports rather than travelling as a silent zero.
    BOOL User32::DrawStateT(HDC hdc, HBRUSH hbrFore, ::DRAWSTATEPROC qfnCallBack, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT uFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, DrawState);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HDC, HBRUSH, DRAWSTATEPROC, LPARAM, WPARAM, int, int, int, int, UINT>(pFunc, hdc, hbrFore, qfnCallBack, lData, wData, x, y, cx, cy, uFlags);
        return bResult;
    }

    //Return policy (U2, table row EndPaint / painting.cpp, 12 Nov 2026): the row reads unchecked with no checkClass -
    //the page reserves nothing. The table's verbatim docQuote "The return value is always nonzero." is the whole
    //Return value section, so D51 step 2 (an API that reserves no return value for errors) reads no-check and no class
    //fits; landing CheckNonZero< BOOL > here would be a no-op check that can never fire, so the row is left unchecked
    //rather than given a check that cannot fail; deliberately unlike the InvalidateRgn row, which carries the same
    //wording and the same reading. AUDIT: the table counts 1 in-tree caller and the site-level read found two spellings
    //of it - ui/gui/controllers/windowcontroller.cpp:537 (return User32::EndPaint (...) ? true : false, the caller
    //reading the always-nonzero flag as a plain true) and ui/gui/painting/paintsession.cpp:47 in
    //PaintSession::~PaintSession, which discards the value outright - so no reading of the flag exists to key, and no
    //token is landed.
    BOOL User32::EndPaint(HWND hWnd, const::PAINTSTRUCT* lpPaint)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EndPaint);
        return Library::Call<BOOL, HWND, const ::PAINTSTRUCT*>(pFunc, hWnd, lpPaint);
    }

    //Return policy (U2, table row ExcludeUpdateRgn / painting.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - an
    //enumerated answer, not a status. The table's verbatim docQuote "The return value specifies the complexity of the excluded
    //region; it can be any one of the following values. Value Meaning COMPLEXREGION Region consists of more than one rectangle.
    //ERROR An error occurred. NULLREGION Region is empty. SIMPLEREGION Region is a single rectangle." - the return is the value
    //the caller branches on, and ERROR is a member of the enumeration the caller must already handle alongside the two region
    //states, so no single value is separable as failure; keying a class on ERROR or on zero would report the legal NULLREGION
    //empty-region answer as a failure. 0 in-tree callers (change-safe). No token is landed.

    int User32::ExcludeUpdateRgn(HDC hDC, HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ExcludeUpdateRgn);
        return Library::Call<int, HDC, HWND>(pFunc, hDC, hWnd);
    }

    //Return policy (U2, table row GetUpdateRect / painting.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - a
    //question about the window's update region, not a status. The table's verbatim docQuote "If the update region is not empty,
    //the return value is nonzero. If there is no update region, the return value is zero." - a window with nothing to repaint is
    //a legal state answering zero, so nothing is reserved and keying BoolCheck would report a legal empty update region as a
    //failure; the rectangle itself is an out-parameter. AUDIT: 1 in-tree caller, so the site-level read is owed before any token
    //moves - none is landed.

    BOOL User32::GetUpdateRect(HWND hWnd, ::LPRECT lpRect, BOOL bErase)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetUpdateRect);
        return Library::Call<BOOL, HWND, ::LPRECT, BOOL>(pFunc, hWnd, lpRect, bErase);
    }

    //Return policy (U2, table row GetUpdateRgn / painting.cpp, 12 Nov 2026): the row reads unchecked with no checkClass -
    //an enumerated answer, not a status. The table's verbatim docQuote "The return value indicates the complexity of the
    //resulting region; it can be one of the following values. Value Meaning COMPLEXREGION Region consists of more than one
    //rectangle. ERROR An error occurred. NULLREGION Region is empty. SIMPLEREGION Region is a single rectangle." - the
    //return is the value the caller branches on, and ERROR is a member of the enumeration the caller must already handle
    //alongside the two region states, so no single value is separable as failure; the same reading the ExcludeUpdateRgn
    //row takes. AUDIT: 1 in-tree caller, ui/gui/controllers/windowcontroller.cpp:547, which returns the value straight out
    //of WindowController::GetUpdateRgn with no test on it - so no reading exists to key, and no token is landed.
    int User32::GetUpdateRgn(HWND hWnd, HRGN hRgn, BOOL bErase)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetUpdateRgn);
        return Library::Call<int, HWND, HRGN, BOOL>(pFunc, hWnd, hRgn, bErase);
    }

    //Return policy (U2, table row GetWindowDC / painting.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HDC > - docQuote "If the function succeeds, the return value is a handle to a device context
    //for the specified window. If the function fails, the return value is NULL , indicating an error or an invalid hWnd
    //parameter." so NULL is reserved for a failed call and the DC handle the caller wraps is never legitimately NULL
    //from this call; the same class the BeginPaint row takes, CheckNonZeroHandle (returncheck.h:137, template< typename
    //THandle > : public AutoErrorCheck< THandle >) over the row's HDC rather than HandleCheck, which keys on
    //INVALID_HANDLE_VALUE; sdkRetType HDC so the base is HDC - no new check class. AUDIT: the table counts 2 in-tree
    //callers and the site-level read found one call spelling, ui/gui/controllers/windowcontroller.cpp:553 inside
    //WindowController::GetDC(), which stores the HDC, tests it against nullptr and only then builds the DeviceContext
    //from it - plus one textual mention at ui/gui/gdiobjects/devicecontext.cpp:74, a comment that compares that
    //GetDC()/ReleaseDC(NULL, hdc) shape to the GetWindowDC route rather than calling it. With the check landed, a NULL
    //DC now raises a continuable issue the Win32ErrorHandler above reports before the caller's own nullptr test runs.
    HDC User32::GetWindowDC(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetWindowDC);
        CheckReturn< HDC, CheckNonZeroHandle< HDC > >::TType hResult = Library::Call<HDC, HWND>(pFunc, hWnd);
        return hResult;
    }

    //Return policy (U2, table row GetWindowRgn / painting.cpp, 12 Nov 2026): the row reads unchecked with no checkClass -
    //an enumerated answer the caller branches on. The table's verbatim docQuote "The return value specifies the type of
    //the region that the function obtains. It can be one of the following values. NULLREGION The region is empty.
    //SIMPLEREGION The region is a single rectangle. COMPLEXREGION The region is more than one rectangle. ERROR The
    //specified window does not have a region, or an error occurred while retrieving the region." - ERROR names a window
    //with no region set, a legal state, so no value is separable as failure; the region itself is an out-parameter.
    //AUDIT: 1 in-tree caller, ui/gui/controllers/windowcontroller.cpp:564, which returns the value straight out of
    //WindowController::GetRgn with no test - no reading to key and no token landed.
    int User32::GetWindowRgn(HWND hWnd, HRGN hRgn)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetWindowRgn);
        return Library::Call<int, HWND, HRGN>(pFunc, hWnd, hRgn);
    }

    //Return policy (U2, table row GetWindowRgnBox / painting.cpp, 12 Nov 2026): the row reads unchecked with no checkClass -
    //the same enumerated answer the GetWindowRgn row takes, the only difference being that the box is returned in the
    //RECTANGLE out-parameter. The table's verbatim docQuote "The return value specifies the type of the region that the
    //function obtains. It can be one of the following values. Value Meaning COMPLEXREGION The region is more than one
    //rectangle. ERROR The specified window does not have a region, or an error occurred while attempting to return the
    //region. NULLREGION The region is empty. SIMPLEREGION The region is a single rectangle." - ERROR again names a window
    //with no region set, a legal state, so nothing is separable as failure; row type int. AUDIT: 1 in-tree caller,
    //ui/gui/controllers/windowcontroller.cpp:569, which returns the value straight out of WindowController::GetRgnBox
    //with no test - no reading to key and no token landed.
    int User32::GetWindowRgnBox(HWND hWnd, ::LPRECT lprc)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetWindowRgnBox);
        return Library::Call<int, HWND, ::LPRECT>(pFunc, hWnd, lprc);
    }

    //Return policy (U2, table row GrayString / painting.cpp, 12 Nov 2026): the row reads unchecked with no checkClass -
    //the documented zero mixes the caller's own callback answer with a real failure. The table's verbatim docQuote "If
    //the string is drawn, the return value is nonzero. If either the TextOut function or the application-defined output
    //function returned zero, or there was insufficient memory to create a memory bitmap for graying, the return value
    //is zero." - the application-defined GRAYSTRINGPROC the caller passes chooses its own zero to mean, so that zero is
    //not separable from the out-of-memory failure and keying BoolCheck would report the caller's own answer as a
    //failure; 0 in-tree callers (change-safe). No token is landed.
    BOOL User32::GrayStringT(HDC hDC, HBRUSH hBrush, ::GRAYSTRINGPROC lpOutputFunc, LPARAM lpData, int nCount, int X, int Y, int nWidth, int nHeight)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, GrayString);
        return Library::Call<BOOL, HDC, HBRUSH, GRAYSTRINGPROC, LPARAM, int, int, int, int, int>(pFunc, hDC, hBrush, lpOutputFunc, lpData, nCount, X, Y, nWidth, nHeight);
    }

    //Return policy (U2, table row InvalidateRect / painting.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the table's verbatim docQuote "If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero." is the whole Return value section and the page names no legal zero, so zero is
    //reserved for a failed invalidation; the rectangle, the window and the erase flag are parameters, so the BOOL carries
    //no answer of its own - deliberately unlike the GetUpdateRect row, whose zero answers the caller's emptiness question.
    //sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. AUDIT: 1 in-tree caller,
    //ui/gui/controllers/windowcontroller.cpp:574, which already spells the result as "return User32::InvalidateRect (...)
    //? true : false" inside WindowController::InvalidateRect - the caller reads zero as a plain false, so landing the
    //check makes a failed invalidation raise a continuable issue the Win32ErrorHandler above reports before that false
    //is returned.
    BOOL User32::InvalidateRect(HWND hWnd, const::RECT* lpRect, BOOL bErase)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, InvalidateRect);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, const ::RECT*, BOOL>(pFunc, hWnd, lpRect, bErase);
        return bResult;
    }

    //Return policy (U2, table row InvalidateRgn / painting.cpp, 12 Nov 2026): the row reads unchecked with no checkClass -
    //the page reserves nothing. The table's verbatim docQuote "The return value is always nonzero." is the whole Return
    //value section, so D51 step 2 (an API that reserves no return value for errors) reads no-check and no class fits;
    //landing CheckNonZero< BOOL > here would be a no-op check that can never fire, so the row is left unchecked rather
    //than given a check that cannot fail - the same reading the EndPaint row takes. AUDIT: 1 in-tree caller,
    //ui/gui/controllers/windowcontroller.cpp:579, which spells it "return User32::InvalidateRgn (...) ? true : false"
    //inside WindowController::InvalidateRgn - the caller reads the always-nonzero flag as a plain true, so no reading of
    //the flag exists to key and no token is landed.
    BOOL User32::InvalidateRgn(HWND hWnd, HRGN hRgn, BOOL bErase)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, InvalidateRgn);
        return Library::Call<BOOL, HWND, HRGN, BOOL>(pFunc, hWnd, hRgn, bErase);
    }

    //Return policy (U2, table row LockWindowUpdate / painting.cpp, 12 Nov 2026): the row reads unchecked with no checkClass -
    //the page names a legal state behind its zero. The table's verbatim docQuote "If the function succeeds, the return
    //value is nonzero. If the function fails, the return value is zero, indicating that an error occurred or another
    //window was already locked." - a second lock attempt while another window already holds the lock is a routine caller
    //state answering zero, so the zero is not separable from a call failure and keying BoolCheck would report a legal
    //already-locked state as a failure; the window to lock is a parameter. AUDIT: 1 in-tree caller,
    //ui/gui/controllers/windowcontroller.cpp:584, which spells it "return User32::LockWindowUpdate (WINDOW_HANDLE) ?
    //true : false" inside WindowController::LockUpdate() - the caller already treats the already-locked zero as a plain
    //false, which is the reading that makes the check wrong; no token is landed.
    BOOL User32::LockWindowUpdate(HWND hWndLock)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, LockWindowUpdate);
        return Library::Call<BOOL, HWND>(pFunc, hWndLock);
    }

    //Return policy (U2, table row PaintDesktop / painting.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the DrawAnimatedRects row's status wording word for word, "If the function succeeds, the return value
    //is nonzero. If the function fails, the return value is zero.", is the whole Return value section and the page names
    //no legal zero, so zero is reserved for a failed desktop paint; the desktop handle is a parameter and the wallpaper
    //is drawn through the device context, so the BOOL carries no answer; sdkRetType BOOL so BoolCheck (returncheck.h:94)
    //covers it - no new check class. AUDIT: 0 in-tree callers, so the landing is change-safe and no call site reads this
    //result today.
    BOOL User32::PaintDesktop(HDC hdc)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, PaintDesktop);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HDC>(pFunc, hdc);
        return bResult;
    }

    //Return policy (U2, table row RedrawWindow / painting.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the DrawAnimatedRects row's status wording word for word, "If the function succeeds, the return value
    //is nonzero. If the function fails, the return value is zero.", is the whole Return value section, so zero is reserved
    //for a failed redraw; the window, the rectangle, the region and the flags are all parameters and the repaint happens
    //through the device context, so the BOOL carries no answer; the Remarks qualify when the repaint happens and name no
    //legal zero; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. AUDIT: 1 in-tree caller,
    //windowcontroller.cpp:589 (WindowController::Redraw), which reads the result as a status with "? true : false" and
    //never treats a zero as a legal answer, so the landed check only adds the error report the caller already relies on.
    BOOL User32::RedrawWindow(HWND hWnd, const::RECT* lprcUpdate, HRGN hrgnUpdate, UINT flags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, RedrawWindow);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, const ::RECT*, HRGN, UINT>(pFunc, hWnd, lprcUpdate, hrgnUpdate, flags);
        return bResult;
    }

    //Return policy (U2, table row SetWindowRgn / painting.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZero< int > - plain status wording on a row whose type is int, "If the function succeeds, the return value
    //is nonzero. If the function fails, the return value is zero."; the region and the window are parameters, so the
    //return carries no answer and zero is reserved; the class base is the row's int (both sdkRetType and pubRetType), so
    //CheckNonZero< int > (returncheck.h:187) rather than BoolCheck, which would key at the wrong type; deliberately unlike
    //the GetWindowRgn row, whose int return is the region-complexity enumeration the caller branches on. AUDIT: 1 in-tree
    //caller, windowcontroller.cpp:594 (WindowController::SetRgn), which returns the int through to its own callers and
    //never reads a zero as a legal answer, so the landed check only adds the error report.
    int User32::SetWindowRgn(HWND hWnd, HRGN hRgn, BOOL bRedraw)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetWindowRgn);
        CheckReturn< int, CheckNonZero< int > >::TType iResult = Library::Call<int, HWND, HRGN, BOOL>(pFunc, hWnd, hRgn, bRedraw);
        return iResult;
    }

    //Return policy (U2, table row UpdateWindow / painting.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the DrawAnimatedRects row's status wording word for word, "If the function succeeds, the return value
    //is nonzero. If the function fails, the return value is zero.", is the whole Return value section; the window and the
    //device context are parameters and the repaint is drawn through the context, so the BOOL carries no answer and zero is
    //reserved; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. AUDIT: 1 in-tree caller,
    //windowcontroller.cpp:599 (WindowController::Update), which reads the result as a status through "? true : false" and
    //never treats a zero as a legal answer.
    BOOL User32::UpdateWindow(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, UpdateWindow);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND>(pFunc, hWnd);
        return bResult;
    }

    //Return policy (U2, table row ValidateRect / painting.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - plain status BOOL for a region-validating action, the DrawAnimatedRects row's status wording word for
    //word, "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero.", is
    //the whole Return value section; the rectangle and the window are parameters, so the return carries no answer and zero
    //is reserved; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. AUDIT: 1 in-tree caller,
    //windowcontroller.cpp:604 (WindowController::ValidateRect), which reads the result as a status through "? true : false".
    BOOL User32::ValidateRect(HWND hWnd, const::RECT* lpRect)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ValidateRect);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, const ::RECT*>(pFunc, hWnd, lpRect);
        return bResult;
    }

    //Return policy (U2, table row ValidateRgn / painting.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the ValidateRect row's wording word for word for a region-validating action, "If the function succeeds,
    //the return value is nonzero. If the function fails, the return value is zero." being the whole Return value section;
    //the region and the window are parameters, so the return carries no answer and zero is reserved; sdkRetType BOOL so
    //BoolCheck (returncheck.h:94) covers it - no new check class. AUDIT: 1 in-tree caller, windowcontroller.cpp:609
    //(WindowController::ValidateRgn), which reads the result as a status through "? true : false".
    BOOL User32::ValidateRgn(HWND hWnd, HRGN hRgn)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ValidateRgn);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, HRGN>(pFunc, hWnd, hRgn);
        return bResult;
    }

    //Return policy (U2, table row WindowFromDC / painting.cpp, 12 Nov 2026): the row reads unchecked with no checkClass -
    //answer-shaped handle with a legal NULL, "The return value is a handle to the window associated with the specified DC.
    //If no window is associated with the specified DC, the return value is NULL ." - a device context that belongs to no
    //window (a printer DC, or a DC the caller created without a window) is a legal state answering NULL, so nothing is
    //reserved and CheckNonZeroHandle< HWND > would report a legal context-less lookup as a failure; same shape as the
    //GetClipboardOwner and GetOpenClipboardWindow rows; row type HWND. AUDIT: 0 in-tree callers, so the reading is
    //change-safe and no call site reads this result today.
    HWND User32::WindowFromDC(HDC hDC)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, WindowFromDC);
        return Library::Call<HWND, HDC>(pFunc, hDC);
    }

}}}//qor::win::api
