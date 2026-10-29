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

    BOOL User32::CloseClipboard()
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CloseClipboard);
        return Library::Call< BOOL >(pFunc);
    }

    int User32::CountClipboardFormats()
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

    HWND User32::GetOpenClipboardWindow()
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
