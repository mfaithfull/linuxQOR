// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Shell execution, command line and process entry points of shell32.dll. See shell.h for the scope of this library and
//purpose.md for the audit trail that established that every name below is exported by the shell32.dll on the target
//system. Note that the shell32.dll surface is DESKTOP only: the SDK declares all of shellapi.h inside
//WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP), and every wrapper below binds through GetProcAddress so a missing
//export raises at run time rather than failing to link.

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

    //ShellExecute returns an HINSTANCE value that is only an instance handle when it is greater than
    //HERR_HINSTANCE_ERRORS (31); values 1 to 31 are the documented error codes and zero means the user cancelled the
    //operation. The value is therefore passed through unchecked for the caller to classify against HERR_HINSTANCE_ERRORS,
    //and the A and W variants are separate exports rather than an AW stem.
    HINSTANCE Shell::ShellExecuteA( ::HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, ShellExecuteA );
        return Library::Call< HINSTANCE, ::HWND, LPCSTR, LPCSTR, LPCSTR, LPCSTR, INT >(pFunc, hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);
    }

    HINSTANCE Shell::ShellExecuteW( ::HWND hwnd, LPCWSTR lpOperation, LPCWSTR lpFile, LPCWSTR lpParameters, LPCWSTR lpDirectory, INT nShowCmd )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, ShellExecuteW );
        return Library::Call< HINSTANCE, ::HWND, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, INT >(pFunc, hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);
    }

    //ShellExecuteEx fills the SHELLEXECUTEINFO structure with the outcome of the operation, including the process handles
    //the caller owns on success, and reports whether the operation completed through the BOOL return
    BOOL Shell::ShellExecuteExA( ::SHELLEXECUTEINFOA* pExecInfo )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, ShellExecuteExA );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, ::SHELLEXECUTEINFOA* >(pFunc, pExecInfo);
        return bResult;
    }

    BOOL Shell::ShellExecuteExW( ::SHELLEXECUTEINFOW* pExecInfo )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, ShellExecuteExW );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, ::SHELLEXECUTEINFOW* >(pFunc, pExecInfo);
        return bResult;
    }

    //ShellAbout draws the application banner and then returns the system uptime in milliseconds through the SDK defined
    //success check, so the value is a measurement rather than a failure state and is passed through unchecked
    INT Shell::ShellAboutA( ::HWND hWnd, LPCSTR szApp, LPCSTR szOtherStuff, ::HICON hIcon )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, ShellAboutA );
        return Library::Call< INT, ::HWND, LPCSTR, LPCSTR, ::HICON >(pFunc, hWnd, szApp, szOtherStuff, hIcon);
    }

    INT Shell::ShellAboutW( ::HWND hWnd, LPCWSTR szApp, LPCWSTR szOtherStuff, ::HICON hIcon )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, ShellAboutW );
        return Library::Call< INT, ::HWND, LPCWSTR, LPCWSTR, ::HICON >(pFunc, hWnd, szApp, szOtherStuff, hIcon);
    }

    //FindExecutable resolves the association chain for a document and writes the executable path into lpResult, which must
    //be at least MAX_PATH characters. The returned HINSTANCE is one of HINSTANCE_BADFILE or HINSTANCE_NOFILE on failure,
    //both of which are non zero, so the classification is left to the caller.
    HINSTANCE Shell::FindExecutableA( LPCSTR lpFile, LPCSTR lpDirectory, LPSTR lpResult )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, FindExecutableA );
        return Library::Call< HINSTANCE, LPCSTR, LPCSTR, LPSTR >(pFunc, lpFile, lpDirectory, lpResult);
    }

    HINSTANCE Shell::FindExecutableW( LPCWSTR lpFile, LPCWSTR lpDirectory, LPWSTR lpResult )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, FindExecutableW );
        return Library::Call< HINSTANCE, LPCWSTR, LPCWSTR, LPWSTR >(pFunc, lpFile, lpDirectory, lpResult);
    }

    //CommandLineToArgvW allocates the argument array from the process heap and returns NULL on failure, so the returned
    //pointer is the contract and the caller releases the array with GlobalFree using pNumArgs entries
    LPWSTR* Shell::CommandLineToArgvW( LPCWSTR lpCmdLine, INT* pNumArgs )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, CommandLineToArgvW );
        return Library::Call< LPWSTR*, LPCWSTR, INT* >(pFunc, lpCmdLine, pNumArgs);
    }

    //DoEnvironmentSubst expands %VARIABLE% references in place and returns the number of substitutions performed, which
    //is zero when the string has none, so the count is passed through unchecked
    DWORD Shell::DoEnvironmentSubstA( LPSTR pszSrc, UINT cchSrc )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, DoEnvironmentSubstA );
        return Library::Call< DWORD, LPSTR, UINT >(pFunc, pszSrc, cchSrc);
    }

    DWORD Shell::DoEnvironmentSubstW( LPWSTR pszSrc, UINT cchSrc )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, DoEnvironmentSubstW );
        return Library::Call< DWORD, LPWSTR, UINT >(pFunc, pszSrc, cchSrc);
    }

    //SHEvaluateSystemCommandTemplate validates a command template against the shell command policy and returns the
    //application, command line and parameter strings the caller can pass to ShellExecuteEx. The out pointers are
    //allocated by the call and released by the caller with FreeHGlobalW.
    HRESULT Shell::SHEvaluateSystemCommandTemplate( PCWSTR pszCmdTemplate, PWSTR* ppszApplication, PWSTR* ppszCommandLine, PWSTR* ppszParameters )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHEvaluateSystemCommandTemplate );
        CheckReturn< HRESULT, LongSuccessCheck >::TType hrResult = Library::Call< HRESULT, PCWSTR, PWSTR*, PWSTR*, PWSTR* >(pFunc, pszCmdTemplate, ppszApplication, ppszCommandLine, ppszParameters);
        return hrResult;
    }

    //SHCreateProcessAsUserW starts a process as the token the structure names, subject to the shell command policy, and
    //fills the in out process handles. There is no A or W variant of this entry point - the SDK and the export list both
    //carry only the W spelling.
    BOOL Shell::SHCreateProcessAsUserW( ::PSHCREATEPROCESSINFOW pscpi )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHCreateProcessAsUserW );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, ::PSHCREATEPROCESSINFOW >(pFunc, pscpi);
        return bResult;
    }

    //SHTestTokenMembership answers whether the token holds the named restricted system access, so FALSE is a valid answer
    //rather than a failure and the BOOL is passed through unchecked. The token must come from the shell token, and hToken
    //may be nullptr to mean the current process token.
    BOOL Shell::SHTestTokenMembership( ::HANDLE hToken, ULONG ulRID )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHTestTokenMembership );
        return Library::Call< BOOL, ::HANDLE, ULONG >(pFunc, hToken, ulRID);
    }

}}} //qor::win::api

