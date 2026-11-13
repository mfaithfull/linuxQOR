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

    //The "Dialog functions" section (user32.h:469-493): 22 declarations, 22 definitions here, in the header's order,
    //all static. 13 of the names are A/W families in user32.dll and every one of them is bound as the W half with
    //qor_pp_useswinapiAW: CreateDialogIndirectParam W 68 / A 66 (plus the internal CreateDialogIndirectParamAorW 67),
    //CreateDialogParam W 6A / A 69, DefDlgProc A0 - exported as a forwarder to NTDLL.NtdllDialogWndProc_W, so the
    //binding resolves through user32 while the code lives in ntdll, DialogBoxIndirectParam W B8 / A B6 (AorW B7),
    //DialogBoxParam W BA / A B9, GetDlgItemText W 151 / A 150, IsDialogMessage W 233 / A 232, MessageBox W 28B / A 284,
    //MessageBoxEx W 286 / A 285, MessageBoxIndirect W 288 / A 287, MessageBoxTimeout W 28A / A 289,
    //SendDlgItemMessage W 312 / A 311, SetDlgItemText W 338 / A 337. The other 9 are single exports with no A/W
    //spelling at all and are bound with the bare stem: EndDialog F2, GetDialogBaseUnits 148, GetDlgCtrlID 14D,
    //GetDlgItem 14E, GetDlgItemInt 14F, GetNextDlgGroupItem 18F, GetNextDlgTabItem 190, MapDialogRect 278,
    //SetDlgItemInt 336. Ordinals are from tmp/_tmp_user32_exports.txt; every literal was read out of cl /EP output and
    //each name was then resolved by GetProcAddress in a live probe (tmp/_tmp_name_resolve_probe.cpp), not inferred
    //from the SDK. Two entries that must not be guessed at: IsDialogMessage is also exported bare (231) but at the
    //same RVA as IsDialogMessageA, so the stem is the ANSI half and binding W is what matches the LPTSTR-visible
    //signature this wrapper publishes; and MessageBoxTimeout has no declaration anywhere in the installed SDK, so the
    //export table and the probe are the only authority for its name and parameter list (status.md U16).
    //Contracts passed through unchanged: DLGPROC is called by user32, not by us, so callbacks handed in must keep the
    //SDK calling convention; EndDialog's INT_PTR becomes the DialogBox* return value; GetDlgCtrlID returns int and
    //callers narrowing it to unsigned short truncate deliberately; GetDialogBaseUnits returns packed font metrics,
    //never a failure code; the MessageBox* family returns a button ID or 0, and 0 also means failure (status.md U2).

    //Return policy (U2, table row CreateDialogIndirectParam / dialog.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HWND >, and the table's verbatim docQuote is "Type: HWND If the function succeeds, the return value is the
    //window handle to the dialog box. If the function fails, the return value is NULL . To get extended error information, call
    //GetLastError ." - NULL is reserved and the success answer is a window handle, which is never NULL, so the session-27 soundness
    //test (a checked-failure site is only sound where the SDK's failure value is not also a legal success value) passes at every
    //value; the AW row has NO-PROTOTYPE sdkRetType so the class base is the published HWND, and CheckNonZeroHandle (returncheck.h:137,
    //Test at :144 is (value == 0x00000000) ? false : true) is already in the library - no new check class. 0 in-tree callers for the
    //exact stem (D14 published surface), so nothing built today reaches this site; G6 keeps the check live on every call, so a failed
    //creation now raises a continuable issue the Win32ErrorHandler above reports rather than travelling as a silent NULL handle.
    HWND User32::CreateDialogIndirectParamT(HINSTANCE hInstance, LPCDLGTEMPLATE lpTemplate, HWND hWndParent, ::DLGPROC lpDialogFunc, LPARAM dwInitParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, CreateDialogIndirectParam);
        CheckReturn< HWND, CheckNonZeroHandle< HWND > >::TType hDialog = Library::Call<HWND, HINSTANCE, LPCDLGTEMPLATE, HWND, ::DLGPROC, LPARAM>(pFunc, hInstance, lpTemplate, hWndParent, lpDialogFunc, dwInitParam);
        return hDialog;
    }

    //Return policy (U2, table row CreateDialogParam / dialog.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HWND >, and the table's verbatim docQuote is word for word the CreateDialogIndirectParam row's - "Type: HWND
    //If the function succeeds, the return value is the window handle to the dialog box. If the function fails, the return value is
    //NULL . To get extended error information, call GetLastError ." - so NULL is reserved and the answer is a window handle that is
    //never NULL, the session-27 test passing at every value; the AW row has NO-PROTOTYPE sdkRetType so the base is the published
    //HWND and CheckNonZeroHandle (returncheck.h:137, Test at :144) is the class the library already has - no new check class. 0 in-tree
    //callers for the exact stem (D14 published surface), so nothing built today reaches this site; G6 keeps the check live on every
    //call, so a failed creation now raises a continuable issue rather than travelling as a silent NULL handle.
    HWND User32::CreateDialogParamT(HINSTANCE hInstance, LPCTSTR lpTemplateName, HWND hWndParent, ::DLGPROC lpDialogFunc, LPARAM dwInitParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, CreateDialogParam);
        CheckReturn< HWND, CheckNonZeroHandle< HWND > >::TType hDialog = Library::Call<HWND, HINSTANCE, LPCTSTR, HWND, ::DLGPROC, LPARAM>(pFunc, hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
        return hDialog;
    }

    //Return policy (U2, table row DefDlgProc / dialog.cpp, 12 Nov 2026): the row reads unchecked with no checkClass. The return is the
    //DLGPROC's answer, not a status - "The return value specifies the result of the message processing and depends on the message sent."
    //is the whole Return value section, so the page names no failure value at all and D51 step 2 (an API that reserves no return value
    //for errors) reads no-check; the value is whatever the callback the caller handed in chose to return, so no value is separable from
    //an answer. Same shape as the SendDlgItemMessage row and the DefWindowProc family in windowprocedure.cpp. 0 in-tree callers, re-measured
    //today T-tolerant - the wrapper is spelled DefDlgProcT and appears nowhere but its definition - and the value comes back exactly as the OS wrote it.
    LRESULT User32::DefDlgProcT(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, DefDlgProc);
        return Library::Call<LRESULT, HWND, UINT, WPARAM, LPARAM>(pFunc, hDlg, Msg, wParam, lParam);
    }

    //Return policy (U2, table row DialogBoxIndirectParam / dialog.cpp, 12 Nov 2026): the row reads unchecked with no checkClass. The
    //success answer is the caller's own value and it routinely lands on the documented failure value - "If the function succeeds, the
    //return value is the nResult parameter specified in the call to the EndDialog function that was used to terminate the dialog box."
    //while canonical dialog code ends with EndDialog(hDlg, FALSE), so 0 is a legal success answer; "If the function fails because the
    //hWndParent parameter is invalid, the return value is zero. The function returns zero in this case for compatibility with previous
    //versions of Windows." puts failure at that same value, so keying a nonzero test here would report a legal nResult as a failure and
    //the session-27 soundness test fails at 0; "If the function fails for any other reason, the return value is –1" sits at a value an
    //nResult can also carry. The row is unchecked on the page's own wording and the value comes back exactly as the OS wrote it. 0 in-tree
    //callers, re-measured today T-tolerant.
    INT_PTR User32::DialogBoxIndirectParamT(HINSTANCE hInstance, LPCDLGTEMPLATE hDialogTemplate, HWND hWndParent, ::DLGPROC lpDialogFunc, LPARAM dwInitParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, DialogBoxIndirectParam);
        return Library::Call<INT_PTR, HINSTANCE, LPCDLGTEMPLATE, HWND, ::DLGPROC, LPARAM>(pFunc, hInstance, hDialogTemplate, hWndParent, lpDialogFunc, dwInitParam);
    }

    //Return policy (U2, table row DialogBoxParam / dialog.cpp, 12 Nov 2026): the row reads unchecked with no checkClass. Same shape as the
    //DialogBoxIndirectParam row with the template named by name - "If the function succeeds, the return value is the value of the nResult
    //parameter specified in the call to the EndDialog function used to terminate the dialog box. If the function fails because the hWndParent
    //parameter is invalid, the return value is zero." - so the nResult the caller's DLGPROC hands to EndDialog (routinely FALSE) is a legal
    //answer sitting at the documented failure value, and "If the function fails for any other reason, the return value is –1" puts the second
    //failure value at a value an nResult can also carry. Keying any test here would report a legal nResult as a failure, so the session-27
    //soundness test fails; the row is unchecked on the page's own wording and the value comes back exactly as the OS wrote it. 0 in-tree
    //callers, re-measured today T-tolerant.
    INT_PTR User32::DialogBoxParamT(HINSTANCE hInstance, LPCTSTR lpTemplateName, HWND hWndParent, ::DLGPROC lpDialogFunc, LPARAM dwInitParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, DialogBoxParam);
        return Library::Call<INT_PTR, HINSTANCE, LPCTSTR, HWND, ::DLGPROC, LPARAM>(pFunc, hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
    }

    //Return policy (U2, table row EndDialog / dialog.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, and the
    //table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the
    //return value is zero. To get extended error information, call GetLastError ." - zero is reserved and the BOOL carries no answer
    //of its own (the dialog being closed is hDlg and the button ID to leave with is nResult, both parameters), so the session-27 test
    //passes at every value; sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94, Test at :101 is value ? true : false)
    //is the class the library already has - no new check class. AUDIT: **2** in-tree callers, both statement-level calls that discard
    //this return value outright (`ui/gui/windows/a_h/dialog.cpp:58` and `:65`, the WM_COMMAND and WM_CLOSE arms of the dialog's own
    //window procedure, each `User32::EndDialog((HWND)(hDlg), dlg ? dlg->OnEnd(...) : ...)` with no assignment and the arm then returning
    //1), so the check cannot change what the caller sees - under G6 it adds a continuable issue the Win32ErrorHandler above reports on
    //a failed close, which today fails silently.
    BOOL User32::EndDialog(HWND hDlg, INT_PTR nResult)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EndDialog);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, INT_PTR>(pFunc, hDlg, nResult);
        return bResult;
    }

    //Return policy (U2, table row GetDialogBaseUnits / dialog.cpp, 12 Nov 2026): the row reads unchecked with no checkClass. Packed value
    //answer with no documented failure value - "The function returns the dialog base units. The low-order word of the return value contains
    //the horizontal dialog box base unit, and the high-order word contains the vertical dialog box base unit." names no failure value at all,
    //so D51 step 2 reads no-check; keying on zero would split a legal small-metric pair rather than a failure. This TU's banner records the
    //same reading ("GetDialogBaseUnits returns packed font metrics, never a failure code"). sdkRetType long, 0 in-tree callers, and the value
    //comes back exactly as the OS wrote it.
    LONG User32::GetDialogBaseUnits(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetDialogBaseUnits);
        return Library::Call<LONG>(pFunc);
    }

    //Return policy (U2, table row GetDlgCtrlID / dialog.cpp, 12 Nov 2026): the row reads checked with checkClass CheckNonZero< int >,
    //and the table's verbatim docQuote is "Type: int If the function succeeds, the return value is the identifier of the control. If
    //the function fails, the return value is zero. An invalid value for the hwndCtl parameter, for example, will cause the function to
    //fail. To get extended error information, call GetLastError ." - zero is reserved and the answer is a control identifier, which is
    //never zero, so the session-27 test passes at every value; sdkRetType int so the base is the SDK type and CheckNonZero (returncheck.h:187,
    //template< typename TNumeric > : public AutoErrorCheck< TNumeric >, Test at :192 is (value == 0) ? false : true) is the class the
    //library already has - no new check class. AUDIT: **8** in-tree callers, the heaviest nonzero row in the tree - one per controller
    //(`ui/gui/controllers/{a_h/animationcontroller.cpp:46, a_h/buttoncontroller.cpp:126, a_h/comboboxcontroller.cpp:47, a_h/editcontroller.cpp:47,
    //i_m/listboxcontroller.cpp:47, i_m/mdiclientcontroller.cpp:47, s_s/scrollbarcontroller.cpp:47, s_s/staticcontroller.cpp:48}`, u19.md:70-79),
    //all re-read today and identical `return static_cast<unsigned short>(User32::GetDlgCtrlID(WINDOW_HANDLE));`; the register's line for
    //buttoncontroller has moved since session 13 (it says `:107`, which is now a closing brace, and the call sits at `:126`), so the
    //count 8 is re-measured rather than taken from the register. 7 of the 8 are
    //compiled (u19.md:79: `mdiclientcontroller.cpp` is absent from gui/CMakeLists.txt) and all 8 are compiled-but-never-invoked, so nothing
    //built today observes the new issue; the narrowing to unsigned short is caller-side and deliberate (u19.md:73-75, this file's banner at
    //:33), so the check keys on the wrapper's int, before any truncation. Under G6 a zero control id now raises a continuable issue rather
    //than reaching the controller as a silent 0 that GetId() hands on as a null id.
    int User32::GetDlgCtrlID(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetDlgCtrlID);
        CheckReturn< int, CheckNonZero< int > >::TType iCtrlId = Library::Call<int, HWND>(pFunc, hWnd);
        return iCtrlId;
    }

    //Return policy (U2, table row GetDlgItem / dialog.cpp, 12 Nov 2026): the row reads checked with checkClass CheckNonZeroHandle< HWND >,
    //and the table's verbatim docQuote is "Type: HWND If the function succeeds, the return value is the window handle of the specified
    //control. If the function fails, the return value is NULL , indicating an invalid dialog box handle or a nonexistent control. To get
    //extended error information, call GetLastError ." - the page reserves NULL for both failure states it names, and the success answer is
    //a control's window handle, which is never NULL, so the session-27 test passes at every value; sdkRetType HWND so the base is the SDK
    //type and CheckNonZeroHandle (returncheck.h:137, Test at :144) is the class the library already has - no new check class. 0 in-tree
    //callers for the exact stem (D14 published surface), so nothing built today reaches this site; G6 keeps the check live on every call,
    //so a lookup that names no control now raises a continuable issue rather than handing the caller a silent NULL handle to use.
    HWND User32::GetDlgItem(HWND hDlg, int nIDDlgItem)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetDlgItem);
        CheckReturn< HWND, CheckNonZeroHandle< HWND > >::TType hControl = Library::Call<HWND, HWND, int>(pFunc, hDlg, nIDDlgItem);
        return hControl;
    }

    //Return policy (U2, table row GetDlgItemInt / dialog.cpp, 12 Nov 2026): the row reads unchecked with no checkClass. The page itself says
    //the failure value is a legal answer - "Note that, because zero is a possible translated value, a return value of zero does not by itself
    //indicate failure." - so the documented zero is not reserved; the success signal lives in the BOOL the lpTranslated parameter points at
    //("If the function fails, the variable pointed to by lpTranslated is set to FALSE"), which this layer never reads, and "If lpTranslated is
    //NULL , the function returns no information about success or failure." removes the signal entirely for that call. Keying CheckNonZero< UINT >
    //here would report a legal translated zero as a failure, so the session-27 test fails at that value; 0 in-tree callers, and the value comes
    //back exactly as the OS wrote it.
    UINT User32::GetDlgItemInt(HWND hDlg, int nIDDlgItem, BOOL* lpTranslated, BOOL bSigned)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetDlgItemInt);
        return Library::Call<UINT, HWND, int, BOOL*, BOOL>(pFunc, hDlg, nIDDlgItem, lpTranslated, bSigned);
    }

    //Return policy (U2, table row GetDlgItemText / dialog.cpp, 12 Nov 2026): the row reads unchecked with no checkClass. Zero-is-legal count -
    //the success answer is "the number of characters copied to the buffer, not including the terminating null character" and a control whose
    //title or text is empty legitimately answers 0 characters, so the documented "If the function fails, the return value is zero." is not
    //separable from a legal empty-text answer; the page's Remarks only bound the upper end ("If the string is as long or longer than the buffer,
    //the buffer will contain the truncated string with a terminating null character.") and name nothing at zero. Same zero-is-legal count shape as
    //the GetWindowTextLength row D51 names, and deliberately unlike the GetClassName row, which is checked because a window class always has a
    //name while a control never must. AW binding with NO-PROTOTYPE sdkRetType; 0 in-tree callers, and the value comes back exactly as the OS wrote it.
    UINT User32::GetDlgItemTextT(HWND hDlg, int nIDDlgItem, LPTSTR lpString, int cchMax)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, GetDlgItemText);
        return Library::Call<UINT, HWND, int, LPTSTR, int>(pFunc, hDlg, nIDDlgItem, lpString, cchMax);
    }

    //Return policy (U2, table row GetNextDlgGroupItem / dialog.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HWND >, and the table's verbatim docQuote is "Type: HWND If the function succeeds, the return value is a
    //handle to the previous (or next) control in the group of controls. If the function fails, the return value is NULL . To get
    //extended error information, call GetLastError ." - NULL is reserved and the answer is a control handle; the enumeration wraps
    //rather than ending, so the answer at the first control of a group is the last one and never NULL, so the session-27 test passes
    //at every value; sdkRetType HWND so the base is the SDK type and CheckNonZeroHandle (returncheck.h:137, Test at :144) is the class
    //the library already has - no new check class. 0 in-tree callers for the exact stem, re-measured today T-tolerant (this stem has
    //no A/W spelling, so `User32::GetNextDlgGroupItem` is the only spelling and it appears nowhere but the definition), so nothing
    //built today reaches this site; G6 keeps the check live on every call, so a walk that answers NULL now raises an issue rather
    //than ending the loop silently on a null handle.
    HWND User32::GetNextDlgGroupItem(HWND hDlg, HWND hCtl, BOOL bPrevious)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetNextDlgGroupItem);
        CheckReturn< HWND, CheckNonZeroHandle< HWND > >::TType hNext = Library::Call<HWND, HWND, HWND, BOOL>(pFunc, hDlg, hCtl, bPrevious);
        return hNext;
    }

    //Return policy (U2, table row GetNextDlgTabItem / dialog.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HWND >, and the table's verbatim docQuote is "Type: HWND If the function succeeds, the return value is the
    //window handle of the previous (or next) control that has the WS_TABSTOP style set. If the function fails, the return value is
    //NULL . To get extended error information, call GetLastError ." - NULL is reserved and the answer is a control handle, so the
    //session-27 test passes at every value; a dialog whose controls none of which set WS_TABSTOP is the caller's question rather
    //than a documented answer, which is why the page names only failure states here; sdkRetType HWND so the base is the SDK type and
    //CheckNonZeroHandle (returncheck.h:137, Test at :144) is the class the library already has - no new check class. 0 in-tree callers
    //for the exact stem (no A/W spelling, so the bare stem is the only spelling), so nothing built today reaches this site; G6 keeps
    //the check live on every call.
    HWND User32::GetNextDlgTabItem(HWND hDlg, HWND hCtl, BOOL bPrevious)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetNextDlgTabItem);
        CheckReturn< HWND, CheckNonZeroHandle< HWND > >::TType hNext = Library::Call<HWND, HWND, HWND, BOOL>(pFunc, hDlg, hCtl, bPrevious);
        return hNext;
    }

    //Return policy (U2, table row IsDialogMessage / dialog.cpp, 12 Nov 2026): the row reads unchecked with no checkClass. Answer-shaped BOOL,
    //not a status - "If the message has been processed, the return value is nonzero. If the message has not been processed, the return value is
    //zero." is the whole Return value section: the zero is the answer the caller needs (the message does not belong to the dialog window and must
    //fall through to the caller's own message loop), the page names no failure value and carries no GetLastError clause at all, so keying BoolCheck
    //would report a legal fall-through as a failure and the session-27 test fails at that value. Same shape as the AreDpiAwarenessContextsEqual
    //question rows. AW binding with NO-PROTOTYPE sdkRetType; 0 in-tree callers, re-measured today T-tolerant, and the value comes back exactly as the OS wrote it.
    BOOL User32::IsDialogMessageT(HWND hDlg, LPMSG lpMsg)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, IsDialogMessage);
        return Library::Call<BOOL, HWND, LPMSG>(pFunc, hDlg, lpMsg);
    }

    //Return policy (U2, table row MapDialogRect / dialog.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, and the
    //table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If the function fails, the return
    //value is zero. To get extended error information, call GetLastError ." - zero is reserved and the BOOL carries no answer of its
    //own: the rectangle being mapped is the lpRect out-parameter, which the caller reads back, so the session-27 test passes at every
    //value; sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94, Test at :101 is value ? true : false) is the class
    //the library already has - no new check class. 0 in-tree callers for the exact stem (no A/W spelling), so nothing built today
    //reaches this site; G6 keeps the check live on every call, so an unmapped rect now raises an issue rather than leaving the caller
    //to read back a rect the call left untouched.
    BOOL User32::MapDialogRect(HWND hDlg, LPRECT lpRect)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, MapDialogRect);
        CheckReturn< BOOL, BoolCheck >::TType bMapped = Library::Call<BOOL, HWND, LPRECT>(pFunc, hDlg, lpRect);
        return bMapped;
    }

    //Return policy (U2, table row MessageBox / dialog.cpp, 12 Nov 2026): the row reads checked with checkClass CheckNonZero< int >, and
    //the table's verbatim docQuote reserves zero while enumerating a success set that contains no zero - "If the function fails, the
    //return value is zero. To get extended error information, call GetLastError . If the function succeeds, the return value is one of
    //the following menu-item values ... IDOK 1 IDCANCEL 2 IDABORT 3 IDRETRY 4 IDIGNORE 5 IDYES 6 IDNO 7 IDTRYAGAIN 10 IDCONTINUE 11" -
    //so the session-27 test passes at every value; the message text, caption and uType are parameters, so the int carries no answer
    //other than the button the user chose. AW binding with NO-PROTOTYPE sdkRetType, so the class base is the published int the wrapper
    //holds, and CheckNonZero (returncheck.h:187, Test at :192 is (value == 0) ? false : true) is the class the library already has -
    //no new check class. AUDIT: the table's 0 counts the bare stem only. This wrapper is spelled MessageBoxT, and a T-tolerant re-count
    //finds **1** site - `ui/gui/windows/i_m/messagebox.cpp:41` `m_result = User32::MessageBoxT(...)`, which the qor::platform::win::MessageBox
    //ctor stores in m_result and `MessageBox::Result()` (messagebox.cpp:57) hands back; u19.md:89-92 records that the only such
    //constructions are `examples/.../button/button.cpp:51` and `.../dialogs/dialogs.cpp:44,57`, both commented out of the build, so the
    //site is compiled-and-reachable rather than live. Under G6 a failed box now raises an issue instead of reaching Result() as a silent
    //0 that no menu-item value can be.
    int User32::MessageBoxT(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, MessageBox);
        CheckReturn< int, CheckNonZero< int > >::TType iButton = Library::Call<int, HWND, LPCTSTR, LPCTSTR, UINT>(pFunc, hWnd, lpText, lpCaption, uType);
        return iButton;
    }

    //Return policy (U2, table row MessageBoxEx / dialog.cpp, 12 Nov 2026): the row reads checked with checkClass CheckNonZero< int >, and
    //the table's verbatim docQuote is word for word the MessageBox row's - zero reserved ("If the function fails, the return value is
    //zero.") and the success set enumerated with no zero in it - so the session-27 test passes at every value; the extra wLanguageId here
    //is a parameter, so the int carries no answer other than the button chosen. AW binding with NO-PROTOTYPE sdkRetType, so the class base
    //is the published int the wrapper holds and CheckNonZero (returncheck.h:187, Test at :192) is the class the library already has -
    //no new check class. AUDIT: 0 in-tree callers, re-measured today T-tolerant - `User32::MessageBoxExT` appears nowhere but this
    //definition, so unlike the MessageBox row above there is no reachable site for this check to change; G6 keeps it live on every call.
    int User32::MessageBoxExT(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, WORD wLanguageId)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, MessageBoxEx);
        CheckReturn< int, CheckNonZero< int > >::TType iButton = Library::Call<int, HWND, LPCTSTR, LPCTSTR, UINT, WORD>(pFunc, hWnd, lpText, lpCaption, uType, wLanguageId);
        return iButton;
    }

    //Return policy (U2, table row MessageBoxIndirect / dialog.cpp, 12 Nov 2026): the row reads checked with checkClass CheckNonZero< int >.
    //The page reserves zero ("If there is not enough memory to create the message box, the return value is zero.") and enumerates the
    //same button-ID success set the MessageBox row carries with no zero in it, so the session-27 test passes at every value; the whole
    //box description arrives through the MSGBOXPARAMS pointer, so the int carries no answer other than the button chosen. AW binding
    //with NO-PROTOTYPE sdkRetType, so the class base is the published int the wrapper holds and CheckNonZero (returncheck.h:187, Test at
    //:192) is the class the library already has - no new check class. AUDIT: the table's 0 counts the bare stem only, and a T-tolerant
    //re-count finds **1** site - `ui/gui/windows/i_m/messagebox.cpp:52` `m_result = User32::MessageBoxIndirectT(...)`, which the qor::platform::win::
    //MessageBox ctor stores in m_result and `MessageBox::Result()` (messagebox.cpp:57) hands back; u19.md:77-78 records that trio
    //(`MessageBoxT` `:41`, `MessageBoxTimeoutT` `:46`, `MessageBoxIndirectT` `:52`). The site is compiled-and-reachable rather than
    //live on a measured path, so under G6 a failed box now raises an issue instead of reaching Result() as a silent 0 no button ID can be.
    int User32::MessageBoxIndirectT(const ::MSGBOXPARAMS* lpmbp)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, MessageBoxIndirect);
        CheckReturn< int, CheckNonZero< int > >::TType iButton = Library::Call<int, const ::MSGBOXPARAMS*>(pFunc, lpmbp);
        return iButton;
    }

    //NOTE: Thanks to CPian Maurizio Pisano
    //Return policy (U2, table row MessageBoxTimeout / dialog.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, and its docQuote
    //column is "-" because the probe found no slug for it anywhere in the Learn index - there is no page to read, so no documented failure value
    //exists and D51 step 2 reads no-check. The SDK declares no prototype for it anywhere in the installed header either (this TU's banner books
    //that, and status.md U16 makes the export table and the live GetProcAddress probe the only authority for its name and parameter list). Same
    //no-page shape as the CancelShutdown, SetWindowWord and SetMessageQueue rows. The checked rows above it are checked on their pages' own wording;
    //this row has no wording to read, so the reading is taken from the absence of a page rather than from a claim about what a timeout returns.
    //0 in-tree callers, re-measured today T-tolerant, and the value comes back exactly as the OS wrote it.
    int User32::MessageBoxTimeoutT(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, WORD wLanguageId, DWORD dwMilliseconds)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, MessageBoxTimeout);
        return Library::Call<int, HWND, LPCTSTR, LPCTSTR, UINT, WORD, DWORD>(pFunc, hWnd, lpText, lpCaption, uType, wLanguageId, dwMilliseconds);
    }

    //Return policy (U2, table row SendDlgItemMessage / dialog.cpp, 12 Nov 2026): the row reads unchecked with no checkClass. The return is the
    //DLGPROC's answer, not a status - "The return value specifies the result of the message processing and depends on the message sent." is the
    //whole Return value section, so the page names no failure value and D51 step 2 reads no-check; the value is whatever the window procedure that
    //handled the message chose to return, so no value is separable from an answer. Same shape as the DefDlgProc row. AW binding with NO-PROTOTYPE
    //sdkRetType, 0 in-tree callers, and the value comes back exactly as the OS wrote it.
    LRESULT User32::SendDlgItemMessageT(HWND hDlg, int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, SendDlgItemMessage);
        return Library::Call<LRESULT, HWND, int, UINT, WPARAM, LPARAM>(pFunc, hDlg, nIDDlgItem, Msg, wParam, lParam);
    }

    //Return policy (U2, table row SetDlgItemInt / dialog.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck. Plain status
    //BOOL - "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended
    //error information, call GetLastError ." reserves zero, and the value being written is the uValue parameter, so the BOOL carries no
    //answer of its own and the session-27 test passes at every value; sdkRetType BOOL so BoolCheck (returncheck.h:94, base
    //AutoErrorCheck< BOOL >) is the class the library already has - no new check class. AUDIT: 0 in-tree callers, re-measured today
    //T-tolerant - `User32::SetDlgItemInt` appears nowhere but this definition - so the check is live on every call (G6) with no site to audit.
    BOOL User32::SetDlgItemInt(HWND hDlg, int nIDDlgItem, UINT uValue, BOOL bSigned)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetDlgItemInt);
        CheckReturn< BOOL, BoolCheck >::TType bSet = Library::Call<BOOL, HWND, int, UINT, BOOL>(pFunc, hDlg, nIDDlgItem, uValue, bSigned);
        return bSet;
    }

    //Return policy (U2, table row SetDlgItemText / dialog.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck. The page gives
    //the SetDlgItemInt row's status wording word for word, so zero is reserved and the text being written is the lpString parameter rather
    //than the return - deliberately unlike the GetDlgItemText row above, whose zero is the legal count answering how much text was read.
    //AW binding with NO-PROTOTYPE sdkRetType, so the class base is the published BOOL the wrapper holds, which is BoolCheck's own base
    //AutoErrorCheck< BOOL > (returncheck.h:94) - no new check class. AUDIT: 0 in-tree callers, re-measured today T-tolerant -
    //`User32::SetDlgItemText` appears nowhere but this definition - so the check is live on every call (G6) with no site to audit.
    BOOL User32::SetDlgItemTextT(HWND hDlg, int nIDDlgItem, LPCTSTR lpString)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, SetDlgItemText);
        CheckReturn< BOOL, BoolCheck >::TType bSet = Library::Call<BOOL, HWND, int, LPCTSTR>(pFunc, hDlg, nIDDlgItem, lpString);
        return bSet;
    }

}}}//qor::win::api
