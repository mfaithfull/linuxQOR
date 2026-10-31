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

    BOOL User32::GetClassInfoT(HINSTANCE hInstance, LPCTSTR lpClassName, LPWNDCLASS lpWndClass)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetClassInfo);
        return Library::Call< BOOL, HINSTANCE, LPCTSTR, LPWNDCLASS >(pFunc, hInstance, lpClassName, lpWndClass);
    }

    BOOL User32::GetClassInfoExT(HINSTANCE hinst, LPCTSTR lpszClass, LPWNDCLASSEX lpwcx)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetClassInfoEx);
        return Library::Call< BOOL, HINSTANCE, LPCTSTR, LPWNDCLASSEX >(pFunc, hinst, lpszClass, lpwcx);
    }

    ULONG_PTR User32::GetClassLongPtrT(HWND hWnd, int nIndex)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetClassLongPtr);
        return Library::Call< ULONG_PTR, HWND, int >(pFunc, hWnd, nIndex);
    }

    int User32::GetClassNameT(HWND hWnd, LPTSTR lpClassName, int nMaxCount)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetClassName);
        return Library::Call< int, HWND, LPTSTR, int >(pFunc, hWnd, lpClassName, nMaxCount);
    }

    WORD User32::GetClassWord(HWND hWnd, int nIndex)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetClassWord);
        return Library::Call< WORD, HWND, int >(pFunc, hWnd, nIndex);
    }

    LONG User32::GetWindowLongT(HWND hWnd, int nIndex)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetWindowLong);
        return Library::Call< LONG, HWND, int >(pFunc, hWnd, nIndex);
    }

    ATOM User32::RegisterClassT(CONST WNDCLASS* lpWndClass)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, RegisterClass);
        return Library::Call< ATOM, CONST WNDCLASS* >(pFunc, lpWndClass);
    }

    ATOM User32::RegisterClassExT(CONST WNDCLASSEX* lpwcx)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, RegisterClassEx);
        return Library::Call< ATOM, CONST WNDCLASSEX* >(pFunc, lpwcx);
    }

    ULONG_PTR User32::SetClassLongPtrT(HWND hWnd, int nIndex, LONG_PTR dwNewLong)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SetClassLongPtr);
        return Library::Call< ULONG_PTR, HWND, int, LONG_PTR >(pFunc, hWnd, nIndex, dwNewLong);
    }

    WORD User32::SetClassWord(HWND hWnd, int nIndex, WORD wNewWord)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetClassWord);
        return Library::Call< WORD, HWND, int, WORD >(pFunc, hWnd, nIndex, wNewWord);
    }

    LONG User32::SetWindowLongT(HWND hWnd, int nIndex, LONG dwNewLong)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SetWindowLong);
        return Library::Call< LONG, HWND, int, LONG >(pFunc, hWnd, nIndex, dwNewLong);
    }

    LONG_PTR User32::SetWindowLongPtrT(HWND hWnd, int nIndex, LONG_PTR dwNewLong)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SetWindowLongPtr);
        return Library::Call< LONG_PTR, HWND, int, LONG_PTR >(pFunc, hWnd, nIndex, dwNewLong);
    }

    LONG_PTR User32::GetWindowLongPtrT(HWND hWnd, int nIndex)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetWindowLongPtr);
        return Library::Call< LONG_PTR, HWND, int >(pFunc, hWnd, nIndex);
    }

    BOOL User32::UnregisterClassT(LPCTSTR lpClassName, HINSTANCE hInstance)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, UnregisterClass);
        return Library::Call< BOOL, LPCTSTR, HINSTANCE >(pFunc, lpClassName, hInstance);
    }

}}}//qor::win::api
