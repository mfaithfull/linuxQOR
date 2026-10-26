// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "errorhandler.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "kernel32.h" //kernel32.h must be the first windows header as it's the primary inclusion point for windows.h
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    LPVOID Kernel32::GetEnvironmentStringsA(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetEnvironmentStringsA );
        CheckReturn< LPVOID, CheckNonZero< LPVOID > >::TType pResult = Library::Call< LPVOID >(pFunc);
        return pResult;
    }

    LPVOID Kernel32::GetEnvironmentStringsW(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetEnvironmentStringsW );
        CheckReturn< LPVOID, CheckNonZero< LPVOID > >::TType pResult = Library::Call< LPVOID >(pFunc);
        return pResult;
    }

    BOOL Kernel32::FreeEnvironmentStringsA( LPSTR lpszEnvironmentBlock )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, FreeEnvironmentStringsA );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPSTR >(pFunc, lpszEnvironmentBlock);
        return bResult;
    }

    BOOL Kernel32::FreeEnvironmentStringsW( LPWSTR lpszEnvironmentBlock )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, FreeEnvironmentStringsW );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPWSTR >(pFunc, lpszEnvironmentBlock);
        return bResult;
    }

    DWORD Kernel32::GetEnvironmentVariableT(LPCTSTR lpName, LPTSTR lpBuffer, DWORD nSize )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, GetEnvironmentVariable );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD, LPCTSTR, LPTSTR, DWORD >(pFunc, lpName, lpBuffer, nSize);
        return dwResult;
    }

    BOOL Kernel32::SetEnvironmentVariableT( LPCTSTR lpName, LPCTSTR lpValue )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, SetEnvironmentVariable );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, LPCTSTR >(pFunc, lpName, lpValue);
        return bResult;
    }

    LPTSTR Kernel32::GetCommandLineT(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, GetCommandLine );
        return Library::Call< LPTSTR >(pFunc);
    }

    VOID Kernel32::GetStartupInfoT( LPSTARTUPINFO pStartupInfo )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, GetStartupInfo );
        Library::voidCall< LPSTARTUPINFO >( pFunc, pStartupInfo );
    }

    UINT Kernel32::WinExec( LPCSTR lpCmdLine, UINT uCmdShow )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, WinExec );
        CheckReturn< UINT, CheckNonZero< UINT > >::TType uResult = Library::Call< UINT, LPCSTR, UINT >(pFunc, lpCmdLine, uCmdShow);
        return uResult;
    }

    //NeedCurrentDirectoryForExePath is exported as the A or W variant only, so we use the AW stem macro
    BOOL Kernel32::NeedCurrentDirectoryForExePathT( LPCTSTR ExeName )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, NeedCurrentDirectoryForExePath );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR >(pFunc, ExeName);
        return bResult;
    }

}}}//qor::win::api