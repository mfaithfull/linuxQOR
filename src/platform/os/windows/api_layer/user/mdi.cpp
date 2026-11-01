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

    //Four wrappers - the whole "user32.h:494 //Multi Document Interface functions" section, declared :495-498. All four
    //declarations are static and all four are defined here in header order (:84, :91, :98, :105) - no U4 (declared,
    //never defined) and no U13 (defined, never declared static) instance. The SDK's MDI block is winuser.h's
    //#ifndef NOMDI region, :12074-:12261, and it holds seven entry points; this section owns four of them while
    //ArrangeIconicWindows (:12199, ordinal E), CascadeWindows (:12249, 24) and TileWindows (:12240, 3AE) are declared in
    //the header's "//Window functions" block (:135, :139, :208) and defined in window.cpp:58, :86 and :572. One SDK family,
    //two header sections, two TUs - U21-shaped, recorded and not moved. The mirror case does not arise here: no member of
    //this section belongs to another family, so unlike keyboard.cpp and devicecontext.cpp the section label and its
    //contents agree.
    //
    //Binding (D13): three of the four stems are A/W families and bind through qor_pp_useswinapiAW - CreateMDIWindowA/W
    //(ordinals 6F/70), DefFrameProcA/W (A1/A2), DefMDIChildProcA/W (A3/A4) - and none of the three bare stems is an
    //export. A bare qor_pp_useswinapi would still resolve today, only because the SDK macros are live in the binding
    //expression: #define CreateMDIWindow CreateMDIWindowW (winuser.h:12230-12234), DefFrameProc (:12147-12151),
    //DefMDIChildProc (:12179-12183) - G1 is exactly the reliance D13 forbids. TranslateMDISysAccel is a genuine single
    //export (3B9, no A or W spelling anywhere in the table) and binds bare. cl /EP emits four literals and no more -
    //"CreateMDIWindowW", "DefFrameProcW", "DefMDIChildProcW", "TranslateMDISysAccel" (tmp/_tmp_mdi_pp.txt:453322,
    //:453329, :453336, :453343) - and all four are in the export table, so this TU contributes 0 of the folder's 17
    //unresolvable bindings (tmp/_tmp_p1census_s21.txt). A live probe (tmp/_tmp_mdi_probe_s21.txt) resolves all four names
    //and reports MISSING for the three bare stems and for the *T spellings, and it had to LoadLibraryW first:
    //GetModuleHandleW(L"user32") fails with gle=126 in a bare console process, which is G5 measured directly.
    //
    //Signatures: every parameter and return type pairs field-for-field with the SDK prototype (DefFrameProcA/W
    //winuser.h:12132/:12141, DefMDIChildProcA/W :12161/:12174, TranslateMDISysAccel :12190, CreateMDIWindowA/W
    //:12205/:12219) - 0 U18 divergences, and CreateMDIWindow's style parameter really is DWORD in this SDK, not the LONG
    //older documentation shows. What was wrong is internal, not published: DefFrameProcT's Call<> named INT for the
    //parameter the member and the SDK both call UINT - the only uppercase INT token in any Call<>/voidCall<> template
    //list in the folder (tmp/_tmp_int_sites_s21.ps1: 1 hit of 435 such lists, now 0) and the only sigcmp flag in this TU.
    //Same width and same value for every real message id, so nothing about the emitted call changes - the point
    //of naming the template arguments is that they restate the signature (U14), and this one contradicted it.
    //
    //Availability (U16): 0 of the four prototypes carries a WINVER/_WIN32_WINNT guard, so this TU adds no version-guard
    //instance. All four sit inside #ifndef NOMDI and the Desktop partition (#if WINAPI_FAMILY_PARTITION(
    //WINAPI_PARTITION_DESKTOP), :12092), and TranslateMDISysAccel additionally inside #ifndef NOMSG (:12185-:12194).
    //Neither NOMDI nor NOMSG nor NOUSER is defined anywhere in this build (0 hits over src/examples/test/cmake), but
    //user32.h:498 publishes the SDK typedef LPMSG, so a consumer that defines NOMSG breaks that declaration outright -
    //the hook.cpp:52-56 shape (opt-out macro), not a version-guard one. TileWindows/CascadeWindows, the two MDI-family
    //calls this TU does not own, are the guarded ones: #if(WINVER >= 0x0400), :12236-:12256 - staged for window.cpp.
    //
    //TCHAR width (U26): of the three AW wrappers only CreateMDIWindowT has a TCHAR parameter (two LPCTSTR), and mdi.cpp's
    //preprocessed output has exactly one LPCTSTR typedef, "typedef LPCWSTR PCTSTR, LPCTSTR;" (tmp/_tmp_mdi_pp.txt:147192),
    //so the wide export and the published signature agree in this TU. DefFrameProcT/DefMDIChildProcT carry the T suffix
    //because their stems are A/W families (the U17 norm) while naming no TCHAR parameter of their own.
    //
    //Contracts the wrappers do not enforce: CreateMDIWindowT returns the new MDI child window handle, which the caller
    //owns and must destroy, and its hWndParent must already be an MDI client window - the client is what DefFrameProcT
    //builds when it handles WM_MDICREATE from a CLIENTCREATESTRUCT. DefFrameProcT belongs in the *frame* window procedure
    //with the MDI client handle as its second argument; DefMDIChildProcT belongs in the *child* window procedure. Swapping
    //the two is the classic MDI bug and nothing in this layer can see it: no handle check, and no CheckReturn/
    //Win32ErrorHandler per the U1/U2 policy. TranslateMDISysAccel is a message-loop call that wants the MDI client window
    //rather than the frame, and its BOOL means only "an MDI system accelerator was consumed" - with the return raw, a
    //swallowed SEH fault and a legitimate 0 are indistinguishable (U7).
    //
    //Liveness (U15): CreateMDIWindowT, DefFrameProcT and DefMDIChildProcT have no use site anywhere in src/examples/test
    //beyond the definition and the declaration. TranslateMDISysAccel has one compiled layer site,
    //MessageHandler::TranslateMDISysAccel at ui/gui/view/handlers/messagehandler.cpp:241 (declared messagehandler.h:93,
    //compiled into winqor_gui at ui/gui/CMakeLists.txt:60, reached from ui/CMakeLists.txt:5), and that handler method has
    //no caller of its own - so all four remain U15 instances. U19 over the 13 suffix variants: 1 LAYER, 1 INDIRECT,
    //0 BYPASS, 0 MEMBER (tmp/_tmp_mdi_bypass_s21.txt); messagehandler.cpp has no comment-only line, so the site is live.
    //That site passes a Message* (ui/gui/view/handlers/message.h:33-44) to the export as ::LPMSG, and the mirror is only
    //ABI-correct on x64: measured both targets with tmp/_tmp_msg_mirror_s21.cpp, ::MSG and Message agree exactly on x64
    //(48 bytes each, hwnd/message/wParam/lParam/time/pt at 0/8/16/24/32/36) and part company from lParam onward on x86
    //(::MSG 28 bytes, lParam@12 time@16 pt@20 against Message 40 bytes, lParam@16 time@24 pt@28) because the mirror's
    //portable 64-bit fields force 8-byte alignment there - its field list is otherwise faithful, right down to the SDK's
    //own #ifdef _MAC lPrivate (winuser.h:1884-1886). Only x64 is configured today (CMakeSettings.json has the single
    //x64-Debug profile), so this is latent; it is winqor_gui surface, recorded under U26 and not fixed here - message.cpp's
    //session will find the same mirror in front of far more wrappers.

    HWND User32::CreateMDIWindowT(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HINSTANCE hInstance, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CreateMDIWindow);
        return Library::Call< HWND, LPCTSTR, LPCTSTR, DWORD, int, int, int, int, HWND, HINSTANCE, LPARAM >(pFunc, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hInstance, lParam);
    }

    LRESULT User32::DefFrameProcT(HWND hWnd, HWND hWndMDIClient, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DefFrameProc);
        return Library::Call< LRESULT, HWND, HWND, UINT, WPARAM, LPARAM >(pFunc, hWnd, hWndMDIClient, uMsg, wParam, lParam);
    }

    LRESULT User32::DefMDIChildProcT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DefMDIChildProc);
        return Library::Call< LRESULT, HWND, UINT, WPARAM, LPARAM >(pFunc, hWnd, uMsg, wParam, lParam);
    }

    BOOL User32::TranslateMDISysAccel(HWND hWndClient, LPMSG lpMsg)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, TranslateMDISysAccel);
        return Library::Call< BOOL, HWND, LPMSG >(pFunc, hWndClient, lpMsg);
    }

}}}//qor::win::api
