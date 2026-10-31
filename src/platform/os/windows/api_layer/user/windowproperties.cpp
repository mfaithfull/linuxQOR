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

    int User32::EnumPropsT(HWND hWnd, PROPENUMPROC lpEnumFunc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, EnumProps);
        return Library::Call< int, HWND, PROPENUMPROC >(pFunc, hWnd, lpEnumFunc);
    }

    int User32::EnumPropsExT(HWND hWnd, PROPENUMPROCEX lpEnumFunc, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, EnumPropsEx);
        return Library::Call< int, HWND, PROPENUMPROCEX, LPARAM >(pFunc, hWnd, lpEnumFunc, lParam);
    }

    HANDLE User32::GetPropT(HWND hWnd, LPCTSTR lpString)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetProp);
        return Library::Call< HANDLE, HWND, LPCTSTR>(pFunc, hWnd, lpString);
    }

    HANDLE User32::RemovePropT(HWND hWnd, LPCTSTR lpString)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, RemoveProp);
        return Library::Call< HANDLE, HWND, LPCTSTR >(pFunc, hWnd, lpString);
    }

    BOOL User32::SetPropT(HWND hWnd, LPCTSTR lpString, HANDLE hData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SetProp);
        return Library::Call< BOOL, HWND, LPCTSTR, HANDLE >(pFunc, hWnd, lpString, hData);
    }

}}}//qor::win::api
