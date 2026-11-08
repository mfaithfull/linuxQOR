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

    //The "//Shell functions" section (user32.h:743-748): five declarations, five definitions - here, in the header's
    //order, all static, all qualified User32::, all five call sites explicit about the SDK argument types (U14:
    //5/5 explicit, 5/5 spaced style). The menu pair at :744-745 was the folder's U4 + U13 shape - declared WITHOUT
    //static and defined in no TU of this folder, so a caller was ill-formed (C2352: a non-static member cannot be
    //qualified through the class name) rather than facing LNK2019, and dumpbin on the built import library showed it
    //directly - the only help-family linker members were ?SetWindowContextHelpId@..., ?GetWindowContextHelpId@... and
    //?WinHelp@... (tmp/_tmp_help_linkermember.txt), the menu pair absent. Task T4 (registers/tasks.md) is CLOSED 8
    //Nov 2026 and lands both halves together: these two definitions are added here, ahead of the window pair exactly
    //as the header orders them, and user32.h:744-745 gains `static` on both declarations, matching the three static
    //members the section already had. Nothing else in the section moves and no ABI is touched, but the published
    //surface gains two members - which is why the task belongs with the U4/U13 sweep, not with a TU fix. The entry
    //points were always real (SetMenuContextHelpId 34A, GetMenuContextHelpId 17C in tmp/_tmp_user32_exports.txt, both
    //RESOLVED by the live probe), so this was never a coverage gap: coverage was already 757 of 1,018 because the
    //instrument counts member declarations. 0 U4 and 0 U13 members remain here (5/5 declared static and defined), 0
    //U11.
    //Section label: "Shell functions" is the classic SDK's "Shell Support Functions" group, whose other members are
    //scattered over this header - CreateWindowExT :145 (window.cpp), DrawStateT :269 / LockWindowUpdate :280
    //(painting.cpp), DrawIconEx :434 and CopyImage/LoadImageT :443-444 (resource.cpp), DlgDirList :546 /
    //DlgDirListComboBoxT :551 (dialog.cpp), LoadBitmap :672. Only these five sit under the label, so the TU name
    //matches the SDK's own "Help support" block (winuser.h:12265) rather than the folder's section name - a
    //U12/U21-shaped placement note, nothing moved here.
    //Bindings, all five verified against the export table: the four context-help-id names are single exports with no
    //A/W family at all - SetWindowContextHelpId 37A, GetWindowContextHelpId 1E1, SetMenuContextHelpId 34A and
    //GetMenuContextHelpId 17C, one row each and no A/W spelling of any of them - so the bare qor_pp_useswinapi is the
    //D13 spelling for all four, including the two added here. WinHelp has to use qor_pp_useswinapiAW because the table
    //has WinHelpA 3EC and WinHelpW 3ED and NO bare WinHelp (probe: WinHelp MISSING, WinHelpW RESOLVED), and because
    //user32.h:14 #undef's the stem before both the class declaration at :748 and this binding, which closes G1's
    //nested-expansion route - a bare binding here would stringize "WinHelp" and throw "Missing library function." at
    //run time. That same #undef is load-bearing for the member NAME: the SDK's
    //WinHelp -> WinHelpW macro (winuser.h:12386) would otherwise rewrite the declaration and the definition out of
    //step with each other, and cl /EP shows the pair agreeing unwritten as
    //"BOOL User32::WinHelp(HWND hWndMain, LPCTSTR lpszHelp, UINT uCommand, ULONG_PTR dwData)" (tmp/_tmp_help_pp.txt).
    //This TU used to carry its own "#undef WinHelp" after including user32.h; it was inert (user32.h:14 has already
    //undef'd the stem and nothing included later can re-emit it past winuser.h's _WINUSER_ guard) and is removed
    //here, with /EP and the /W4 compile unchanged afterwards - console.cpp lost the same line in session 7, so no
    //.cpp in this folder #undefs anything now.
    //The five literals cl /EP resolves here are "SetMenuContextHelpId", "GetMenuContextHelpId", "SetWindowContextHelpId",
    //"GetWindowContextHelpId" and "WinHelpW" (the last re-measured 8 Nov 2026 in tmp/_tmp_t4_help_pp.txt - the line
    //numbers shift with the weight of this comment block, the literals do not). tmp/_tmp_sigcmp.ps1 over user32.h:744-748
    //gives 5/5 decl==def ok and 5/5 Call ok, the wrong-stem scan (tmp/_tmp_wrong_stem_scan.ps1) flags nothing in this
    //file, and the parameter-name scan reports 0 divergences - which is all this section can support, since the SDK's
    //four help-id prototypes (winuser.h:9125-9146) have UNNAMED parameters, so those names cannot be compared with the
    //SDK at all.
    //Signatures match the SDK field for field: SetWindowContextHelpId(HWND, DWORD) winuser.h:9125-9127,
    //GetWindowContextHelpId(HWND) :9132-9133, SetMenuContextHelpId(HMENU, DWORD) :9138-9140, GetMenuContextHelpId(HMENU)
    //:9145-9146 - all four in the same #if(WINVER >= 0x0400) + Desktop block - and WinHelpW(HWND, LPCWSTR, UINT,
    //ULONG_PTR) :12377-12384. The returns match too, so no U18 instance: the two BOOLs are genuine success flags (WinHelp
    //reports failure as FALSE + GetLastError), while BOTH context-help-id getters return DWORD 0 both when the object
    //carries no context id and when the handle is rejected, so failure and a legitimate 0 are indistinguishable - and
    //per the settled policy this TU adds no Win32ErrorHandler and no CheckReturn (U1/U2: 5 call sites, 0 handlers, 0
    //checks).
    //The wide/narrow half is driven by one switch and is consistent here: qor_pp_unicode (configuration.h:13) makes
    //kernel32.h:7-10 define UNICODE/_UNICODE before Windows.h, so LPCTSTR is LPCWSTR (measured both ways in
    //tmp/_tmp_tchar_probe.cpp: with that include chain _UNICODE=1, sizeof(TCHAR)=2, LPCTSTR==LPCWSTR; without it the
    //SDK defaults hold, _UNICODE=0 and LPCTSTR==LPCSTR), and the linker name this library exports is
    //?WinHelp@User32@api@win@qor@@SAHPEAUHWND__@@PEB_WI_K@Z, i.e. a const wchar_t* (tmp/_tmp_help_linkermember.txt).
    //A consumer TU that reaches <Windows.h> before kernel32.h sees the narrow spelling and references a PEB_ form this
    //library does not export, so the mismatch surfaces as a link error rather than as a wide entry point handed a
    //narrow string. In the SDK WinHelp's hWndMain and lpszHelp are _In_opt_ (:12373-12374, :12381-12382) - a NULL
    //window or NULL path is legal - while uCommand and dwData are _In_; the wrapper's HWND/LPCTSTR admit NULL, so
    //nothing here rejects the optional case. No QOR-side value names the HELP_* command constants
    //(winuser.h:12333-12355) or HELPWININFO, which is why the only caller passes a bare unsigned int, and Microsoft
    //documents WinHelp as superseded by HtmlHelp, which no api_layer library wraps at all. WinHelp owns its help-file
    //instance internally and none of the five wrappers acquires a handle, so the folder's "an acquired handle is
    //returned for the caller to release" contract does not apply to this section - the HWND/HMENU it takes is caller-owned.
    //U16: the four context-help-id prototypes sit under #if(WINVER >= 0x0400) (:9105) + the Desktop family (:9110)
    //inside #ifndef NOUSER (:64); WinHelpA/W sit under #ifndef NOHELP (:12270) + Desktop (:12268) with NO version
    //guard. The direction is the benign one, unlike FLASHWINFO/HDEVNOTIFY: every type the five wrappers name is
    //version-independent, so NOUSER/NOHELP or a small WINVER leaves user32.h:744-748 well-formed and the bindings
    //still resolve against the shipped DLL.
    //Liveness (U15 3 of 5): the only sites in the tree are WindowController::SetContextHelpId, GetContextHelpId and Help
    //(ui/gui/controllers/windowcontroller.cpp:785, :790, :795 - built through ui/CMakeLists.txt:5 +
    //gui/CMakeLists.txt:27), and those three methods have no call site anywhere in src/examples/test, so the chain is
    //compiled, exported and never entered. The menu pair adds 0 of 2: a scan of every .cpp/.h under src, examples and
    //test (3,221 files, tmp/_tmp_t4_callers_scan.ps1, run 8 Nov 2026) finds SetMenuContextHelpId / GetMenuContextHelpId
    //in no call site at all and no direct SDK call either, so the pair is published and never entered - the T2 shape,
    //and the only thing the fix changes for a caller is that the pair now links instead of being ill-formed. U19 is as
    //clean as it gets over the five (tmp/_tmp_help_bypass.txt: 3 LAYER, 0 INDIRECT, 0 BYPASS, 0 MEMBER). Both BOOLs are
    //real success flags, so the caller's fold to bool is semantically right (no
    //U24 instance, unlike FlashWindow's previous-state BOOL); :785 converts implicitly while :795 writes
    //"? true : false" - a gui-side consistency nit, not a layer one. The gui's WM_HELP path
    //(ui/gui/view/handlers/interactive/help.cpp:46) takes the context id from the HELPINFO payload, so it never goes
    //through either context-help-id getter.

    //The menu half of the context-help-id family - user32.h:744-745, which the header has held since the folder review
    //and which task T4 closes from both sides: this TU gains the two definitions, and user32.h:744-745 gains `static` on
    //both declarations, matching the three static members the section already had. Same contract as the window pair just
    //below: the id is a plain DWORD property of the menu (0 clears it), no handle is acquired or released here, and
    //GetMenuContextHelpId's DWORD return makes a rejected menu handle indistinguishable from a menu that legitimately
    //carries no context id - so no Win32ErrorHandler and no CheckReturn, exactly as the window pair gets none (U1/U2).
    //Both bind bare: the export table has one row each (SetMenuContextHelpId 34A RVA 0002FBC0, GetMenuContextHelpId
    //17C RVA 000796C0) and no A/W spelling of either stem, and neither stem is in user32.h's nine #undefs, so the bare
    //qor_pp_useswinapi is the D13 spelling for both (G1/G2).
    BOOL User32::SetMenuContextHelpId(HMENU hmenu, DWORD dwContextHelpId)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetMenuContextHelpId);
        return Library::Call<BOOL, HMENU, DWORD>(pFunc, hmenu, dwContextHelpId);
    }

    DWORD User32::GetMenuContextHelpId(HMENU hmenu)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetMenuContextHelpId);
        return Library::Call<DWORD, HMENU>(pFunc, hmenu);
    }

    BOOL User32::SetWindowContextHelpId(HWND hwnd, DWORD dwContextHelpId)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetWindowContextHelpId);
        return Library::Call<BOOL, HWND, DWORD>(pFunc, hwnd, dwContextHelpId);
    }

    DWORD User32::GetWindowContextHelpId(HWND hwnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetWindowContextHelpId);
        return Library::Call<DWORD, HWND>(pFunc, hwnd);
    }

    BOOL User32::WinHelp(HWND hWndMain, LPCTSTR lpszHelp, UINT uCommand, ULONG_PTR dwData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, WinHelp);
        return Library::Call<BOOL, HWND, LPCTSTR, UINT, ULONG_PTR>(pFunc, hWndMain, lpszHelp, uCommand, dwData);
    }

}}}//qor::win::api
