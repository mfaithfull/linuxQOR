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

    //=== api_layer conventions ========================================= Reviewed 5 Nov 2026
    //   (session 36: correctness / consistency / safety / UB / conventions; 2 wrappers)
    //
    //Two wrappers - the whole "user32.h:321 //Window Procedure functions" section: declared :322-:323, both static, both
    //defined here through the User32:: qualifier and in header order (:76 CallWindowProcT, :83 DefWindowProcT), so 2/2
    //in-section declarations pair with a definition: 0 U4 (declared, never defined), 0 U11 (definition unqualified),
    //0 U13 (defined, declared non-static), 0 U17 (both sit over an A/W family and both carry the folder's T suffix).
    //2/2 members carry qor_pp_fcontext (:78, :85).
    //
    //Return and parameter types are the SDK's exactly (U18 clean): "DefWindowProcA/W" LRESULT WINAPI at winuser.h:4146,
    //:4159 against LRESULT at :76 and :83, and "CallWindowProcA/W" LRESULT WINAPI with "_In_ WNDPROC lpPrevWndFunc" at
    //:4181, :4190 - the layer's WNDPROC parameter is the STRICT prototype's own type, not a reinterpret. One reading
    //trap: the three lines immediately above "DefWindowProcA/W" are the UNTAKEN "#ifndef _MAC" arm's "LRESULT CALLBACK"
    //(:4143-:4144); the taken arm is "LRESULT WINAPI" (:4140-:4141), and a line-window scan of the name's three
    //predecessors prints the wrong convention here.
    //
    //The WNDPROC parameter is a genuine type check, measured (tmp/_tmp_s36_cbprobe.cpp built by
    //tmp/_tmp_s36_cbprobe.bat, then the same questions over pure SDK headers on both arches in
    //tmp/_tmp_s36_conv.cpp + .bat): is_same<WNDPROC,FARPROC>=0, a real CALLBACK procedure converts (=1), and a wrong
    //return type, a wrong parameter list or a FARPROC from GetProcAddress all answer 0 - a mismatched procedure cannot
    //reach the OS silently through CallWindowProcT. The one hole is the calling convention:
    //is_convertible<__cdecl procedure,WNDPROC> measures 1 on x64 (one convention, so MSVC treats the pointers as the
    //same type) and 0 on x86 - the type system only catches a wrong-convention procedure on the arch that does not
    //build (U6), so the consumers that supply procedures spell the convention themselves
    //(windowfactory.cpp:49 qor_pp_compiler_stdcallconvention). A NO_STRICT consumer is the other way round:
    //winuser.h:4207, :4216 redeclare "CallWindowProcA/W" with lpPrevWndFunc as FARPROC while WNDPROC itself (:91) is
    //declared outside the STRICT arms, so this TU's parameter keeps its type either way.
    //
    //The seam this TU cannot close: Get/SetWindowLongPtrT (windowclass.cpp) answer LONG_PTR, and
    //is_convertible<LONG_PTR,WNDPROC>=0 and is_convertible<WNDPROC,LONG_PTR>=0 on both arches, so a caller that reads a
    //procedure back out of GWLP_WNDPROC must cast it before CallWindowProcT will take it. Nobody does - 0 in-tree call
    //sites for CallWindowProcT (published surface, D14), so this is a note for the audit, not a finding.
    //
    //Binding (D13): 2 qor_pp_useswinapiAW (:79, :86) over stems CallWindowProc and DefWindowProc; a fresh
    //"dumpbin /exports" of C:\Windows\System32\user32.dll (the session-22 x86 capture agrees) gives each stem
    //"<stem>A" and "<stem>W" and NO bare export, so the AW spelling is the only one that could resolve at all and
    //P1/P2/P6 cannot arise here. cl /EP over the TU (tmp/_tmp_win_pp_s36.bat read by tmp/_tmp_s36_ppclass.ps1): 2
    //binding lines, 2 literals CallWindowProcW and DefWindowProcW, module literal L"user32" 2/2, 0 absent from the
    //export table, 0 ...WW, 0 ...T; wrong-stem scan 0 hits here (folder: 428 bindings, 2 flagged - cursor.cpp:64,
    //window.cpp:197 - both older TUs).
    //
    //U7 bites harder here than anywhere in the folder: Library::Call's SEH handler hands a default-constructed LRESULT,
    //i.e. 0, straight back to the OS from a window procedure - for WM_NCCREATE/WM_CREATE that reads as "failed" and for
    //WM_CTLCOLOR* as a NULL brush, and the serious(...) report is the only trace (no handler installed, U1). Recorded,
    //nothing added; the consumers that use the value treat it as an answer - winctrlparent.cpp:139, :144, :150 wrap the
    //WM_CTLCOLOR* brush it returns in a non-owning Brush(PrimitiveHandle(...), false).
    //U19: one live layer call - ui/gui/window.cpp:106, reached through the Window::DefWindowProcT member (window.h:67)
    //from 32 live sites in view/handlers and ui/egl, 3 of which drop the LRESULT altogether (menu.cpp:45,
    //toplevel.cpp:122, :127); 2 live SDK bypasses sit in the main window procedure itself, windowfactory.cpp:71 and :75
    //call the DefWindowProc macro (= DefWindowProcW under this build) instead of this wrapper, so they lose this layer's
    //qor_pp_fcontext and its SEH frame; 2 more are in examples (retropad controller.cpp:324, retropad.cpp:890) and 18
    //occurrences are commented out (gl/windowimpl.cpp, view/handlers/interactive/ime.cpp).
    //U16: guard depth 3 - "#ifndef _WINUSER_", "#ifndef NOUSER", WINAPI_FAMILY_PARTITION(DESKTOP | GAMES) - with no
    //WINVER or _WIN32_WINNT floor on either pair, so nothing to record. U25: 0 CONST and 0 lowercase const in this TU
    //and in its 2 header declarations. T7: the #include "../returncheck.h" at (then) :11 was unused here (0 CheckReturn,
    //BoolCheck or Win32ErrorHandler) - one of the 35 dead, removed under T7 on 5 Nov 2026. U14 is frozen
    //(D15): :80 writes Library::Call<LRESULT,WNDPROC, HWND, UINT, WPARAM, LPARAM>( - glued comma, no space before the
    //"(" - left as-is, no tally booked.


    LRESULT User32::CallWindowProcT(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CallWindowProc);
        return Library::Call<LRESULT,WNDPROC, HWND, UINT, WPARAM, LPARAM>(pFunc, lpPrevWndFunc, hWnd, Msg, wParam, lParam);
    }

    LRESULT User32::DefWindowProcT(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DefWindowProc);
        return Library::Call<LRESULT, HWND, UINT, WPARAM, LPARAM>(pFunc, hWnd, Msg, wParam, lParam);
    }

}}}//qor::win::api
