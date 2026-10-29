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

    BOOL User32::AddClipboardFormatListener(HWND hwnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, AddClipboardFormatListener);
        return Library::Call< BOOL, HWND >(pFunc, hwnd);
    }

    BOOL User32::ChangeClipboardChain(HWND hWndRemove, HWND hWndNewNext)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ChangeClipboardChain);
        return Library::Call< BOOL, HWND, HWND >(pFunc, hWndRemove, hWndNewNext);
    }

    BOOL User32::CloseClipboard(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CloseClipboard);
        return Library::Call< BOOL >(pFunc);
    }

    int User32::CountClipboardFormats(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CountClipboardFormats);
        return Library::Call< int >(pFunc);
    }

    BOOL User32::EmptyClipboard(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EmptyClipboard);
        return Library::Call< BOOL >(pFunc);
    }

    UINT User32::EnumClipboardFormats(UINT format)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EnumClipboardFormats);
        return Library::Call< UINT, UINT >(pFunc, format);
    }

    HANDLE User32::GetClipboardData(UINT uFormat)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetClipboardData);
        return Library::Call< HANDLE, UINT >(pFunc, uFormat);
    }

    int User32::GetClipboardFormatNameT(UINT format, LPTSTR lpszFormatName, int cchMaxCount)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetClipboardFormatName);
        return Library::Call< int, UINT, LPTSTR, int >(pFunc, format, lpszFormatName, cchMaxCount);
    }

    HWND User32::GetClipboardOwner(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetClipboardOwner);
        return Library::Call< HWND >(pFunc);
    }

    DWORD User32::GetClipboardSequenceNumber(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetClipboardSequenceNumber);
        return Library::Call< DWORD >(pFunc);
    }

    HWND User32::GetClipboardViewer(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetClipboardViewer);
        return Library::Call< HWND >(pFunc);
    }

    HWND User32::GetOpenClipboardWindow(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetOpenClipboardWindow);
        return Library::Call< HWND >(pFunc);
    }

    int User32::GetPriorityClipboardFormat(UINT* paFormatPriorityList, int cFormats)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPriorityClipboardFormat);
        return Library::Call< int, UINT*, int >(pFunc, paFormatPriorityList, cFormats);
    }

    BOOL User32::GetUpdatedClipboardFormats(PUINT lpuiFormats, UINT cFormats, PUINT pcFormatsOut)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetUpdatedClipboardFormats);
        return Library::Call< BOOL, PUINT, UINT, PUINT >(pFunc, lpuiFormats, cFormats, pcFormatsOut);
    }

    BOOL User32::IsClipboardFormatAvailable(UINT format)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsClipboardFormatAvailable);
        return Library::Call< BOOL, UINT >(pFunc, format);
    }

    BOOL User32::OpenClipboard(HWND hWndNewOwner)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, OpenClipboard);
        return Library::Call< BOOL, HWND >(pFunc, hWndNewOwner);
    }

    UINT User32::RegisterClipboardFormatT(LPCTSTR lpszFormat)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, RegisterClipboardFormat);
        return Library::Call< UINT, LPCTSTR >(pFunc, lpszFormat);
    }

    BOOL User32::RemoveClipboardFormatListener(HWND hwnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, RemoveClipboardFormatListener);
        return Library::Call< BOOL, HWND >(pFunc, hwnd);
    }

    HANDLE User32::SetClipboardData(UINT uFormat, HANDLE hMem)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetClipboardData);
        return Library::Call< HANDLE, UINT, HANDLE >(pFunc, uFormat, hMem);
    }

    HWND User32::SetClipboardViewer(HWND hWndNewViewer)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetClipboardViewer);
        return Library::Call< HWND, HWND >(pFunc, hWndNewViewer);
    }

}}}//qor::win::api
