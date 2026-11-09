// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//=== api_layer conventions ========================================= Added 9 Nov 2026
//   (T9 step 9d: the "//Button Control functions" section declared 3 members non-static and no TU defined any of them - new TU)
//
//T9 (registers/tasks.md, D26) is the tranche queue for U4's remaining declared-but-undefined surface, and this TU lands
//step 9d's two halves together the way 9a's strings.cpp, 9b's rawinput.cpp, 9c's systeminfo.cpp and T6's listbox.cpp did:
//it defines all 3 members of user32.h's "//Button Control functions" (user32.h:378-380) - CheckDlgButton CheckRadioButton
//IsDlgButtonChecked - and user32.h gains `static` on all 3 declarations. U4's link gap (a caller of any of the 3 failed
//at LNK2019) and U18's non-static shape close together. Coverage stays 759 of 1,018 - all 3 were already declared, so the
//coverage instrument counted them; this closes a link gap, never a coverage gap (the arithmetic D24 recorded for T6 and
//D27/D28/D29 for 9a/9b/9c). The section is 3/3 declared, 3/3 defined and 3/3 bound: the head is user32.h:377 and the next
//section "//Coordinate Transform functions" is :382, so no member and no blank sits outside this TU, and registers/u18-coverage.md
//names no Button Control name as an exclusion, so this TU contributes no raw gap.
//
//Binding spelling - D13 applied, and 3/3 take the bare form: 3 x qor_pp_useswinapi(user32, <literal>) and 0 x
//qor_pp_useswinapiAW. This is 9a's shape (34 bare / 0 AW), not 9b's or 9c's (6/1 and 3/1): none of the three stems is
//#undef'd at user32.h:13-:21 (that block lists RegisterDeviceNotification WinHelp SystemParametersInfo GetRawInputDeviceInfo
//DlgDirList DlgDirSelectEx DlgDirListComboBox DlgDirSelectComboBoxEx LoadBitmap and no Button Control name), and none is an
//SDK stem macro switch - the switch probe over um\WinUser.h reads 0 lines for the three stems, while the two declarations
//immediately above the block spell one: um\WinUser.h:5190-:5194 "#ifdef UNICODE / #define SetDlgItemText SetDlgItemTextW /
//#else / #define SetDlgItemText SetDlgItemTextA / #endif" and :5214-:5218 the same for GetDlgItemText. So G1's rewrite of the
//binding name before it is stringized does not fire here, and the literal the source spells is the literal GetProcAddress
//is given. qor_pp_useswinapiAW is not merely wrong here but unavailable: it pastes Stem##W at kernel32.h:77, and the loaded
//export table (tmp/_tmp_user32_exports.txt, 1,018 names) reads no row for CheckDlgButtonA/W, CheckRadioButtonA/W or
//IsDlgButtonCheckedA/W - the three rows are bare, CheckDlgButton hint 42 (ordinal 1575), CheckRadioButton hint 47 (1580),
//IsDlgButtonChecked hint 234 (2071). A live probe (tmp/_tmp_t9_9d_probe.cpp run by tmp/_tmp_t9_9d_probe.bat,
//tmp/_tmp_t9_9d_probe.txt) resolves 3/3 bare at 00007FFF2CC26490 / :CC264D0 / :CC26770 over a user32.dll handle of
//00007FFF2CBD0000 - each address the handle plus the table's RVA - and reads MISSING for all six A/W spellings, while a
//deliberately bogus module name resolves to NULL, so the probe discriminates rather than reporting a vacuous hit.
//
//SDK availability, read at the declaration line in um\WinUser.h 10.0.26100.0: CheckDlgButton :5223, CheckRadioButton :5231,
//IsDlgButtonChecked :5240. All three sit at the same depth 4 - #ifndef _WINUSER_ (:13), #ifndef NOUSER (:64), #ifndef NOCTLMGR
//(:4912) and #if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) (:5006) - so they are the whole "//Button Control" block the
//NOCTLMGR guard opens, and the block is contiguous with the Dialog functions above it (SetDlgItemInt :5161, GetDlgItemInt :5170,
//SetDlgItemTextA/W :5179/:5186, GetDlgItemTextA/W :5200/:5209). NOCTLMGR/NOUSER/NODIALOG are defined nowhere in the tree
//(_tmp_t9_9d_sdkctx.ps1: 0 hits for each over the 3,565 #define lines in src/cmake/examples/test, and 0 in um\WinUser.h's own),
//so D21's pin (kernel32.h:22-30, _WIN32_WINNT 0x0A00) is the live condition and user32.h adds no guard of its own - the unpinned
//shrink macros stay the half of U16 the pin did not close, recorded rather than acted on.
//
//Signatures - D18's rule: the published signature is the prototype of the export this member binds, parameter names included.
//All 3 declarations already copy it exactly, so the header edit is `static` only and no type and no parameter name moves:
//CheckDlgButton( _In_ HWND hDlg, _In_ int nIDButton, _In_ UINT uCheck) at :5223-:5226, CheckRadioButton( _In_ HWND hDlg,
//_In_ int nIDFirstButton, _In_ int nIDLastButton, _In_ int nIDCheckButton) at :5231-:5235, IsDlgButtonChecked( _In_ HWND hDlg,
//_In_ int nIDButton) at :5240-:5242, against user32.h:378-:380's HWND/int/UINT, HWND/int/int/int and HWND/int spellings and the
//same names. The return types match too: WINUSERAPI BOOL (:5221), WINUSERAPI BOOL (:5229), WINUSERAPI UINT (:5238).
//
//Return convention - the G6 audit, done before choosing a wrapper type: the SDK claims nothing to check here. The SAL window
//over each declaration reads _In_ only - no _Check_return_ and no _Success_(...) - which is the shape um\WinUser.h:5196
//_Ret_range_(0, cchMax) and :5203 _Out_writes_(cchMax) show the SDK using on its neighbours, so unlike SystemParametersInfo
//(9c: _Success_(return != FALSE) at :13251/:13260) these three carry no failure claim, and G6 says CheckReturn<...>::TType is
//always the checked wrapper type anyway, so adopting it here would raise a continuable(...) issue on a return the SDK never
//calls a failure. The layer therefore hands all 3 returns over as written, and the two BOOLs are read as questions, not
//assertions: CheckDlgButton/CheckRadioButton answer "is this control set to this state", so FALSE is a legal answer and a bad
//hDlg is indistinguishable from that answer; IsDlgButtonChecked answers 0/1, where 0 is a legal "not checked" and not a failure.
//No return convention is adopted and no CheckReturn/Win32ErrorHandler is introduced.
//
//Consumers: the bypass scan over src/examples/test (_tmp_t9_9d_measure.ps1, 3,226 files) reads exactly one hit per name -
//user32.h:378/:379/:380, the declarations themselves - so 0 call sites exist and the `static` edit follows no caller.
//
//Census over this close (_tmp_static_census.ps1): user32.h keeps 634 member-declaration lines and stays 855 lines, static
//593 -> 596, non-static 41 -> 38, distinct defined member names / binding lines 592 -> 595 over 50 -> 51 sources, with 0
//defined-but-not-static and 0 defined-without-a-declaration both sides; _tmp_decl_no_def_repo_scan.ps1 reads its declared-and-
//undefined count 7 -> 4, leaving exactly 9e's 2 (user32.h:779-:780), 9f's 1 (:776) and 9g's 1 (:783) plus the 34 DDE ") const;"
//lines its regex misses by design.
//
//Gate: buttoncontrol.cpp standalone at /W4 exit 0, then the folder 51/51 at /W4 exit 0, cl /EP emitting the 3 literals
//above against the module literal L"user32", and - because user32.h moved - the all-libraries run as a diff against a pre-edit
//run (118 cpp echoes, 166 diagnostic lines, 1 C1083 at opengl/wgl.cpp:31, exit 2).

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //The dialog's own question "is this control set to this state": hDlg names the dialog window, nIDButton the control's
    //resource id and uCheck the state to ask for - the SDK takes it as a plain _In_ UINT, not a BOOL, so the caller states
    //the state rather than the header narrowing it. No _Success_ and no _Check_return_ here, so FALSE is the answer "not set"
    //and a bad hDlg is indistinguishable from that answer; see the banner's G6 audit for why there is no CheckReturn.
    BOOL User32::CheckDlgButton(HWND hDlg, int nIDButton, UINT uCheck)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CheckDlgButton);
        return Library::Call< BOOL, HWND, int, UINT >(pFunc, hDlg, nIDButton, uCheck);
    }

    //The same question over a range: nIDFirstButton..nIDLastButton bounds the group and nIDCheckButton picks the control
    //inside it, so this is the one member here with four arguments and no state argument of its own - uCheck does not exist
    //for a group, the SDK asks whether the checked control of the group is nIDCheckButton. The range is the caller's to get
    //right: nothing in the SDK or here sizes or orders it, and FALSE covers both "not the checked one" and "outside the group".
    BOOL User32::CheckRadioButton(HWND hDlg, int nIDFirstButton, int nIDLastButton, int nIDCheckButton)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CheckRadioButton);
        return Library::Call< BOOL, HWND, int, int, int >(pFunc, hDlg, nIDFirstButton, nIDLastButton, nIDCheckButton);
    }

    //The state as a value rather than a question: 0 for "not checked" and 1 for "checked", so this UINT is a two-valued
    //answer and 0 is legal - the reason the two BOOL questions above and this one all hand their return over as written.
    //The export is bare (hint 234) and the table has no IsDlgButtonCheckedA/W row, so the binding is the bare spelling;
    //see the banner.
    UINT User32::IsDlgButtonChecked(HWND hDlg, int nIDButton)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsDlgButtonChecked);
        return Library::Call< UINT, HWND, int >(pFunc, hDlg, nIDButton);
    }

}}}   //qor::win::api

