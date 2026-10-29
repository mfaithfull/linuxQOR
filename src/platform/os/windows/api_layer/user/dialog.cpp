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

    HWND User32::CreateDialogIndirectParamT(HINSTANCE hInstance, LPCDLGTEMPLATE lpTemplate, HWND hWndParent, ::DLGPROC lpDialogFunc, LPARAM lParamInit)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CreateDialogIndirectParam);
        return Library::Call<HWND, HINSTANCE, LPCDLGTEMPLATE, HWND, DLGPROC, LPARAM>(pFunc, hInstance, lpTemplate, hWndParent, lpDialogFunc, lParamInit);
    }

    HWND User32::CreateDialogParamT(HINSTANCE hInstance, LPCTSTR lpTemplateName, HWND hWndParent, ::DLGPROC lpDialogFunc, LPARAM dwInitParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CreateDialogParam);
        return Library::Call<HWND, HINSTANCE, LPCTSTR, HWND, DLGPROC, LPARAM>(pFunc, hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
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
        return Library::Call<INT_PTR, HINSTANCE, LPCTSTR, HWND, DLGPROC, LPARAM>(pFunc, hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
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
        qor_pp_useswinapi(user32, GetDlgItemTextT);
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
        qor_pp_useswinapi(user32, MessageBox);
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
