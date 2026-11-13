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
    //Ten wrappers - the whole "user32.h:555 //Mouse functions" section: declared :556-:565, all static, defined here
    //in header order (:97 DragDetect .. :160 TrackMouseEvent). The section pairs cleanly - 10/10
    //decl == def and 0 parameter-name divergences (tmp/_tmp_mouse_pre_s25.txt, tmp/_tmp_namediv_pre_s25.txt), so this
    //TU contributes 0 U4 (declared, never defined), 0 U11 (definition unqualified) and 0 U13 (defined, non-static).
    //
    //Binding (D13): every one of these ten stems is a SINGLE export and none of them has an A/W family. user32.dll
    //exports DragDetect (ordinal 1711, hint CB, RVA 0002EC30), GetCapture (1800/125/0002B4B0), GetDoubleClickTime
    //(1844/152/0002EF00), GetMouseMovePointsEx (1904/18E/0002F030), mouse_event (2577/3F5/0007ED00), ReleaseCapture
    //(2269/2FE/000040E0), SetCapture (2301/320/0002F9E0), SetDoubleClickTime (2325/339/0008EEF0), SwapMouseButton
    //(2426/3A4/0008F0C0) and TrackMouseEvent (2441/3B3/0002FE40) - tmp/_tmp_user32_exports.txt has no <stem>A and no
    //<stem>W row for any of them, and winuser.h declares each exactly once with no #define for the stem at all
    //(tmp/_tmp_mouse_class_s25.ps1: macro=False 10/10, P6-shaped 0). The bare qor_pp_useswinapi at :100, :107,
    //:114, :121, :128, :135, :142, :149, :156 and :163 is therefore not a style preference: for these ten names it
    //is the only spelling that can ever bind, because G1's stem -> stemW macro does not exist for any of them.
    //
    //What this TU looked like before session 25 (2 Nov 2026): all ten bindings were qor_pp_useswinapiAW - the mirror
    //image of the P2 class. AW pastes the suffix itself, _NAME##W at kernel32.h:52 (W, because qor_pp_unicode is on),
    //without consulting the SDK at all, so the TU asked for DragDetectW ... LPTRACKMOUSEEVENTW. cl /EP over the
    //pre-fix TU emits exactly those ten literals at pp lines 453254,453261,...,453317 of
    //tmp/_tmp_mouse_pp_pre_s25.txt; a live GetProcAddress probe reports MISSING for all ten and RESOLVED for all ten
    //bare stems at the export-table RVAs above, plus MISSING for TrackMouseEventW, TrackMouseEventA, TrackMouseEventT
    //and DragDetectA (tmp/_tmp_mouse_probe_s25.cpp -> tmp/_tmp_mouse_probe_s25.txt; the probe had to LoadLibraryW
    //first - GetModuleHandleW(L"user32") fails with gle=126 in a bare console process, which is G5). pFunc is a
    //function-local static const, so the NULL was cached for the life of the process (U5) and each of the ten threw
    //"Missing library function." from library.h:50-53 on every call. Those were 10 of the folder's 11 unresolvable
    //bindings (P1); the folder census now reads 428 bindings, 1 unresolvable - window.cpp:124 - against 11 before
    //(tmp/_tmp_p1census_pre_s25.txt, tmp/_tmp_p1census_post_s25.txt).
    //
    //P5's second instance went in the same pass: the old binding line bound the parameter TYPE,
    //qor_pp_useswinapiAW(user32, LPTRACKMOUSEEVENT), instead of the member's stem, so even a bare binding there would
    //have asked GetProcAddress for a type name. tmp/_tmp_wrong_stem_scan.ps1 folder-wide: 4 flags before this TU, 3
    //after, and the 3 survivors are the known-benign ones (cursor.cpp:64 = the P3 CopyCursorT -> CopyIcon alias;
    //resource.cpp:19 and :26 = U11's unqualified definitions), i.e. the folder now has 0 live wrong-stem sites.
    //
    //U14(b): the TrackMouseEvent call site at :164 was Call< BOOL >(pFunc, lpEventTrack) - one of the folder's
    //four argument-deducing sites - and now states the type it passes. This TU is 10/10 spaced
    //(tmp/_tmp_u14_pre_s25.txt), so the spaced form keeps local style rather than adding churn. Folder-wide: 424
    //explicit / 4 deducing before, 425 / 3 after, the survivors being window.cpp:118,254,317.
    //
    //Signatures: field-for-field against the SDK, parameter names included - GetMouseMovePointsEx
    //winuser.h:1353-1361, TrackMouseEvent :2770-2774, SwapMouseButton :3643-3647, GetDoubleClickTime :4273-4277,
    //SetDoubleClickTime :4279-4283, mouse_event :6102-6110, GetCapture :6961-6965, SetCapture :6967-6971,
    //ReleaseCapture :6973-6977 and DragDetect :8152-8157. Two SDK details worth recording: SetDoubleClickTime's
    //prototype gives its UINT no name at all (:4283), so "uInterval" is this layer's own, and DragDetect takes
    //POINT by value exactly as the SDK does (:8157). After the U6 fix Library::Call builds its __stdcall fPtr from
    //the very template arguments the call site spells (library.h:43-63), so the by-value POINT and mouse_event's
    //ULONG_PTR travel through a pointer type that matches the export on x64 and on x86 alike.
    //
    //U16: 3 of the 10 prototypes are version-guarded - GetMouseMovePointsEx under #if(WINVER >= 0x0500) (opened
    //winuser.h:1329, closed :1366), DragDetect under #if(WINVER >= 0x0400) (:8107, closed :8162) and TrackMouseEvent
    //inside two nested #if(_WIN32_WINNT >= 0x0400) (:2743 outer, :2758 inner, closed :2779 and :2784). The SDK's own
    //#endif comments in that stretch are crossed: :2756 closes the #ifndef NOTRACKMOUSEEVENT opened at :2744, so
    //that switch gates only the TME_* constants at :2746-2752, not the struct and not the call - read the guard
    //stack, not the comments (tmp/_tmp_mouse_guards_s25.ps1 -> tmp/_tmp_mouse_guards_s25.txt). GetCapture,
    //SetCapture and ReleaseCapture additionally sit in the DESKTOP | PC_APP partition block (:6892) while the other
    //seven are DESKTOP-only. No build-wide WINVER or _WIN32_WINNT pin exists in this tree, so these ten declarations
    //are unconditional while three of the prototypes they pair with are not. Recorded, nothing changed (U16).
    //
    //Contracts: mouse_event (:125) has no return value at all, so it has no failure signal. Three returns
    //are states rather than success flags - SwapMouseButton's BOOL says whether the buttons WERE swapped,
    //GetCapture's NULL says "this window does not own the capture", DragDetect's FALSE says "no drag happened" - and
    //the other six are failure-indicating: FALSE from ReleaseCapture and SetDoubleClickTime and TrackMouseEvent,
    //NULL from SetCapture, 0 from GetDoubleClickTime and -1 from GetMouseMovePointsEx. Two of those take a cbSize
    //the wrapper cannot validate: GetMouseMovePointsEx wants sizeof(MOUSEMOVEPOINT) and TrackMouseEvent is _Inout_
    //(:2774) with sizeof(TRACKMOUSEEVENT), and GetMouseMovePointsEx's "resolution" is really one of the two
    //GMMP_USE_*_POINTS constants (winuser.h:1347-1348) although the signature is a bare DWORD. Per the standing
    //U1/U2 policy there is no Win32ErrorHandler and no CheckReturn in this TU, so a swallowed SEH fault stays
    //indistinguishable from a legitimate zero or FALSE (U7).
    //
    //Call path (U19): 9 LAYER / 7 INDIRECT / 0 BYPASS / 0 MEMBER over the 14 spellings of these stems
    //(tmp/_tmp_mouse_bypass_s25.ps1 -> tmp/_tmp_mouse_bypass_s25.txt, comment-stripped). Every wrapper has a
    //through-layer site in code winqor_gui compiles - ui/gui/controllers/i_m/mouse.cpp:39,44,49,54,59,64,69,74 and
    //windowcontroller.cpp:673 - which is what makes this TU's binding fix behavioural rather than cosmetic (split
    //unchanged since session 20: tmp/_tmp_mouse_bypass_s25.ps1, measured 2 Nov 2026). The Mouse controller has no
    //in-tree caller of its own; under D14 that is not a finding, it only says these ten throws were latent rather
    //than live. 0 U17 - no stem in this section is an A/W family, so no T suffix is owed and the folder census is
    //unchanged (525 declarations parsed / 108 A+W+T norm / 404 single-export bare / 11 + 2 deviations,
    //tmp/_tmp_tsuffix_pre_s25.txt). 0 U26 - no wrapper here takes a TCHAR-typed parameter and neither
    //TRACKMOUSEEVENT (:2763-2768) nor MOUSEMOVEPOINT (:1334-1339) has such a field, so no mirror-width question
    //arises in this section. 0 U25 - no const in any of the ten declarations, and ::POINT is qualified identically
    //in declaration and definition.

    //Return policy (U2, table row DragDetect / mouse.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, and
    //the table's verbatim docQuote is "Type: BOOL If the user moved the mouse outside of the drag rectangle while holding
    //down the left button, the return value is nonzero. If the user did not move the mouse outside of the drag rectangle
    //while holding down the left button, the return value is zero." - the two branches are the two answers to the caller's
    //question ("did a drag happen?"), so zero is the answer for "no drag" rather than a failure value; the page names no
    //failure value at all and never tells the caller to call GetLastError, so D51 step 2 (an API that reserves no return
    //value for errors) reads no-check and the session-27 soundness test fails at that value - keying BoolCheck here would
    //report a legal no-drag answer as a failure. The value comes back exactly as the SDK produced it.
    BOOL User32::DragDetect(HWND hwnd, ::POINT pt)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DragDetect);
        return Library::Call< BOOL, HWND, ::POINT >(pFunc, hwnd, pt);
    }

    //Return policy (U2, table row GetCapture / mouse.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, and
    //the table's verbatim docQuote is "Type: HWND The return value is a handle to the capture window associated with the
    //current thread. If no window in the thread has captured the mouse, the return value is NULL ." - NULL is the empty
    //result of the query the call performs, a legal state of the thread (a thread that never called SetCapture), so
    //nothing is reserved for failure and the session-27 soundness test fails at that value; HandleCheck (returncheck.h:112,
    //which fails on INVALID_HANDLE_VALUE) and CheckNonZeroHandle (returncheck.h:137, which fails on NULL) would both report
    //a legal "no capture window" answer as a failure, so no class fits and none is landed. The value comes back exactly as
    //the SDK produced it.
    HWND User32::GetCapture(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetCapture);
        return Library::Call< HWND >(pFunc);
    }

    //Return policy (U2, table row GetDoubleClickTime / mouse.cpp, 12 Nov 2026): the row reads unchecked with no checkClass,
    //and the table's verbatim docQuote is "Type: UINT The return value specifies the current double-click time, in
    //milliseconds. The maximum return value is 5000 milliseconds." - the return is the value the caller asks the system for,
    //and the page bounds the answer above rather than reserving anything below: there is no "If the function fails" branch
    //and the page never tells the caller to call GetLastError, so D51 step 2 reads no-check and keying CheckNonZero< UINT >
    //would report a legal zero-millisecond setting as a failure. The value comes back exactly as the SDK produced it.
    UINT User32::GetDoubleClickTime(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetDoubleClickTime);
        return Library::Call< UINT >(pFunc);
    }

    //Return policy (U2, table row GetMouseMovePointsEx / mouse.cpp, 12 Nov 2026): the row reads checked with checkClass
    //TCheckFailureValue< int, -1 >, and the table's verbatim docQuote is "Type: int If the function succeeds, the return
    //value is the number of points in the buffer. Otherwise, the function returns -1. For extended error information, your
    //application can call GetLastError ." (the page writes that minus sign as an en-dash; the source stays ASCII) - the SDK
    //reserves -1 as the failure value and the success answer is a point count, so -1 is never a legal answer while a zero
    //point count is (an empty buffer is the answer the caller asked for) and must stay a pass. That polarity rules CheckNonZero
    //out and names TCheckFailureValue (returncheck.h:213, Test at :226 is (value == iFailure) ? false : true), which fails
    //only at -1 and passes at every count including 0; sdkRetType is int so the class base is int and the template is already
    //in the vocabulary - no new check class. The table's inTreeCallers column reads 1, an upper bound that counts comments and
    //other libraries' declarations; the site-level read (12 Nov 2026) finds no live call site for this stem anywhere in the
    //tree, so the row is change-safe and G6's liveness costs nothing built today.
    int User32::GetMouseMovePointsEx(UINT cbSize, LPMOUSEMOVEPOINT lppt, LPMOUSEMOVEPOINT lpptBuf, int nBufPoints, DWORD resolution)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetMouseMovePointsEx);
        CheckReturn< int, TCheckFailureValue< int, -1 > >::TType iResult = Library::Call< int, UINT, LPMOUSEMOVEPOINT, LPMOUSEMOVEPOINT, int, DWORD >(pFunc, cbSize, lppt, lpptBuf, nBufPoints, resolution);
        return iResult;
    }

    VOID User32::mouse_event(DWORD dwFlags, DWORD dx, DWORD dy, DWORD dwData, ULONG_PTR dwExtraInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, mouse_event);
        return Library::voidCall< DWORD, DWORD, DWORD, DWORD, ULONG_PTR >(pFunc, dwFlags, dx, dy, dwData, dwExtraInfo);
    }

    //Return policy (U2, table row ReleaseCapture / mouse.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, call GetLastError ." - the call's subject is the
    //thread's own capture state and the BOOL reports only whether the release happened, so the BOOL carries no answer and
    //zero is reserved for the documented failure; the session-27 soundness test passes at every value and sdkRetType is BOOL,
    //so BoolCheck (returncheck.h:94, the AutoErrorCheck< BOOL > subclass already used by the errorhandling.cpp:85 and
    //systemshutdown.cpp:134 precedent) covers it - no new check class. The table's inTreeCallers column reads 1, an upper
    //bound; the site-level read (12 Nov 2026) finds the single hit to be the comment at synchronization.cpp:85, not a call
    //site, so there is no live caller today and G6's liveness costs nothing built today.
    BOOL User32::ReleaseCapture(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ReleaseCapture);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL >(pFunc);
        return bResult;
    }

    //Return policy (U2, table row SetCapture / mouse.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, and
    //the table's verbatim docQuote is "Type: HWND The return value is a handle to the window that had previously captured
    //the mouse. If there is no such window, the return value is NULL ." - the handle is the previous-capture state the caller
    //asked about, and a thread that had never captured the mouse legitimately answers NULL, so nothing is reserved for
    //failure and the session-27 soundness test fails at that value; CheckNonZeroHandle< HWND > (returncheck.h:137) would
    //report that legal first capture as a failure. The value comes back exactly as the SDK produced it.
    HWND User32::SetCapture(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetCapture);
        return Library::Call< HWND, HWND >(pFunc, hWnd);
    }

    //Return policy (U2, table row SetDoubleClickTime / mouse.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, call GetLastError ." - the interval being set is the
    //parameter and the BOOL reports only whether the set happened, so the BOOL carries no answer and zero is reserved; the
    //session-27 soundness test passes at every value and sdkRetType is BOOL, so BoolCheck (returncheck.h:94) covers it - no
    //new check class. The table's inTreeCallers column reads 1, an upper bound; the site-level read (12 Nov 2026) finds only
    //comments and the user32.h:531 declaration for this stem, so there is no live call site and G6's liveness costs nothing
    //built today.
    BOOL User32::SetDoubleClickTime(UINT uInterval)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetDoubleClickTime);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, UINT >(pFunc, uInterval);
        return bResult;
    }

    //Return policy (U2, table row SwapMouseButton / mouse.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, and
    //the table's verbatim docQuote is "Type: BOOL If the meaning of the mouse buttons was reversed previously, before the
    //function was called, the return value is nonzero. If the meaning of the mouse buttons was not reversed, the return value
    //is zero." - the BOOL is the previous state the caller asked for, not the outcome of the swap (the swap itself is
    //unconditional), so zero is a legal answer and nothing is reserved; the page names no failure value and never tells the
    //caller to call GetLastError, so D51 step 2 reads no-check and keying BoolCheck would report a legal previously-unreversed
    //state as a failure. The value comes back exactly as the SDK produced it.
    BOOL User32::SwapMouseButton(BOOL fSwap)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SwapMouseButton);
        return Library::Call< BOOL, BOOL >(pFunc, fSwap);
    }

    //Return policy (U2, table row TrackMouseEvent / mouse.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //and the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero . If the function
    //fails, return value is zero. To get extended error information, call GetLastError ." (the page's own wording drops the
    //article in the second clause) - the TRACKMOUSEEVENT structure the caller passes is both the request and the out-parameter,
    //so the event data answers through that structure rather than through the return and the BOOL carries no answer; zero is
    //therefore reserved for the documented failure and the session-27 soundness test passes at every value, so BoolCheck
    //(returncheck.h:94) covers the BOOL sdkRetType - no new check class. The table's inTreeCallers column reads 1, an upper
    //bound; the site-level read (12 Nov 2026) finds only comments, this TU's own definition and the user32.h:533 declaration,
    //so there is no live call site and G6's liveness costs nothing built today.
    BOOL User32::TrackMouseEvent(LPTRACKMOUSEEVENT lpEventTrack)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, TrackMouseEvent);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPTRACKMOUSEEVENT >(pFunc, lpEventTrack);
        return bResult;
    }

}}}//qor::win::api
