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
    //:8788, SetScrollInfo :12052, SetScrollPos :8816. Four parameter names differed from the SDK's (wSBFlags vs
    //wSBflags, fnBar vs nBar twice, fRedraw vs redraw) until the folder-wide parameter-name alignment of 6 Nov 2026
    //renamed all four to the SDK spelling in header and definition together - 0 name divergence remains (U18 step 4).
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
    //side - the settled policy for this layer, not a per-TU fix. Its ../returncheck.h include (then :11) used none of it
    // - one of the 35 dead includes, removed under T7 on 5 Nov 2026.
    //
    //U2 pass (12 Nov 2026, LargeTUBatchRegionChecks / LargeTUBatchL1 item 3 "scrollbar sites"): that include is back
    //and this pass is why it is live - the 7 rows the table reads checked (GetScrollBarInfo, GetScrollRange, ScrollDC,
    //ScrollWindow, ScrollWindowEx, SetScrollRange, ShowScrollBar) build CheckReturn over their Call, and the other 5
    //(EnableScrollBar, GetScrollInfo, GetScrollPos, SetScrollInfo, SetScrollPos) keep the SDK value with a written reason
    //at the site. Census after the pass: CheckReturn-code=7 == table-checked=7, returncheck-includes=1. Of the seven,
    //exactly three have live in-tree callers - ui/gui/controllers/s_s/scrollbarcontroller.cpp:67 GetScrollRange, :82
    //SetScrollRange and :87 ShowScrollBar, each "return User32::<stem>(...)" inside a bool ScrollBarController facade -
    //so landing BoolCheck there replaces a FALSE those facades used to return with a throw, the behaviour change D51
    //accepts for a row whose page reserves zero for failure; GetScrollBarInfo, ScrollDC, ScrollWindow and ScrollWindowEx
    //have no caller anywhere in the tree (published surface, D14). The five unchecked rows are all called from that same
    //facade (:52 EnableScrollBar, :57 GetScrollInfo, :62 GetScrollPos, :72 SetScrollInfo, :77 SetScrollPos), which is
    //precisely why they stay token-free: each hands the call's answer to its caller, and a check would turn a legal
    //zero into a throw.

    //Return policy (U2, table row EnableScrollBar / scrollbar.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, and
    //the table's verbatim docQuote is "Type: BOOL If the arrows are enabled or disabled as specified, the return value is nonzero. If the
    //arrows are already in the requested state or an error occurs, the return value is zero." - the page puts a legal no-change outcome on
    //the SAME value as an error, so zero answers "already in that state" and nothing is separable from failure; keying BoolCheck would
    //report a legal no-change as a throw. The value comes back exactly as the SDK produced it. Audit:
    //ui/gui/controllers/s_s/scrollbarcontroller.cpp:52 returns this BOOL to its caller as a bool.
    BOOL User32::EnableScrollBar(HWND hWnd, UINT wSBflags, UINT wArrows)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EnableScrollBar);
        return Library::Call<BOOL, HWND, UINT, UINT>(pFunc, hWnd, wSBflags, wArrows);
    }

    //Return policy (U2, table row GetScrollBarInfo / scrollbar.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, and
    //the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the
    //return value is zero. To get extended error information, call GetLastError ." - the PSCROLLBARINFO the caller passes is both the
    //request and the out-parameter, so the scrollbar metrics answer through it and the BOOL carries nothing besides whether the call
    //happened; zero is therefore reserved, the session-27 soundness test passes at every value, and sdkRetType is BOOL so BoolCheck
    //(returncheck.h:94) covers it - no new check class. Audit: no caller anywhere in the tree (the U19 census reads this stem at 0),
    //so the row is change-safe (D14).
    BOOL User32::GetScrollBarInfo(HWND hwnd, LONG idObject, ::PSCROLLBARINFO psbi)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetScrollBarInfo);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, LONG, ::PSCROLLBARINFO>(pFunc, hwnd, idObject, psbi);
        return bResult;
    }

    //Return policy (U2, table row GetScrollInfo / scrollbar.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, and the
    //table's verbatim docQuote is "Type: BOOL If the function retrieved any values, the return value is nonzero. If the function does
    //not retrieve any values, the return value is zero. To get extended error information, call GetLastError ." - the BOOL reports
    //whether a bar carries scroll information at all, and a bar that carries none is a legal state answering zero, so nothing is
    //reserved and the session-27 test fails at that value; the values the query yields answer through the LPSCROLLINFO out-parameter.
    //Audit: ui/gui/controllers/s_s/scrollbarcontroller.cpp:57 returns this BOOL to its caller as a bool.
    BOOL User32::GetScrollInfo(HWND hwnd, int nBar, ::LPSCROLLINFO lpsi)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetScrollInfo);
        return Library::Call<BOOL, HWND, int, ::LPSCROLLINFO>(pFunc, hwnd, nBar, lpsi);
    }

    //Return policy (U2, table row GetScrollPos / scrollbar.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, and the
    //table's verbatim docQuote is "Type: int If the function succeeds, the return value is the current position of the scroll box. If the
    //function fails, the return value is zero." - the int IS the position the caller asked for, and the page works a range of "0 through
    //100", so the failure answer 0 is also an answer; no class fits and keying CheckNonZero< int > would report a legal position as a throw. Audit:
    //ui/gui/controllers/s_s/scrollbarcontroller.cpp:62 returns this int to its caller as the position.
    int User32::GetScrollPos(HWND hWnd, int nBar)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetScrollPos);
        return Library::Call<int, HWND, int>(pFunc, hWnd, nBar);
    }

    //Return policy (U2, table row GetScrollRange / scrollbar.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, and
    //the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the
    //return value is zero. To get extended error information, call GetLastError ." - the minimum and maximum the query yields answer
    //through the two LPINT out-parameters the caller passes, so this BOOL carries nothing besides whether the call happened and zero
    //is reserved; the session-27 soundness test passes at every value, and sdkRetType is BOOL so BoolCheck (returncheck.h:94) covers
    //it - no new check class. Audit: ui/gui/controllers/s_s/scrollbarcontroller.cpp:67 returns this BOOL to its caller as a bool, so a
    //failed range query now throws out of that facade instead of returning false.
    BOOL User32::GetScrollRange(HWND hWnd, int nBar, LPINT lpMinPos, LPINT lpMaxPos)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetScrollRange);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, int, LPINT, LPINT>(pFunc, hWnd, nBar, lpMinPos, lpMaxPos);
        return bResult;
    }

    //Return policy (U2, table row ScrollDC / scrollbar.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, and the
    //table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the
    //return value is zero. To get extended error information, call GetLastError ." - the device context, the scroll and clip RECTs and
    //the update region are all parameters, and the invalidated region answers through the LPRECT out-parameter the caller passes, so
    //this BOOL carries nothing besides whether the scroll happened and zero is reserved; the session-27 soundness test passes at every
    //value, and sdkRetType is BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. Audit: no caller anywhere in the
    //tree (the U19 census reads this stem at 0), so the row is change-safe (D14).
    BOOL User32::ScrollDC(HDC hDC, int dx, int dy, const ::RECT *lprcScroll, const ::RECT *lprcClip, HRGN hrgnUpdate, ::LPRECT lprcUpdate)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ScrollDC);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HDC, int, int, const ::RECT*, const ::RECT*, HRGN, ::LPRECT>(
            pFunc, hDC, dx, dy, lprcScroll, lprcClip, hrgnUpdate, lprcUpdate);
        return bResult;
    }

    //Return policy (U2, table row ScrollWindow / scrollbar.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, and
    //the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the
    //return value is zero. To get extended error information, call GetLastError ." - the same BOOL status wording as the ScrollDC row:
    //the window, the scroll amounts and the two RECTs are all parameters and the call answers through none of them, so the BOOL carries
    //nothing besides whether the scroll happened and zero is reserved;
    //sdkRetType is BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. Audit: no caller anywhere in the tree (the
    //U19 census reads this stem at 0), so the row is change-safe (D14) - the ScrollWindowEx row below is the Ex twin this TU's
    //header block contrasts.
    BOOL User32::ScrollWindow(HWND hWnd, int XAmount, int YAmount, const ::RECT* lpRect, const ::RECT* lpClipRect)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ScrollWindow);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, int, int, const ::RECT*, const ::RECT*>(pFunc, hWnd, XAmount, YAmount, lpRect, lpClipRect);
        return bResult;
    }

    //Return policy (U2, table row ScrollWindowEx / scrollbar.cpp, 12 Nov 2026): the row reads checked with checkClass CheckNonZero< int >, and
    //the table's verbatim docQuote is "Type: int If the function succeeds, the return value is SIMPLEREGION (rectangular invalidated region),
    //COMPLEXREGION (nonrectangular invalidated region; overlapping rectangles), or NULLREGION (no invalidated region). If the function fails,
    //the return value is ERROR." - the failure answer is the named constant ERROR, and the installed wingdi.h region-flag block defines it at
    //:118 ERROR 0 directly above :119 NULLREGION 1 / :120 SIMPLEREGION 2 / :121 COMPLEXREGION 3, so every legal answer this call can return
    //is nonzero and zero is reserved; the session-27 test passes at 0 and sdkRetType is int, so the class base is int and CheckNonZero (
    //returncheck.h:187, template< typename TNumeric > : public AutoErrorCheck< TNumeric >) covers it - no new check class. This is the Ex
    //twin of the ScrollWindow row above, whose page answers BOOL: here the value is the region, so the check keys on the type's zero rather
    //than on a BOOL. Audit: no caller anywhere in the tree (the U19 census reads this stem at 0), so the row is change-safe (D14).
    int User32::ScrollWindowEx(HWND hWnd, int dx, int dy, const ::RECT* prcScroll, const ::RECT* prcClip, HRGN hrgnUpdate, ::LPRECT prcUpdate, UINT flags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ScrollWindowEx);
        CheckReturn< int, CheckNonZero< int > >::TType nResult = Library::Call<int, HWND, int, int, const ::RECT*, const ::RECT*, HRGN, ::LPRECT, UINT>(pFunc,
            hWnd, dx, dy, prcScroll, prcClip, hrgnUpdate, prcUpdate, flags);
        return nResult;
    }

    //Return policy (U2, table row SetScrollInfo / scrollbar.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, and the
    //table's verbatim docQuote is the whole Return value section "Type: int The return value is the current position of the scroll box." -
    //the page names no failure value at all, so D51 step 2 (an API that reserves no return value for errors) reads no-check; the int is
    //the position the caller asked for and 0 is a legal position, so keying CheckNonZero< int > would report a legal position as a throw.
    //Audit: ui/gui/controllers/s_s/scrollbarcontroller.cpp:72 returns this int to its caller as the position.
    int User32::SetScrollInfo(HWND hwnd, int nBar, ::LPCSCROLLINFO lpsi, BOOL redraw)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetScrollInfo);
        return Library::Call<int, HWND, int, ::LPCSCROLLINFO, BOOL>(pFunc, hwnd, nBar, lpsi, redraw);
    }

    //Return policy (U2, table row SetScrollPos / scrollbar.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, and the
    //table's verbatim docQuote is "Type: int If the function succeeds, the return value is the previous position of the scroll box. If the
    //function fails, the return value is zero." - the int is the previous position the call answers with, and the same "0 through 100"
    //range the GetScrollPos row cites puts a legal previous position on the documented failure value, so nothing is separable from
    //failure; keying CheckNonZero< int > would report a box already at 0 as a throw. Audit:
    //ui/gui/controllers/s_s/scrollbarcontroller.cpp:77 returns this int to its caller as the position.
    int User32::SetScrollPos(HWND hWnd, int nBar, int nPos, BOOL bRedraw)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetScrollPos);
        return Library::Call<int, HWND, int, int, BOOL>(pFunc, hWnd, nBar, nPos, bRedraw);
    }

    //Return policy (U2, table row SetScrollRange / scrollbar.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, and
    //the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the
    //return value is zero. To get extended error information, call GetLastError ." - the range being set and the redraw flag are
    //parameters, so the BOOL carries nothing besides whether the set happened and zero is reserved; the session-27 soundness test
    //passes at every value, and sdkRetType is BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. Audit:
    //ui/gui/controllers/s_s/scrollbarcontroller.cpp:82 returns this BOOL to its caller as a bool, so a failed range set now throws
    //out of that facade instead of returning false.
    BOOL User32::SetScrollRange(HWND hWnd, int nBar, int nMinPos, int nMaxPos, BOOL bRedraw)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetScrollRange);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, int, int, int, BOOL>(pFunc, hWnd, nBar, nMinPos, nMaxPos, bRedraw);
        return bResult;
    }

    //Return policy (U2, table row ShowScrollBar / scrollbar.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, and
    //the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the
    //return value is zero. To get extended error information, call GetLastError ." - the bar and the show flag are parameters, so the
    //BOOL carries nothing besides whether the call happened and zero is reserved; the session-27 soundness test passes at every value,
    //and sdkRetType is BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. Deliberately unlike the EnableScrollBar
    //row above, whose page puts a legal no-change outcome on the same zero: here the only documented zero is a failed call. Audit:
    //ui/gui/controllers/s_s/scrollbarcontroller.cpp:87 returns this BOOL to its caller as a bool, so a failed show now throws out of
    //that facade instead of returning false.
    BOOL User32::ShowScrollBar(HWND hWnd, int wBar, BOOL bShow)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ShowScrollBar);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, int, BOOL>(pFunc, hWnd, wBar, bShow);
        return bResult;
    }

}}}//qor::win::api
