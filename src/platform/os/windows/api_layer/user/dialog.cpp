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

    HWND User32::CreateDialogIndirectParamT(HINSTANCE hInstance, LPCDLGTEMPLATE lpTemplate, HWND hWndParent, ::DLGPROC lpDialogFunc, LPARAM lParamInit)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CreateDialogIndirectParam);
        return Library::Call<HWND, HINSTANCE, LPCDLGTEMPLATE, HWND, ::DLGPROC, LPARAM>(pFunc, hInstance, lpTemplate, hWndParent, lpDialogFunc, lParamInit);
    }

    HWND User32::CreateDialogParamT(HINSTANCE hInstance, LPCTSTR lpTemplateName, HWND hWndParent, ::DLGPROC lpDialogFunc, LPARAM dwInitParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CreateDialogParam);
        return Library::Call<HWND, HINSTANCE, LPCTSTR, HWND, ::DLGPROC, LPARAM>(pFunc, hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
    }

    LRESULT User32::DefDlgProcT(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DefDlgProc);
        return Library::Call<LRESULT, HWND, UINT, WPARAM, LPARAM>(pFunc, hDlg, Msg, wParam, lParam);
    }

    INT_PTR User32::DialogBoxIndirectParamT(HINSTANCE hInstance, LPCDLGTEMPLATE hDialogTemplate, HWND hWndParent, ::DLGPROC lpDialogFunc, LPARAM dwInitParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DialogBoxIndirectParam);
        return Library::Call<INT_PTR, HINSTANCE, LPCDLGTEMPLATE, HWND, ::DLGPROC, LPARAM>(pFunc, hInstance, hDialogTemplate, hWndParent, lpDialogFunc, dwInitParam);
    }

    INT_PTR User32::DialogBoxParamT(HINSTANCE hInstance, LPCTSTR lpTemplateName, HWND hWndParent, ::DLGPROC lpDialogFunc, LPARAM dwInitParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DialogBoxParam);
        return Library::Call<INT_PTR, HINSTANCE, LPCTSTR, HWND, ::DLGPROC, LPARAM>(pFunc, hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
    }

    BOOL User32::EndDialog(HWND hDlg, INT_PTR nResult)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EndDialog);
        return Library::Call<BOOL, HWND, INT_PTR>(pFunc, hDlg, nResult);
    }

    LONG User32::GetDialogBaseUnits(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDialogBaseUnits);
        return Library::Call<LONG>(pFunc);
    }

    int User32::GetDlgCtrlID(HWND hwndCtl)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDlgCtrlID);
        return Library::Call<int, HWND>(pFunc, hwndCtl);
    }

    HWND User32::GetDlgItem(HWND hDlg, int nIDDlgItem)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDlgItem);
        return Library::Call<HWND, HWND, int>(pFunc, hDlg, nIDDlgItem);
    }

    UINT User32::GetDlgItemInt(HWND hDlg, int nIDDlgItem, BOOL* lpTranslated, BOOL bSigned)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDlgItemInt);
        return Library::Call<UINT, HWND, int, BOOL*, BOOL>(pFunc, hDlg, nIDDlgItem, lpTranslated, bSigned);
    }

    UINT User32::GetDlgItemTextT(HWND hDlg, int nIDDlgItem, LPTSTR lpString, int nMaxCount)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetDlgItemText);
        return Library::Call<UINT, HWND, int, LPTSTR, int>(pFunc, hDlg, nIDDlgItem, lpString, nMaxCount);
    }

    HWND User32::GetNextDlgGroupItem(HWND hDlg, HWND hCtl, BOOL bPrevious)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetNextDlgGroupItem);
        return Library::Call<HWND, HWND, HWND, BOOL>(pFunc, hDlg, hCtl, bPrevious);
    }

    HWND User32::GetNextDlgTabItem(HWND hDlg, HWND hCtl, BOOL bPrevious)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetNextDlgTabItem);
        return Library::Call<HWND, HWND, HWND, BOOL>(pFunc, hDlg, hCtl, bPrevious);
    }

    BOOL User32::IsDialogMessageT(HWND hDlg, LPMSG lpMsg)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, IsDialogMessage);
        return Library::Call<BOOL, HWND, LPMSG>(pFunc, hDlg, lpMsg);
    }

    BOOL User32::MapDialogRect(HWND hDlg, LPRECT lpRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, MapDialogRect);
        return Library::Call<BOOL, HWND, LPRECT>(pFunc, hDlg, lpRect);
    }

    int User32::MessageBoxT(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, MessageBox);
        return Library::Call<int, HWND, LPCTSTR, LPCTSTR, UINT>(pFunc, hWnd, lpText, lpCaption, uType);
    }

    int User32::MessageBoxExT(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, WORD wLanguageId)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, MessageBoxEx);
        return Library::Call<int, HWND, LPCTSTR, LPCTSTR, UINT, WORD>(pFunc, hWnd, lpText, lpCaption, uType, wLanguageId);
    }

    int User32::MessageBoxIndirectT(const ::LPMSGBOXPARAMS lpMsgBoxParams)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, MessageBoxIndirect);
        return Library::Call<int, const ::LPMSGBOXPARAMS>(pFunc, lpMsgBoxParams);
    }

    //NOTE: Thanks to CPian Maurizio Pisano
    int User32::MessageBoxTimeout(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, WORD wLanguageId, DWORD dwMilliseconds)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, MessageBoxTimeout);
        return Library::Call<int, HWND, LPCTSTR, LPCTSTR, UINT, WORD, DWORD>(pFunc, hWnd, lpText, lpCaption, uType, wLanguageId, dwMilliseconds);
    }

    LRESULT User32::SendDlgItemMessageT(HWND hDlg, int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SendDlgItemMessage);
        return Library::Call<LRESULT, HWND, int, UINT, WPARAM, LPARAM>(pFunc, hDlg, nIDDlgItem, Msg, wParam, lParam);
    }

    BOOL User32::SetDlgItemInt(HWND hDlg, int nIDDlgItem, UINT uValue, BOOL bSigned)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetDlgItemInt);
        return Library::Call<BOOL, HWND, int, UINT, BOOL>(pFunc, hDlg, nIDDlgItem, uValue, bSigned);
    }

    BOOL User32::SetDlgItemTextT(HWND hDlg, int nIDDlgItem, LPCTSTR lpString)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SetDlgItemText);
        return Library::Call<BOOL, HWND, int, LPCTSTR>(pFunc, hDlg, nIDDlgItem, lpString);
    }

}}}//qor::win::api
