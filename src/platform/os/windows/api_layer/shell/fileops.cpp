// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Shell file operation entry points of shell32.dll. See shell.h for the scope of this library and purpose.md for the audit
//trail that established that every name below is exported by the shell32.dll on the target system.

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

    //SHFileOperation returns an OS error number rather than a BOOL; zero is success and any other value is the reason the
    //batch operation failed, which is the TSuccessCheck< int > contract. The A and W variants are separate exports.
    int Shell::SHFileOperationA( ::LPSHFILEOPSTRUCTA lpFileOp )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHFileOperationA );
        CheckReturn< int, TSuccessCheck< int > >::TType iResult = Library::Call< int, ::LPSHFILEOPSTRUCTA >(pFunc, lpFileOp);
        return iResult;
    }

    int Shell::SHFileOperationW( ::LPSHFILEOPSTRUCTW lpFileOp )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHFileOperationW );
        CheckReturn< int, TSuccessCheck< int > >::TType iResult = Library::Call< int, ::LPSHFILEOPSTRUCTW >(pFunc, lpFileOp);
        return iResult;
    }

    //SHFreeNameMappings releases the name mapping handle owned by a completed SHFileOperation and has no failure state
    void Shell::SHFreeNameMappings( ::HANDLE hNameMappings )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHFreeNameMappings );
        Library::voidCall< ::HANDLE >( pFunc, hNameMappings );
    }

    //SHGetDiskFreeSpaceEx fills the three ULARGE_INTEGER out parameters and reports success through the BOOL return.
    //Passing pszDirectoryName as nullptr queries the process current working directory, which the SDK documents as the
    //behaviour for that case. The A and W variants are separate exports, both present in the shell32.dll export list.
    BOOL Shell::SHGetDiskFreeSpaceExA( LPCSTR pszDirectoryName, ::ULARGE_INTEGER* pulFreeBytesAvailableToCaller, ::ULARGE_INTEGER* pulTotalNumberOfBytes, ::ULARGE_INTEGER* pulTotalNumberOfFreeBytes )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHGetDiskFreeSpaceExA );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCSTR, ::ULARGE_INTEGER*, ::ULARGE_INTEGER*, ::ULARGE_INTEGER* >(pFunc, pszDirectoryName, pulFreeBytesAvailableToCaller, pulTotalNumberOfBytes, pulTotalNumberOfFreeBytes);
        return bResult;
    }

    BOOL Shell::SHGetDiskFreeSpaceExW( LPCWSTR pszDirectoryName, ::ULARGE_INTEGER* pulFreeBytesAvailableToCaller, ::ULARGE_INTEGER* pulTotalNumberOfBytes, ::ULARGE_INTEGER* pulTotalNumberOfFreeBytes )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHGetDiskFreeSpaceExW );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCWSTR, ::ULARGE_INTEGER*, ::ULARGE_INTEGER*, ::ULARGE_INTEGER* >(pFunc, pszDirectoryName, pulFreeBytesAvailableToCaller, pulTotalNumberOfBytes, pulTotalNumberOfFreeBytes);
        return bResult;
    }

    //SHGetFileInfo returns the DWORD_PTR address of the structure that psfi points at, or NULL on failure. A zero here is
    //a real failure state, but the value is also the alias of the caller supplied structure, so the pointer is passed
    //through for the caller to inspect alongside the fields the call filled in.
    DWORD_PTR Shell::SHGetFileInfoA( LPCSTR pszPath, DWORD dwFileAttributes, ::SHFILEINFOA* psfi, UINT cbFileInfo, UINT uFlags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHGetFileInfoA );
        return Library::Call< DWORD_PTR, LPCSTR, DWORD, ::SHFILEINFOA*, UINT, UINT >(pFunc, pszPath, dwFileAttributes, psfi, cbFileInfo, uFlags);
    }

    DWORD_PTR Shell::SHGetFileInfoW( LPCWSTR pszPath, DWORD dwFileAttributes, ::SHFILEINFOW* psfi, UINT cbFileInfo, UINT uFlags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHGetFileInfoW );
        return Library::Call< DWORD_PTR, LPCWSTR, DWORD, ::SHFILEINFOW*, UINT, UINT >(pFunc, pszPath, dwFileAttributes, psfi, cbFileInfo, uFlags);
    }

    //SHGetNewLinkInfo resolves the name of the link to create for the target and reports success through the BOOL return
    BOOL Shell::SHGetNewLinkInfoA( LPCSTR pszLinkTo, LPCSTR pszDir, LPSTR pszName, BOOL* pfMustCopy, UINT uFlags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHGetNewLinkInfoA );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCSTR, LPCSTR, LPSTR, BOOL*, UINT >(pFunc, pszLinkTo, pszDir, pszName, pfMustCopy, uFlags);
        return bResult;
    }

    BOOL Shell::SHGetNewLinkInfoW( LPCWSTR pszLinkTo, LPCWSTR pszDir, LPWSTR pszName, BOOL* pfMustCopy, UINT uFlags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHGetNewLinkInfoW );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCWSTR, LPCWSTR, LPWSTR, BOOL*, UINT >(pFunc, pszLinkTo, pszDir, pszName, pfMustCopy, uFlags);
        return bResult;
    }

    //IsLFNDrive reports the file system capability of the volume holding the path, so FALSE is a valid answer and only
    //a raised shell error is a failure; the BOOL return is passed through unchecked for that reason.
    BOOL Shell::IsLFNDriveA( LPCSTR pszPath )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, IsLFNDriveA );
        return Library::Call< BOOL, LPCSTR >(pFunc, pszPath);
    }

    BOOL Shell::IsLFNDriveW( LPCWSTR pszPath )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, IsLFNDriveW );
        return Library::Call< BOOL, LPCWSTR >(pFunc, pszPath);
    }

}}} //qor::win::api
