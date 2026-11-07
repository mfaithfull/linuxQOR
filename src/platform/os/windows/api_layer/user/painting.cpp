// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
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


    HDC User32::BeginPaint(HWND hWnd, ::LPPAINTSTRUCT lpPaint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, BeginPaint);
        return Library::Call<HDC, HWND, ::LPPAINTSTRUCT>(pFunc, hWnd, lpPaint);
    }

    BOOL User32::DrawAnimatedRects(HWND hwnd, int idAni, const::RECT* lprcFrom, const::RECT* lprcTo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DrawAnimatedRects);
        return Library::Call<BOOL, HWND, int, const ::RECT*, const ::RECT*>(pFunc, hwnd, idAni, lprcFrom, lprcTo);
    }

    BOOL User32::DrawCaption(HWND hwnd, HDC hdc, ::LPCRECT lprect, UINT flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DrawCaption);
        return Library::Call<BOOL, HWND, HDC, ::LPCRECT, UINT>(pFunc, hwnd, hdc, lprect, flags);
    }

    BOOL User32::DrawEdge(HDC hdc, ::LPRECT qrc, UINT edge, UINT grfFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DrawEdge);
        return Library::Call<BOOL, HDC, ::LPRECT, UINT, UINT>(pFunc, hdc, qrc, edge, grfFlags);
    }

    BOOL User32::DrawFocusRect(HDC hDC, const::RECT* lprc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DrawFocusRect);
        return Library::Call<BOOL, HDC, const ::RECT*>(pFunc, hDC, lprc);
    }

    BOOL User32::DrawFrameControl(HDC hdc, ::LPRECT lprc, UINT uType, UINT uState)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DrawFrameControl);
        return Library::Call<BOOL, HDC, ::LPRECT, UINT, UINT>(pFunc, hdc, lprc, uType, uState);
    }

    BOOL User32::DrawStateT(HDC hdc, HBRUSH hbrFore, ::DRAWSTATEPROC qfnCallBack, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT uFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DrawState);
        return Library::Call<BOOL, HDC, HBRUSH, DRAWSTATEPROC, LPARAM, WPARAM, int, int, int, int, UINT>(pFunc, hdc, hbrFore, qfnCallBack, lData, wData, x, y, cx, cy, uFlags);
    }

    BOOL User32::EndPaint(HWND hWnd, const::PAINTSTRUCT* lpPaint)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EndPaint);
        return Library::Call<BOOL, HWND, const ::PAINTSTRUCT*>(pFunc, hWnd, lpPaint);
    }

    int User32::ExcludeUpdateRgn(HDC hDC, HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ExcludeUpdateRgn);
        return Library::Call<int, HDC, HWND>(pFunc, hDC, hWnd);
    }

    BOOL User32::GetUpdateRect(HWND hWnd, ::LPRECT lpRect, BOOL bErase)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetUpdateRect);
        return Library::Call<BOOL, HWND, ::LPRECT, BOOL>(pFunc, hWnd, lpRect, bErase);
    }

    int User32::GetUpdateRgn(HWND hWnd, HRGN hRgn, BOOL bErase)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetUpdateRgn);
        return Library::Call<int, HWND, HRGN, BOOL>(pFunc, hWnd, hRgn, bErase);
    }

    HDC User32::GetWindowDC(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetWindowDC);
        return Library::Call<HDC, HWND>(pFunc, hWnd);
    }

    int User32::GetWindowRgn(HWND hWnd, HRGN hRgn)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetWindowRgn);
        return Library::Call<int, HWND, HRGN>(pFunc, hWnd, hRgn);
    }

    int User32::GetWindowRgnBox(HWND hWnd, ::LPRECT lprc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetWindowRgnBox);
        return Library::Call<int, HWND, ::LPRECT>(pFunc, hWnd, lprc);
    }

    BOOL User32::GrayStringT(HDC hDC, HBRUSH hBrush, ::GRAYSTRINGPROC lpOutputFunc, LPARAM lpData, int nCount, int X, int Y, int nWidth, int nHeight)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GrayString);
        return Library::Call<BOOL, HDC, HBRUSH, GRAYSTRINGPROC, LPARAM, int, int, int, int, int>(pFunc, hDC, hBrush, lpOutputFunc, lpData, nCount, X, Y, nWidth, nHeight);
    }

    BOOL User32::InvalidateRect(HWND hWnd, const::RECT* lpRect, BOOL bErase)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, InvalidateRect);
        return Library::Call<BOOL, HWND, const ::RECT*, BOOL>(pFunc, hWnd, lpRect, bErase);
    }

    BOOL User32::InvalidateRgn(HWND hWnd, HRGN hRgn, BOOL bErase)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, InvalidateRgn);
        return Library::Call<BOOL, HWND, HRGN, BOOL>(pFunc, hWnd, hRgn, bErase);
    }

    BOOL User32::LockWindowUpdate(HWND hWndLock)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, LockWindowUpdate);
        return Library::Call<BOOL, HWND>(pFunc, hWndLock);
    }

    BOOL User32::PaintDesktop(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, PaintDesktop);
        return Library::Call<BOOL, HDC>(pFunc, hdc);
    }

    BOOL User32::RedrawWindow(HWND hWnd, const::RECT* lprcUpdate, HRGN hrgnUpdate, UINT flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, RedrawWindow);
        return Library::Call<BOOL, HWND, const ::RECT*, HRGN, UINT>(pFunc, hWnd, lprcUpdate, hrgnUpdate, flags);
    }

    int User32::SetWindowRgn(HWND hWnd, HRGN hRgn, BOOL bRedraw)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetWindowRgn);
        return Library::Call<int, HWND, HRGN, BOOL>(pFunc, hWnd, hRgn, bRedraw);
    }

    BOOL User32::UpdateWindow(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UpdateWindow);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    BOOL User32::ValidateRect(HWND hWnd, const::RECT* lpRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ValidateRect);
        return Library::Call<BOOL, HWND, const ::RECT*>(pFunc, hWnd, lpRect);
    }

    BOOL User32::ValidateRgn(HWND hWnd, HRGN hRgn)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ValidateRgn);
        return Library::Call<BOOL, HWND, HRGN>(pFunc, hWnd, hRgn);
    }

    HWND User32::WindowFromDC(HDC hDC)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, WindowFromDC);
        return Library::Call<HWND, HDC>(pFunc, hDC);
    }

}}}//qor::win::api
