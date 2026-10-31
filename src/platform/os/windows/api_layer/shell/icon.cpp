// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Icon, image list and icon overlay entry points of shell32.dll. See shell.h for the scope of this library and purpose.md
//for the audit trail that established that every name below is exported by the shell32.dll on the target system.

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "../kernel/errorhandler.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "shell.h" //shell.h must be the first windows header as it's the primary inclusion point for windows.h
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //The icon extraction family returns a handle on success and NULL or zero on failure. The handle is the resource the
    //caller must pass to DeleteObject, so a NULL return is reported through the CheckNonZero contract for the handle
    //valued forms and the count forms are passed through where zero is a meaningful count.
    ::HICON Shell::DuplicateIcon( ::HINSTANCE hInst, ::HICON hIcon )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, DuplicateIcon );
        CheckReturn< ::HICON, CheckNonZero< ::HICON > >::TType hResult = Library::Call< ::HICON, ::HINSTANCE, ::HICON >(pFunc, hInst, hIcon);
        return hResult;
    }

    ::HICON Shell::ExtractAssociatedIconA( ::HINSTANCE hInst, LPSTR pszIconPath, WORD* piIcon )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, ExtractAssociatedIconA );
        CheckReturn< ::HICON, CheckNonZero< ::HICON > >::TType hResult = Library::Call< ::HICON, ::HINSTANCE, LPSTR, WORD* >(pFunc, hInst, pszIconPath, piIcon);
        return hResult;
    }

    ::HICON Shell::ExtractAssociatedIconW( ::HINSTANCE hInst, LPWSTR pszIconPath, WORD* piIcon )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, ExtractAssociatedIconW );
        CheckReturn< ::HICON, CheckNonZero< ::HICON > >::TType hResult = Library::Call< ::HICON, ::HINSTANCE, LPWSTR, WORD* >(pFunc, hInst, pszIconPath, piIcon);
        return hResult;
    }

    ::HICON Shell::ExtractAssociatedIconExA( ::HINSTANCE hInst, LPSTR pszIconPath, WORD* piIconIndex, WORD* piIconId )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, ExtractAssociatedIconExA );
        CheckReturn< ::HICON, CheckNonZero< ::HICON > >::TType hResult = Library::Call< ::HICON, ::HINSTANCE, LPSTR, WORD*, WORD* >(pFunc, hInst, pszIconPath, piIconIndex, piIconId);
        return hResult;
    }

    ::HICON Shell::ExtractAssociatedIconExW( ::HINSTANCE hInst, LPWSTR pszIconPath, WORD* piIconIndex, WORD* piIconId )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, ExtractAssociatedIconExW );
        CheckReturn< ::HICON, CheckNonZero< ::HICON > >::TType hResult = Library::Call< ::HICON, ::HINSTANCE, LPWSTR, WORD*, WORD* >(pFunc, hInst, pszIconPath, piIconIndex, piIconId);
        return hResult;
    }

    ::HICON Shell::ExtractIconA( ::HINSTANCE hInst, LPCSTR pszExeFileName, UINT nIconIndex )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, ExtractIconA );
        CheckReturn< ::HICON, CheckNonZero< ::HICON > >::TType hResult = Library::Call< ::HICON, ::HINSTANCE, LPCSTR, UINT >(pFunc, hInst, pszExeFileName, nIconIndex);
        return hResult;
    }

    ::HICON Shell::ExtractIconW( ::HINSTANCE hInst, LPCWSTR pszExeFileName, UINT nIconIndex )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, ExtractIconW );
        CheckReturn< ::HICON, CheckNonZero< ::HICON > >::TType hResult = Library::Call< ::HICON, ::HINSTANCE, LPCWSTR, UINT >(pFunc, hInst, pszExeFileName, nIconIndex);
        return hResult;
    }

    //ExtractIconEx fills the caller sized icon handle arrays and returns the number of icons found, which may legitimately
    //be zero for a file with no icon, so the count is passed through unchecked.
    UINT Shell::ExtractIconExA( LPCSTR lpszFile, INT nIconIndex, ::HICON* phiconLarge, ::HICON* phiconSmall, UINT nIcons )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, ExtractIconExA );
        return Library::Call< UINT, LPCSTR, INT, ::HICON*, ::HICON*, UINT >(pFunc, lpszFile, nIconIndex, phiconLarge, phiconSmall, nIcons);
    }

    UINT Shell::ExtractIconExW( LPCWSTR lpszFile, INT nIconIndex, ::HICON* phiconLarge, ::HICON* phiconSmall, UINT nIcons )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, ExtractIconExW );
        return Library::Call< UINT, LPCWSTR, INT, ::HICON*, ::HICON*, UINT >(pFunc, lpszFile, nIconIndex, phiconLarge, phiconSmall, nIcons);
    }

    //SHGetStockIconInfo fills the SHSTOCKICONINFO out structure, which owns the icon handles the caller must release with
    //DeleteObject, and reports the resolution outcome through the HRESULT
    HRESULT Shell::SHGetStockIconInfo( ::SHSTOCKICONID siid, UINT uFlags, ::SHSTOCKICONINFO* psii )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHGetStockIconInfo );
        CheckReturn< HRESULT, LongSuccessCheck >::TType hrResult = Library::Call< HRESULT, ::SHSTOCKICONID, UINT, ::SHSTOCKICONINFO* >(pFunc, siid, uFlags, psii);
        return hrResult;
    }

    //SHGetImageList resolves a shell image list interface through the REFIID/COM style query, so the out object pointer is
    //the contract alongside the HRESULT
    HRESULT Shell::SHGetImageList( INT iImageList, REFIID riid, void** ppvObj )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHGetImageList );
        CheckReturn< HRESULT, LongSuccessCheck >::TType hrResult = Library::Call< HRESULT, INT, REFIID, void** >(pFunc, iImageList, riid, ppvObj);
        return hrResult;
    }

    //SHLoadNonloadedIconOverlayIdentifiers takes no parameters and registers the icon overlay handlers the shell has not
    //loaded yet, reporting failure through the HRESULT
    HRESULT Shell::SHLoadNonloadedIconOverlayIdentifiers( void )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHLoadNonloadedIconOverlayIdentifiers );
        CheckReturn< HRESULT, LongSuccessCheck >::TType hrResult = Library::Call< HRESULT >(pFunc);
        return hrResult;
    }

}}} //qor::win::api

