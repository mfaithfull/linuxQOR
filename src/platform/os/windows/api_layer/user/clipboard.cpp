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

    //Twenty wrappers, one per declaration in the "Clipboard functions" section (user32.h:107-126), in header order.
    //Eighteen bind the bare stem because they are single user32.dll exports with no A/W family: the clipboard block
    //winuser.h:5418-5587 declares each of them directly and defines no stem -> stemW macro for any of them, so the
    //stem is the literal name GetProcAddress is given. Only GetClipboardFormatName and RegisterClipboardFormat have
    //A/W families (winuser.h:5494-5540); those two bind qor_pp_useswinapiAW and carry the T suffix. qor_pp_unicode is 1
    //(configuration.h:13), so kernel32.h:7-10 defines UNICODE: the AW macro picks the W entry point and
    //LPTSTR/LPCTSTR are wide - the same switch drives both, so a wrapper cannot end up wide-named with narrow
    //parameters.
    //Affinity and ownership stay the caller's contract and are deliberately not enforced here: OpenClipboard takes
    //the clipboard for the calling thread's task (hWndNewOwner NULL = current task), fails while another process
    //holds it, and must be paired with CloseClipboard on that same thread; EmptyClipboard only works while the
    //clipboard is open and owned; SetClipboardData hands hMem to the system on success and leaves it with the caller
    //when it fails; GetClipboardData returns a system-owned handle that must not be freed, written, or used after
    //the clipboard content changes. GetPriorityClipboardFormat signals failure with 0 and "none of the listed
    //formats is available" with -1, i.e. the opposite polarity to the CheckNonZero idiom used in api_layer/shell
    //and api_layer/kernel, so a naive non-zero return check on that one would raise issues on success paths.
    //Section coverage against the public SDK: 20 of the 21 entry points winuser.h declares; GetClipboardMetadata
    //(:5487-5492, GETCLIPBMETADATA) is the only unwrapped one. The viewer-chain trio SetClipboardViewer /
    //GetClipboardViewer / ChangeClipboardChain is deprecated in favour of the AddClipboardFormatListener /
    //RemoveClipboardFormatListener pair (WM_CLIPBOARDUPDATE); both halves are exposed, picking between them is the
    //caller's job.

    //Return policy (U2, table row AddClipboardFormatListener / clipboard.cpp, 12 Nov 2026): the row reads checked with
    //checkClass BoolCheck, and the table's verbatim docQuote is "Type: BOOL Returns TRUE if successful, FALSE otherwise.
    //Call GetLastError for additional details." - FALSE is reserved for the documented failure and this member answers
    //nothing other than the registration's success, so the session-27 soundness test (a checked-failure site is only
    //sound where the SDK's failure value is not also a legal success value) passes at every value; BOOL + zero-means-failure
    //takes the BoolCheck the library already has (returncheck.h:94, Test at :101 is value ? true : false) - no new check
    //class. sdkRetType BOOL so the base is BOOL. 0 in-tree callers for the exact stem (D14 published surface), so nothing
    //built today reaches this site; G6 keeps the check live on every call, so a failed registration now raises a continuable
    //issue the Win32ErrorHandler above reports instead of travelling as a silent success (status.md U7).
    BOOL User32::AddClipboardFormatListener(HWND hwnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, AddClipboardFormatListener);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND >(pFunc, hwnd);
        return bResult;
    }

    //Return policy (U2, table row ChangeClipboardChain / clipboard.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "Type: BOOL The return value indicates the result of passing the
    //WM_CHANGECBCHAIN message to the windows in the clipboard viewer chain. Because a window in the chain typically
    //returns FALSE when it processes WM_CHANGECBCHAIN , the return value from ChangeClipboardChain is typically FALSE ." -
    //FALSE is the ordinary legal answer of a successful chain walk rather than a reported failure, so keying a check on it
    //would report a legal chain walk as a failure and the session-27 soundness test fails at that value; the row is
    //unchecked on the page's own wording and the value comes back exactly as the OS wrote it.
    BOOL User32::ChangeClipboardChain(HWND hWndRemove, HWND hWndNewNext)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ChangeClipboardChain);
        return Library::Call< BOOL, HWND, HWND >(pFunc, hWndRemove, hWndNewNext);
    }

    //Return policy (U2, table row CloseClipboard / clipboard.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero.
    //If the function fails, the return value is zero. To get extended error information, call GetLastError ." - zero is
    //reserved for the failure and this member carries no answer, so the session-27 soundness test passes at every value;
    //BOOL + zero-means-failure takes BoolCheck (returncheck.h:94) - no new check class. 0 in-tree callers for the exact
    //stem (D14 published surface); G6 keeps the check live on every call, so a failed release now raises a continuable
    //issue the Win32ErrorHandler above reports rather than travelling as a silent success (status.md U7).
    BOOL User32::CloseClipboard(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CloseClipboard);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL >(pFunc);
        return bResult;
    }

    //Return policy (U2, table row CountClipboardFormats / clipboard.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "Type: int If the function succeeds, the return value is the number
    //of different data formats currently on the clipboard. If the function fails, the return value is zero. To get
    //extended error information, call GetLastError ." - zero sits on both sides: an open clipboard holding no formats is
    //a legal success that answers 0, so the session-27 test fails at that value and the count is handed back untouched
    //(the same zero-is-legal shape as the GetWindowTextLength count rows).
    int User32::CountClipboardFormats(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CountClipboardFormats);
        return Library::Call< int >(pFunc);
    }

    //Return policy (U2, table row EmptyClipboard / clipboard.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is word-for-word the CloseClipboard page's status line "Type: BOOL If
    //the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get
    //extended error information, call GetLastError ." - zero is reserved and an emptied clipboard is not this member's
    //answer to carry, so the session-27 soundness test passes at every value; BOOL + zero-means-failure takes BoolCheck
    //(returncheck.h:94) - no new check class. 0 in-tree callers for the exact stem; G6 keeps the check live on every
    //call, so a failed empty now raises a continuable issue the Win32ErrorHandler above reports (status.md U7).
    BOOL User32::EmptyClipboard(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EmptyClipboard);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL >(pFunc);
        return bResult;
    }

    //Return policy (U2, table row EnumClipboardFormats / clipboard.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote ends "If there are no more clipboard formats to enumerate, the return
    //value is zero. In this case, the GetLastError function returns the value ERROR_SUCCESS . This lets you distinguish
    //between function failure and the end of enumeration." - the page itself says zero is the legal end-of-enumeration
    //answer, so keying a check on it would report a finished enumeration as a failure and the session-27 soundness test
    //fails at that value; the format the caller is enumerating comes back exactly as the OS wrote it.
    UINT User32::EnumClipboardFormats(UINT format)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EnumClipboardFormats);
        return Library::Call< UINT, UINT >(pFunc, format);
    }

    //Return policy (U2, table row GetClipboardData / clipboard.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HANDLE >, and the table's verbatim docQuote is "Type: HANDLE If the function succeeds, the
    //return value is the handle to a clipboard object in the specified format. If the function fails, the return value
    //is NULL . To get extended error information, call GetLastError ." - NULL is reserved and every success answer is a
    //clipboard-object handle, so no legal answer collides with the tested value and the session-27 test passes; sdkRetType
    //HANDLE so the base is HANDLE, and CheckNonZeroHandle (returncheck.h:136, Test at :144 is (value == 0x00000000) ?
    //false : true) is the handle template the vocabulary already offers - no new check class. 0 in-tree callers for the
    //exact stem (D14 published surface); G6 keeps the check live on every call, so a NULL now raises a continuable issue
    //the Win32ErrorHandler above reports rather than travelling as a null object handle (status.md U7).
    HANDLE User32::GetClipboardData(UINT uFormat)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetClipboardData);
        CheckReturn< HANDLE, CheckNonZeroHandle< HANDLE > >::TType hResult = Library::Call< HANDLE, UINT >(pFunc, uFormat);
        return hResult;
    }

    //Return policy (U2, table row GetClipboardFormatName / clipboard.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZero< int >, and the table's verbatim docQuote is "Type: int If the function succeeds, the return value is
    //the length, in characters, of the string copied to the buffer. If the function fails, the return value is zero,
    //indicating that the requested format does not exist or is predefined. To get extended error information, call
    //GetLastError ." - the page names zero as the failure value and a successful copy length is never 0, so the session-27
    //test passes at every value; sdkRetType is NO-PROTOTYPE (this member is in the A/W family, so the bare stem is not
    //declared in the SDK) and pubRetType is int, so the class base is the declared int and CheckNonZero (returncheck.h:186,
    //Test at :194 is (value == 0) ? false : true) covers it - no new check class. 0 in-tree callers for the exact stem;
    //G6 keeps the check live on every call, so a zero length now raises a continuable issue the Win32ErrorHandler above
    //reports rather than travelling as an empty name buffer (status.md U7).
    int User32::GetClipboardFormatNameT(UINT format, LPTSTR lpszFormatName, int cchMaxCount)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, GetClipboardFormatName);
        CheckReturn< int, CheckNonZero< int > >::TType iResult = Library::Call< int, UINT, LPTSTR, int >(pFunc, format, lpszFormatName, cchMaxCount);
        return iResult;
    }

    //Return policy (U2, table row GetClipboardOwner / clipboard.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "Type: HWND If the function succeeds, the return value is the handle
    //to the window that owns the clipboard. If the clipboard is not owned, the return value is NULL . To get extended
    //error information, call GetLastError ." - an unowned clipboard is a legal state that answers NULL, so a zero-keyed
    //check reports that legal state as a failure and the session-27 test fails at that value; the owner handle comes back
    //exactly as the OS wrote it.
    HWND User32::GetClipboardOwner(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetClipboardOwner);
        return Library::Call< HWND >(pFunc);
    }

    //Return policy (U2, table row GetClipboardSequenceNumber / clipboard.cpp, 12 Nov 2026): the row reads unchecked with
    //no checkClass, and the table's verbatim docQuote is "Type: DWORD The return value is the clipboard sequence number.
    //If you do not have WINSTA_ACCESSCLIPBOARD access to the window station, the function returns zero." - the page names
    //no "If the function fails" value at all: the number is a counter the caller only compares for change, so every value
    //including 0 is a legal answer and zero doubles as the no-access signal. Step 2 of D51's process (an API that reserves
    //no return value for errors) bars a check and the counter comes back as written.
    DWORD User32::GetClipboardSequenceNumber(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetClipboardSequenceNumber);
        return Library::Call< DWORD >(pFunc);
    }

    //Return policy (U2, table row GetClipboardViewer / clipboard.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "Type: HWND If the function succeeds, the return value is the handle
    //to the first window in the clipboard viewer chain. If there is no clipboard viewer, the return value is NULL . To
    //get extended error information, call GetLastError ." - a chain with no viewer is a legal state answering NULL, so a
    //zero-keyed check reports that legal state as a failure and the session-27 test fails at that value.
    HWND User32::GetClipboardViewer(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetClipboardViewer);
        return Library::Call< HWND >(pFunc);
    }

    //Return policy (U2, table row GetOpenClipboardWindow / clipboard.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "Type: HWND If the function succeeds, the return value is the handle
    //to the window that has the clipboard open. If no window has the clipboard open, the return value is NULL . To get
    //extended error information, call GetLastError ." - an unowned clipboard is a legal state answering NULL, the same
    //shape as the GetClipboardOwner row, so the session-27 test fails at that value and the handle comes back as written.
    HWND User32::GetOpenClipboardWindow(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetOpenClipboardWindow);
        return Library::Call< HWND >(pFunc);
    }

    //Return policy (U2, table row GetPriorityClipboardFormat / clipboard.cpp, 12 Nov 2026): the row reads unchecked with
    //no checkClass, and the table's verbatim docQuote opens "Type: int If the function succeeds, the return value is the
    //first clipboard format in the list for which data is available. If the clipboard is empty, the return value is NULL."
    //and the page's next sentence gives -1 for "the clipboard contains data, but not in any of the specified formats" -
    //0 and -1 are both legal answers to the caller's list question (empty clipboard, and no listed format available), so
    //neither is a reserved failure value and the session-27 test fails at both; this is the polarity the folder header
    //notes against the CheckNonZero idiom. The answer comes back exactly as the OS wrote it.
    int User32::GetPriorityClipboardFormat(UINT* paFormatPriorityList, int cFormats)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPriorityClipboardFormat);
        return Library::Call< int, UINT*, int >(pFunc, paFormatPriorityList, cFormats);
    }

    //Return policy (U2, table row GetUpdatedClipboardFormats / clipboard.cpp, 12 Nov 2026): the row reads checked with
    //checkClass BoolCheck, and the table's verbatim docQuote is "Type: BOOL The function returns TRUE if successful;
    //otherwise, FALSE . Call GetLastError for additional details." - FALSE is reserved for the documented failure and the
    //format list this member produces is an out-parameter rather than the return, so the session-27 soundness test passes
    //at every value; BOOL + zero-means-failure takes BoolCheck (returncheck.h:94) - no new check class. 0 in-tree callers
    //for the exact stem; G6 keeps the check live on every call, so a failed refresh now raises a continuable issue the
    //Win32ErrorHandler above reports (status.md U7).
    BOOL User32::GetUpdatedClipboardFormats(PUINT lpuiFormats, UINT cFormats, PUINT pcFormatsOut)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetUpdatedClipboardFormats);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, PUINT, UINT, PUINT >(pFunc, lpuiFormats, cFormats, pcFormatsOut);
        return bResult;
    }

    //Return policy (U2, table row IsClipboardFormatAvailable / clipboard.cpp, 12 Nov 2026): the row reads unchecked with
    //no checkClass, and the table's verbatim docQuote is "Type: BOOL If the clipboard format is available, the return
    //value is nonzero. If the clipboard format is not available, the return value is zero. To get extended error
    //information, call GetLastError ." - the zero answers the caller's question about the format's availability rather
    //than reporting a failure, so a check keyed on it would report a legal answer as a failure and the session-27 test
    //fails at that value; same shape as the IsWinEventHookInstalled row in accessibility.cpp.
    BOOL User32::IsClipboardFormatAvailable(UINT format)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsClipboardFormatAvailable);
        return Library::Call< BOOL, UINT >(pFunc, format);
    }

    //Return policy (U2, table row OpenClipboard / clipboard.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is word-for-word the CloseClipboard page's status line "Type: BOOL If
    //the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get
    //extended error information, call GetLastError ." - zero is reserved for the failure (another process still holding
    //the clipboard is one of them) and this member answers nothing besides taking the clipboard for the calling task, so
    //the session-27 soundness test passes at every value; BOOL + zero-means-failure takes BoolCheck (returncheck.h:94)
    // - no new check class. 0 in-tree callers for the exact stem; G6 keeps the check live on every call, so a refused
    //open now raises a continuable issue the Win32ErrorHandler above reports rather than leaving the caller to assume
    //it holds the clipboard (status.md U7).
    BOOL User32::OpenClipboard(HWND hWndNewOwner)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, OpenClipboard);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND >(pFunc, hWndNewOwner);
        return bResult;
    }

    //Return policy (U2, table row RegisterClipboardFormat / clipboard.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZero< UINT >, and the table's verbatim docQuote is "Type: UINT If the function succeeds, the return value
    //identifies the registered clipboard format. If the function fails, the return value is zero. To get extended error
    //information, call GetLastError ." - zero is reserved and a registered format id is never 0, so the session-27 test
    //passes at every value; sdkRetType is NO-PROTOTYPE (this member is in the A/W family, so the bare stem is not declared
    //in the SDK) and pubRetType is UINT, so the class base is the declared UINT and CheckNonZero (returncheck.h:186,
    //Test at :194 is (value == 0) ? false : true) covers it - no new check class. 0 in-tree callers for the exact stem;
    //G6 keeps the check live on every call, so a zero id now raises a continuable issue the Win32ErrorHandler above
    //reports rather than travelling as a format id the caller would pass on (status.md U7).
    UINT User32::RegisterClipboardFormatT(LPCTSTR lpszFormat)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, RegisterClipboardFormat);
        CheckReturn< UINT, CheckNonZero< UINT > >::TType uResult = Library::Call< UINT, LPCTSTR >(pFunc, lpszFormat);
        return uResult;
    }

    //Return policy (U2, table row RemoveClipboardFormatListener / clipboard.cpp, 12 Nov 2026): the row reads checked with
    //checkClass BoolCheck, and the table's verbatim docQuote is word-for-word the AddClipboardFormatListener page's status
    //line "Type: BOOL Returns TRUE if successful, FALSE otherwise. Call GetLastError for additional details." - FALSE is
    //reserved for the documented failure and the withdrawal carries no answer, so the session-27 soundness test passes at
    //every value; this row takes the same BoolCheck (returncheck.h:94) as its pair - no new check class. 0 in-tree callers
    //for the exact stem; G6 keeps the check live on every call, so a failed removal now raises a continuable issue the
    //Win32ErrorHandler above reports (status.md U7).
    BOOL User32::RemoveClipboardFormatListener(HWND hwnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, RemoveClipboardFormatListener);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND >(pFunc, hwnd);
        return bResult;
    }

    //Return policy (U2, table row SetClipboardData / clipboard.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HANDLE >, and the table's verbatim docQuote is "Type: HANDLE If the function succeeds, the return
    //value is the handle to the data. If the function fails, the return value is NULL . To get extended error information,
    //call GetLastError ." - NULL is reserved and every success answer is the data handle the caller passed in, so no legal
    //answer collides with the tested value and the session-27 test passes; sdkRetType HANDLE so the base is HANDLE and the
    //row takes the same CheckNonZeroHandle< HANDLE > (returncheck.h:136) as the GetClipboardData row - no new check class.
    //0 in-tree callers for the exact stem; G6 keeps the check live on every call, so a NULL now raises a continuable issue
    //the Win32ErrorHandler above reports rather than leaving the caller to assume the data is on the clipboard (status.md U7).
    HANDLE User32::SetClipboardData(UINT uFormat, HANDLE hMem)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetClipboardData);
        CheckReturn< HANDLE, CheckNonZeroHandle< HANDLE > >::TType hResult = Library::Call< HANDLE, UINT, HANDLE >(pFunc, uFormat, hMem);
        return hResult;
    }

    //Return policy (U2, table row SetClipboardViewer / clipboard.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "Type: HWND If the function succeeds, the return value identifies
    //the next window in the clipboard viewer chain. If an error occurs or there are no other windows in the clipboard
    //viewer chain, the return value is NULL." - the sentence puts NULL on both sides of the error test: the end of the
    //viewer chain is a legal state answering NULL, so a zero-keyed check would report a complete chain as a failure and
    //the session-27 test fails at that value.
    HWND User32::SetClipboardViewer(HWND hWndNewViewer)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetClipboardViewer);
        return Library::Call< HWND, HWND >(pFunc, hWndNewViewer);
    }

}}}//qor::win::api
