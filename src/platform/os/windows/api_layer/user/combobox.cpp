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
    //The "Combo Box functions" section (user32.h:550-553): three wrappers, three bindings, in header order.
    //GetComboBoxInfo is a single user32.dll export (winuser.h:14785 declares it directly, no A/W family and no
    //stem -> stemW macro), so the bare stem is the literal name GetProcAddress is given. DlgDirListComboBox and
    //DlgDirSelectComboBoxEx do have A/W families (winuser.h:11643-11687) and must bind through
    //qor_pp_useswinapiAW: those two stems are #undef'd at user32.h:15,16, so the SDK's stem -> stemW macro cannot
    //reach the binding expression and a bare qor_pp_useswinapi would ask for the literal stem, which the DLL does
    //not export (live probe against the loaded user32.dll: DlgDirListComboBox MISSING / DlgDirListComboBoxW
    //RESOLVED, likewise DlgDirSelectComboBoxEx MISSING / DlgDirSelectComboBoxExW RESOLVED). qor_pp_unicode is 1, so
    //the AW macro takes the W entry point and LPTSTR is wide - the same switch drives both halves.
    //The DlgDir pair is dialog-oriented and addresses the control by id inside hDlg. lpPathSpec is _Inout_
    //(winuser.h:11648,11657): the call rewrites the caller's buffer, which stays caller-owned and must be sized for
    //the path plus whatever text comes back; DlgDirSelectComboBoxEx writes at most nCount characters into the
    //caller-owned lpString (_Out_writes_(cchOut), winuser.h:11672,11680). Neither allocates anything, and the file
    //filter is the DDL_* set (winuser.h:11607-11616). GetComboBoxInfo needs pcbi->cbSize pre-set to
    //sizeof(COMBOBOXINFO) and returns the combo box's own child window handles (hwndItem, hwndList), which stay
    //owned by the control and must never be closed by the caller.
    //Both DlgDir wrappers carry the folder's T suffix (the SDK stem + T spelling used by 108 of the library's A/W
    //wrappers) because LPTSTR is the switchable TCHAR type and the binding follows qor_pp_unicode; GetComboBoxInfo
    //has no A/W family so it keeps the bare SDK name. Renamed 29 Oct 2026 in this TU's session - neither name had a
    //call site anywhere in src/examples/test. The 11 other A/W wrappers declared without the suffix are tracked as
    //open item U17 for one folder-wide decision.
    //Section coverage is complete: those three are the only *Combo*-named entry points in the user32.dll table.

    int User32::DlgDirListComboBoxT(HWND hDlg, LPTSTR lpPathSpec, int nIDComboBox, int nIDStaticPath, UINT uFiletype)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DlgDirListComboBox);
        return Library::Call<int, HWND, LPTSTR, int, int, UINT>(pFunc, hDlg, lpPathSpec, nIDComboBox, nIDStaticPath, uFiletype);
    }

    BOOL User32::DlgDirSelectComboBoxExT(HWND hDlg, LPTSTR lpString, int nCount, int nIDComboBox)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DlgDirSelectComboBoxEx);
        return Library::Call<BOOL, HWND, LPTSTR, int, int>(pFunc, hDlg, lpString, nCount, nIDComboBox);
    }

    BOOL User32::GetComboBoxInfo(HWND hwndCombo, PCOMBOBOXINFO pcbi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetComboBoxInfo);
        return Library::Call<BOOL, HWND, PCOMBOBOXINFO>(pFunc, hwndCombo, pcbi);
    }

}}}//qor::win::api
