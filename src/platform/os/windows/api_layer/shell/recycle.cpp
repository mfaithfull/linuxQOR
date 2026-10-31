// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Recycle bin and printer command entry points of shell32.dll. See shell.h for the scope of this library and purpose.md
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

    //SHEmptyRecycleBin reports S_OK, a shell specific error for an empty or disabled bin, or a user cancelled code. The
    //A and W variants are separate exports; passing pszRootPath as nullptr empties the whole bin.
    HRESULT Shell::SHEmptyRecycleBinA( ::HWND hwnd, LPCSTR pszRootPath, DWORD dwFlags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHEmptyRecycleBinA );
        CheckReturn< HRESULT, LongSuccessCheck >::TType hrResult = Library::Call< HRESULT, ::HWND, LPCSTR, DWORD >(pFunc, hwnd, pszRootPath, dwFlags);
        return hrResult;
    }

    HRESULT Shell::SHEmptyRecycleBinW( ::HWND hwnd, LPCWSTR pszRootPath, DWORD dwFlags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHEmptyRecycleBinW );
        CheckReturn< HRESULT, LongSuccessCheck >::TType hrResult = Library::Call< HRESULT, ::HWND, LPCWSTR, DWORD >(pFunc, hwnd, pszRootPath, dwFlags);
        return hrResult;
    }

    //SHQueryRecycleBin fills the LPSHQUERYRBINFO out structure with the item count and total size of the bin and reports
    //the query outcome through the HRESULT
    HRESULT Shell::SHQueryRecycleBinA( LPCSTR pszRootPath, ::LPSHQUERYRBINFO* pSHQueryRBInfo )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHQueryRecycleBinA );
        CheckReturn< HRESULT, LongSuccessCheck >::TType hrResult = Library::Call< HRESULT, LPCSTR, ::LPSHQUERYRBINFO* >(pFunc, pszRootPath, pSHQueryRBInfo);
        return hrResult;
    }

    HRESULT Shell::SHQueryRecycleBinW( LPCWSTR pszRootPath, ::LPSHQUERYRBINFO* pSHQueryRBInfo )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHQueryRecycleBinW );
        CheckReturn< HRESULT, LongSuccessCheck >::TType hrResult = Library::Call< HRESULT, LPCWSTR, ::LPSHQUERYRBINFO* >(pFunc, pszRootPath, pSHQueryRBInfo);
        return hrResult;
    }

    //SHInvokePrinterCommand drives the shell printer dialog through the action codes in PRINTACTION_*; the BOOL return
    //reports whether the dialog action completed.
    BOOL Shell::SHInvokePrinterCommandA( ::HWND hwnd, UINT uAction, LPCSTR lpBuf1, LPCSTR lpBuf2, BOOL fModal )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHInvokePrinterCommandA );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, ::HWND, UINT, LPCSTR, LPCSTR, BOOL >(pFunc, hwnd, uAction, lpBuf1, lpBuf2, fModal);
        return bResult;
    }

    BOOL Shell::SHInvokePrinterCommandW( ::HWND hwnd, UINT uAction, LPCWSTR lpBuf1, LPCWSTR lpBuf2, BOOL fModal )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHInvokePrinterCommandW );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, ::HWND, UINT, LPCWSTR, LPCWSTR, BOOL >(pFunc, hwnd, uAction, lpBuf1, lpBuf2, fModal);
        return bResult;
    }

}}} //qor::win::api
