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

    //Twelve wrappers - the whole "user32.h:573 //ScrollBar functions" section: declared :574-:585, all static, all
    //defined here through the User32:: qualifier and in header order (in this TU: :70 EnableScrollBar,
    //:77 GetScrollBarInfo, :84 GetScrollInfo, :91 GetScrollPos, :98 GetScrollRange, :105 ScrollDC, :113 ScrollWindow,
    //:120 ScrollWindowEx, :128 SetScrollInfo, :135 SetScrollPos, :142 SetScrollRange, :149 ShowScrollBar). 12/12
    //declarations pair with a definition here, so the TU contributes 0 U4 (declared, never defined), 0 U11 (definition
    //unqualified), 0 U13 (defined, declared non-static), 0 U17 (A/W family without the folder's T suffix) and 0 U18
    //(return type contradicts the SDK): tmp/_tmp_scroll_names_s30.ps1 reports 0 header-vs-definition parameter-name
    //divergences over the twelve, and tmp/_tmp_sigcmp.ps1 -Tu scrollbar.cpp -First 574 -Last 585 reports 12/12
    //declaration==definition - return type, parameter types, names and Call<> arity.
    //
    //Binding (D13): all twelve stems are single exports with no A/W family and no winuser.h macro for the stem
    //(_tmp_scroll_regions_s30.ps1 prints 0 "#define <Stem>" sites for each of the twelve), so the bare
    //qor_pp_useswinapi at :73, :80, :87, :94, :101, :108, :116, :123, :131, :138, :145 and :152 is the only spelling that
    //can bind them at all - user32.dll exports EnableScrollBar (ED), GetScrollBarInfo (1BD), GetScrollInfo (1BE),
    //GetScrollPos (1BF), GetScrollRange (1C0), ScrollDC (30E), ScrollWindow (30F), ScrollWindowEx (310), SetScrollInfo
    //(362), SetScrollPos (363), SetScrollRange (364) and ShowScrollBar (397) in tmp/_tmp_user32_exports.txt, and
    //cl /EP over the TU emits exactly those twelve literals (tmp/_tmp_scroll_pp_pre_s30.txt from :453254): 12/12
    //present in the export table, 0 macro, hence 0 P1, 0 P2 (no macro to rewrite the name, G1) and 0 P6.
    //
    //Module token: these twelve sites read qor_pp_useswinapi(User32, ...) until 3 Nov 2026 (review session 30) - the
    //folder's only non-lowercase spelling, 12 of its 428 bindings; they now say user32 like the other 416. The macro
    //stringizes the token (kernel32.h:50), so each wrapper had been asking GetModuleHandleW for L"User32". Ground rule
    //G4 measured that lookup case-insensitive; tmp/_tmp_scroll_g4probe_s30.cpp re-measures it for these twelve entry
    //points on x64 and x86 - one HMODULE for user32/User32/USER32, the same FARPROC address for every name, and a bogus
    //module name resolves to nothing, so the probe does discriminate. cl /EP before and after differ in those twelve
    //literals and in nothing else (tmp/_tmp_scroll_neutral_s30.txt: 12 differing lines, all module literals, binding
    //literal multiset identical) - behaviour-neutral, as G4 says.
    //
    //Signatures: return types read from winuser.h with tmp/_tmp_scroll_ret_s30.ps1, which prints the line above the
    //stem - BOOL EnableScrollBar :8859, GetScrollBarInfo :14763, GetScrollInfo :12061, GetScrollRange :8842, ScrollDC
    //:8776, ScrollWindow :8766, SetScrollRange :8832, ShowScrollBar :8851; int GetScrollPos :8825, ScrollWindowEx
    //:8788, SetScrollInfo :12052, SetScrollPos :8816. Four parameter names differ from the SDK's (wSBFlags vs
    //wSBflags, fnBar vs nBar twice, fRedraw vs redraw) - recorded under U18, cosmetic, and header and definition agree
    //with each other verbatim, so nothing inside this library contradicts itself.
    //
    //SDK visibility (U16): 12/12 prototypes are WINAPI_PARTITION_DESKTOP only; 3 are version-gated - SetScrollInfo and
    //GetScrollInfo under #if(WINVER >= 0x0400) (winuser.h:12026), GetScrollBarInfo under #if(WINVER >= 0x0500)
    //(:13594) - 6 more sit in the #ifndef NOSCROLL feature block at :8811 and GetScrollBarInfo also in
    //#ifndef NOWINABLE at :13716, while ScrollWindow, ScrollDC and ScrollWindowEx carry only the partition guard
    //(tmp/_tmp_scroll_guards_s30.txt, full stacks in tmp/_tmp_scroll_stack_s30.txt). No suppression macro is defined
    //in this build, so all twelve pair with always-visible declarations here.
    //
    //Callers (U19): exactly one, qor::platform::win::ScrollBarController, which calls eight of the twelve at
    //ui/gui/controllers/s_s/scrollbarcontroller.cpp:52, :57, :62, :67, :72, :77, :82 and :87; GetScrollBarInfo,
    //ScrollDC, ScrollWindow and ScrollWindowEx have no caller anywhere in the tree, which is published surface, not a
    //finding (D14). 0 SDK-bypass spellings folder-wide for this family - the census over the 48 stem spellings is
    //8 LAYER / 8 same-named-member-definition rows (session 28's false-positive class) / 0 BYPASS
    //(tmp/_tmp_scroll_pre_s30.txt:167-:185, tmp/_tmp_scroll_pre_s30b.txt).
    //
    //Return policy (U1/U2): 0 Win32ErrorHandler and 0 CheckReturn in all twelve wrappers, and none of the twelve SDK
    //prototypes carries _Check_return_ (tmp/_tmp_s30_measure2.txt), so a discarded failure leaves no trace on either
    //side - the settled policy for this layer, not a per-TU fix. :11 includes ../returncheck.h and uses none of it (one
    //of the 35 dead includes; left in place).

    BOOL User32::EnableScrollBar(HWND hWnd, UINT wSBFlags, UINT wArrows)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EnableScrollBar);
        return Library::Call<BOOL, HWND, UINT, UINT>(pFunc, hWnd, wSBFlags, wArrows);
    }

    BOOL User32::GetScrollBarInfo(HWND hwnd, LONG idObject, ::PSCROLLBARINFO psbi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetScrollBarInfo);
        return Library::Call<BOOL, HWND, LONG, ::PSCROLLBARINFO>(pFunc, hwnd, idObject, psbi);
    }

    BOOL User32::GetScrollInfo(HWND hwnd, int fnBar, ::LPSCROLLINFO lpsi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetScrollInfo);
        return Library::Call<BOOL, HWND, int, ::LPSCROLLINFO>(pFunc, hwnd, fnBar, lpsi);
    }

    int User32::GetScrollPos(HWND hWnd, int nBar)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetScrollPos);
        return Library::Call<int, HWND, int>(pFunc, hWnd, nBar);
    }

    BOOL User32::GetScrollRange(HWND hWnd, int nBar, LPINT lpMinPos, LPINT lpMaxPos)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetScrollRange);
        return Library::Call<BOOL, HWND, int, LPINT, LPINT>(pFunc, hWnd, nBar, lpMinPos, lpMaxPos);
    }

    BOOL User32::ScrollDC(HDC hDC, int dx, int dy, const ::RECT *lprcScroll, const ::RECT *lprcClip, HRGN hrgnUpdate, ::LPRECT lprcUpdate)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ScrollDC);
        return Library::Call<BOOL, HDC, int, int, const ::RECT*, const ::RECT*, HRGN, ::LPRECT>(
            pFunc, hDC, dx, dy, lprcScroll, lprcClip, hrgnUpdate, lprcUpdate);
    }

    BOOL User32::ScrollWindow(HWND hWnd, int XAmount, int YAmount, const ::RECT* lpRect, const ::RECT* lpClipRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ScrollWindow);
        return Library::Call<BOOL, HWND, int, int, const ::RECT*, const ::RECT*>(pFunc, hWnd, XAmount, YAmount, lpRect, lpClipRect);
    }

    int User32::ScrollWindowEx(HWND hWnd, int dx, int dy, const ::RECT* prcScroll, const ::RECT* prcClip, HRGN hrgnUpdate, ::LPRECT prcUpdate, UINT flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ScrollWindowEx);
        return Library::Call<int, HWND, int, int, const ::RECT*, const ::RECT*, HRGN, ::LPRECT, UINT>(pFunc,
            hWnd, dx, dy, prcScroll, prcClip, hrgnUpdate, prcUpdate, flags);
    }

    int User32::SetScrollInfo(HWND hwnd, int fnBar, ::LPCSCROLLINFO lpsi, BOOL fRedraw)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetScrollInfo);
        return Library::Call<int, HWND, int, ::LPCSCROLLINFO, BOOL>(pFunc, hwnd, fnBar, lpsi, fRedraw);
    }

    int User32::SetScrollPos(HWND hWnd, int nBar, int nPos, BOOL bRedraw)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetScrollPos);
        return Library::Call<int, HWND, int, int, BOOL>(pFunc, hWnd, nBar, nPos, bRedraw);
    }

    BOOL User32::SetScrollRange(HWND hWnd, int nBar, int nMinPos, int nMaxPos, BOOL bRedraw)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetScrollRange);
        return Library::Call<BOOL, HWND, int, int, int, BOOL>(pFunc, hWnd, nBar, nMinPos, nMaxPos, bRedraw);
    }

    BOOL User32::ShowScrollBar(HWND hWnd, int wBar, BOOL bShow)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ShowScrollBar);
        return Library::Call<BOOL, HWND, int, BOOL>(pFunc, hWnd, wBar, bShow);
    }

}}}//qor::win::api
