// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//=== api_layer conventions ========================================= Added 8 Nov 2026
//   (T6: the "//List Box functions" section declared three members and no TU defined any of them - new TU)
//
//T6 (registers/tasks.md) is CLOSED here. user32.h's "//List Box functions" section declares DlgDirList, DlgDirSelectEx
//and GetListBoxInfo, all three non-static, and no .cpp in the folder defines any of them - a folder-wide scan for
//User32::<name> over the sources returns nothing, and a scan for the bare names returns only the three user32.h
//declarations - and there was no listbox TU - so the published class carried three members that fail at a caller with
//LNK2019 (open item U4) rather than at compile time (open item U18: non-static on an all-static surface). This TU
//lands both halves together, as T1's security.cpp, T3's powermanagement.cpp, T4's help.cpp and T5's ime.cpp did:
//it adds the three definitions, and user32.h:552-554 gains `static` on all three declarations. Coverage stays at
//759 of 1,018 - all three were already declared, so the instrument already counted them; this was a link gap.
//
//Binding: the DlgDir pair is a real A/W family and the bare stem is not exported - the table for the loaded user32.dll
//has DlgDirListA C1 / DlgDirListW C4 and DlgDirSelectExA C7 / DlgDirSelectExW C8, no bare spelling of either - so both
//bind qor_pp_useswinapiAW (D13). The #undef of both stems at user32.h:17,18 is what makes that spelling load-bearing:
//the SDK's own DlgDirList -> DlgDirListW (winuser.h:11596) and DlgDirSelectEx -> DlgDirSelectExW (:11638) macros are
//erased before the class is declared, so a bare qor_pp_useswinapi would stringize the un-exported literal stem, cache
//a NULL pFunc for the process lifetime and throw on every call (G2 + G5). GetListBoxInfo is a genuine single export
//(ordinal 174, RVA 0002EFF0, no A spelling and no W spelling of it in the table), so the bare qor_pp_useswinapi is the
//D13 spelling there. cl /EP on this TU resolves this TU's three bindings to the literals "DlgDirListW" "DlgDirSelectExW"
//"GetListBoxInfo" against the module literal L"user32" (the fourth literal in that output, "wsprintfW", is the header-
//inline wsprintfT body in user32.h, not a binding of this TU).
//
//Signatures (D18: the published signature is the prototype of the export it binds, parameter names included), read as
//a forward pass at the declaration line in um/winuser.h 10.0.26100.0 - the header the gate's /showIncludes resolves:
//DlgDirListA/W :11580/:11589 int WINAPI (HWND hDlg, LPSTR|LPWSTR lpPathSpec, int nIDListBox, int nIDStaticPath,
//UINT uFileType); DlgDirSelectExA/W :11624/:11632 BOOL WINAPI (HWND hwndDlg, LPSTR|LPWSTR lpString, int chCount,
//int idListBox); GetListBoxInfo :14899 DWORD WINAPI (HWND hwnd). The DlgDir pair sits inside #ifndef NOUSER (:64) and
//#ifndef NOCTLMGR (:11233) under the Desktop Family partition, with no version guard; GetListBoxInfo additionally sits
//inside #if(WINVER >= 0x0500) (:13594) and #ifndef NOWINABLE (:13716). kernel32.h:22-30 pins WINVER 0x0A00, so all
//three prototypes are visible here; NOUSER/NOCTLMGR/NOWINABLE are unpinned shrink macros, so a consumer that defines
//one of them loses the prototype (the half of U16 the pin did not close).
//
//Two of the three declarations needed more than `static`: DlgDirSelectEx is declared with the folder's own parameter
//names (HWND hDlg, LPTSTR lpString, int nCount, int nIDListBox) rather than the SDK's, so user32.h:553 moves them to
//hwndDlg / chCount / idListBox and this definition matches, parameter for parameter. Parameter names in a member
//declaration bind nothing, so that is a readability change on the published surface, not an ABI change. The other two
//declarations already spell the SDK's names.
//
//Contracts, all three stated rather than coded: DlgDirList's lpPathSpec is _Inout_ (winuser.h:11582,11591) - the call
//rewrites the caller's buffer with the entry to display next, so the buffer stays caller-owned and has to be sized for
//the path plus what comes back, and the int result is a count of matches whose negative spelling means the buffer
//filled. DlgDirSelectEx writes at most chCount characters into the caller-owned lpString (_Out_writes_(chCount),
//winuser.h:11626,11634) and FALSE means the text did not fit. GetListBoxInfo returns the number of items per row of
//the list box and 0 when the window is not one, so a legitimate 0 and a swallowed SEH fault are indistinguishable
//(the U7 shape the T6 record names). Per the standing U1/U2 policy none of the three installs a Win32ErrorHandler and
//none is wrapped in CheckReturn: all three results are values or counts, so a non-zero test would be wrong for them.
//
//U17: DlgDirList and DlgDirSelectEx are two of the eleven A/W members declared without the folder's T suffix. Neither
//is renamed here - the session-11 defer policy keeps the eleven as they are until one folder-wide pass, and ime.cpp's
//SendIMEMessageEx is the identical case (binding already AW-correct, only the name out of step). After this TU 6 of the
//eleven are defined and 5 are still U4 declared-and-undefined.
//
//Liveness (U15/U19): 0 call sites for any of the three names in src, examples or test, and 0 direct-SDK bypass sites -
//a comment-stripped search for the three stems over the tree returns only the user32.h declarations and comment lines
//in combobox.cpp, help.cpp and monitor.cpp. This TU adds surface nothing above the layer calls yet, the exported-and-
//unused shape D14 decided is not a finding.
//
//Gate: listbox.cpp standalone at /W4 exit 0, then the folder 47/47 at /W4 exit 0, dated by tmp/obj/listbox.obj being
//newer than the source; user32.h moved, so the all-libraries run was re-run as a diff against the pre-edit run.

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //The list box is addressed from inside its dialog: hDlg is the dialog window, nIDListBox the control's id inside it
    //and nIDStaticPath one of the DDL_* file-filter bits (winuser.h:11607-11616). lpPathSpec is both the pattern in and
    //the next entry out, so the caller owns it and the layer forwards it untouched. The count comes back as written.
    int User32::DlgDirList(HWND hDlg, LPTSTR lpPathSpec, int nIDListBox, int nIDStaticPath, UINT uFileType)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DlgDirList);
        return Library::Call< int, HWND, LPTSTR, int, int, UINT >(pFunc, hDlg, lpPathSpec, nIDListBox, nIDStaticPath, uFileType);
    }

    //BOOL, but not a plain success flag: the SDK's own note is that FALSE means the selected control's text did not fit
    //into the chCount characters the caller supplied. lpString is _Out_writes_(chCount) and stays caller-owned; the
    //layer neither sizes nor inspects it.
    BOOL User32::DlgDirSelectEx(HWND hwndDlg, LPTSTR lpString, int chCount, int idListBox)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DlgDirSelectEx);
        return Library::Call< BOOL, HWND, LPTSTR, int, int >(pFunc, hwndDlg, lpString, chCount, idListBox);
    }

    //A value, not a flag: the number of list-box items per row for the window named by hwnd, 0 when that window is not
    //a list box. Returned raw, which is what leaves a swallowed fault and a legitimate 0 indistinguishable (U7).
    DWORD User32::GetListBoxInfo(HWND hwnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetListBoxInfo);
        return Library::Call< DWORD, HWND >(pFunc, hwnd);
    }

}}}

