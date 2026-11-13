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
    //the path plus whatever text comes back; DlgDirSelectComboBoxEx writes at most cchOut characters into the
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

    //Return policy (U2, table row DlgDirListComboBox / combobox.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZero< int >, and the table's verbatim docQuote is "Type: int If the function succeeds, the return value is
    //nonzero. If the function fails, the return value is zero. For example, if the string specified by lpPathSpec is not
    //a valid path, the function fails. To get extended error information, call GetLastError ." - the answer this stem
    //produces is the rewritten lpPathSpec buffer the caller owns, so the int is that status word alone and zero is
    //reserved for failure; the session-27 soundness test (a checked-failure site is only sound where the SDK's failure
    //value is not also a legal success value) passes at every value, and the int row takes the CheckNonZero template
    //(returncheck.h:187, Test at :192 is (value == 0) ? false : true) the vocabulary already offers - no new check class.
    //The row is an AW binding whose prototype the census could not resolve (NO-PROTOTYPE), so the class base is the
    //published int this wrapper declares rather than an SDK spelling. 0 in-tree callers (D14 published surface), so the
    //new throw is confined to that surface; the wrapper still forwards all five arguments as the caller wrote them.
    int User32::DlgDirListComboBoxT(HWND hDlg, LPTSTR lpPathSpec, int nIDComboBox, int nIDStaticPath, UINT uFiletype)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, DlgDirListComboBox);
        CheckReturn< int, CheckNonZero< int > >::TType nResult = Library::Call<int, HWND, LPTSTR, int, int, UINT>(pFunc, hDlg, lpPathSpec, nIDComboBox, nIDStaticPath, uFiletype);
        return nResult;
    }

    //Return policy (U2, table row DlgDirSelectComboBoxEx / combobox.cpp, 12 Nov 2026): the row reads UNCHECKED with no
    //check class, and the table's verbatim docQuote is "Type: BOOL If the current selection is a directory name, the
    //return value is nonzero. If the current selection is not a directory name, the return value is zero. To get
    //extended error information, call GetLastError ." - the row's note records "zero is an answer not a failure", the
    //selection being a file rather than a directory is an ordinary question the caller asked, so the session-27 test
    //fails at that value and a check would report a legal answer as a failure: this site keeps 0 CheckReturn and hands
    //the value over as written. AW binding, NO-PROTOTYPE, BOOL.
    BOOL User32::DlgDirSelectComboBoxExT(HWND hwndDlg, LPTSTR lpString, int cchOut, int idComboBox)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, DlgDirSelectComboBoxEx);
        return Library::Call<BOOL, HWND, LPTSTR, int, int>(pFunc, hwndDlg, lpString, cchOut, idComboBox);
    }

    //Return policy (U2, table row GetComboBoxInfo / combobox.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero.
    //If the function fails, the return value is zero. To get extended error information, call GetLastError ." - the
    //answer this stem produces is the PCBOMBOBOXINFO the caller hands in and the call fills in, so the BOOL is pure
    //status and zero is reserved for failure; the session-27 soundness test passes at every value, and BOOL + zero-
    //means-failure takes the BoolCheck the library already has (returncheck.h:94, Test at :99 is value ? true : false)
    //- no new check class. 3 in-tree callers, re-read before this token moved: ui/gui/controllers/a_h/comboboxcontroller
    //cpp:114 folds this BOOL into its own bool return, :149 and :184 call the wrapper for the filled-in cbi and drop
    //the return. All three pass the arguments this wrapper forwards unchanged, so the new throw only replaces their
    //FALSE path with a continuable issue the Win32ErrorHandler above reports - the two discarding callers gain that
    //report where they previously had silence; the wrapper still forwards both arguments as the caller wrote them.
    BOOL User32::GetComboBoxInfo(HWND hwndCombo, PCOMBOBOXINFO pcbi)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetComboBoxInfo);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, PCOMBOBOXINFO>(pFunc, hwndCombo, pcbi);
        return bResult;
    }

}}}//qor::win::api
