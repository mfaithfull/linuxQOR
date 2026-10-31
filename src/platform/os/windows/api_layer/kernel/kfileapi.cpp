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

    BOOL Kernel32::AreFileApisANSI(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, AreFileApisANSI );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL >(pFunc);
        return bResult;
    }

    BOOL Kernel32::CheckNameLegalDOS8Dot3A( LPCTSTR lpName, LPSTR lpOemName, DWORD OemNameSize, PBOOL pbNameContainsSpaces, PBOOL pbNameLegal )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CheckNameLegalDOS8Dot3A );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, LPSTR, DWORD, PBOOL, PBOOL >(pFunc, lpName, lpOemName, OemNameSize, pbNameContainsSpaces, pbNameLegal);
        return bResult;
    }

    BOOL Kernel32::CheckNameLegalDOS8Dot3W( LPCTSTR lpName, LPSTR lpOemName, DWORD OemNameSize, PBOOL pbNameContainsSpaces, PBOOL pbNameLegal )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CheckNameLegalDOS8Dot3W );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, LPSTR, DWORD, PBOOL, PBOOL >(pFunc, lpName, lpOemName, OemNameSize, pbNameContainsSpaces, pbNameLegal);
        return bResult;
    }

    BOOL Kernel32::CopyFile( LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, CopyFile );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, LPCTSTR, BOOL >(pFunc, lpExistingFileName, lpNewFileName, bFailIfExists);
        return bResult;
    }

    BOOL Kernel32::CopyFileEx( LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, ::LPPROGRESS_ROUTINE lpProgressRoutine, void* lpData, LPBOOL pbCancel, DWORD dwCopyFlags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, CopyFileEx );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, LPCTSTR, ::LPPROGRESS_ROUTINE, LPVOID, LPBOOL, DWORD >(pFunc, lpExistingFileName, lpNewFileName, lpProgressRoutine, lpData, pbCancel, dwCopyFlags);
        return bResult;
    }

    BOOL Kernel32::CopyFileTransacted( LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, ::LPPROGRESS_ROUTINE lpProgressRoutine, LPVOID lpData, LPBOOL pbCancel, DWORD dwCopyFlags, HANDLE hTransaction )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, CopyFileTransacted );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, LPCTSTR, ::LPPROGRESS_ROUTINE, LPVOID, LPBOOL, DWORD, HANDLE >(pFunc, lpExistingFileName, lpNewFileName, lpProgressRoutine, lpData, pbCancel, dwCopyFlags, hTransaction);
        return bResult;
    }

    BOOL Kernel32::CreateHardLink( LPCTSTR lpFileName, LPCTSTR lpExistingFileName, ::LPSECURITY_ATTRIBUTES lpSecurityAttributes )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, CreateHardLink );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, LPCTSTR, ::LPSECURITY_ATTRIBUTES >(pFunc, lpFileName, lpExistingFileName, lpSecurityAttributes);
        return bResult;
    }

    BOOL Kernel32::CreateHardLinkTransacted( LPCTSTR lpFileName, LPCTSTR lpExistingFileName, ::LPSECURITY_ATTRIBUTES lpSecurityAttributes, HANDLE hTransaction )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, CreateHardLinkTransacted );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, LPCTSTR, ::LPSECURITY_ATTRIBUTES, HANDLE >(pFunc, lpFileName, lpExistingFileName, lpSecurityAttributes, hTransaction);
        return bResult;
    }

    BOOL Kernel32::CreateSymbolicLink( LPCTSTR lpSymlinkFileName, LPCTSTR lpTargetFileName, DWORD dwFlags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, CreateSymbolicLink );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, LPCTSTR, DWORD >(pFunc, lpSymlinkFileName, lpTargetFileName, dwFlags);
        return bResult;
    }

    BOOL Kernel32::CreateSymbolicLinkTransacted( LPCTSTR lpSymlinkFileName, LPCTSTR lpTargetFileName, DWORD dwFlags, HANDLE hTransaction )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, CreateSymbolicLinkTransacted );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, LPCTSTR, DWORD, HANDLE >(pFunc, lpSymlinkFileName, lpTargetFileName, dwFlags, hTransaction);
        return bResult;
    }

    BOOL Kernel32::DeleteFile( LPCTSTR lpFileName )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, DeleteFile );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR >(pFunc, lpFileName);
        return bResult;
    }

    BOOL Kernel32::DeleteFileTransacted( LPCTSTR lpFileName, HANDLE hTransaction )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, DeleteFileTransacted );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, HANDLE >(pFunc, lpFileName, hTransaction);
        return bResult;
    }

    BOOL Kernel32::GetBinaryType( LPCTSTR lpApplicationName, LPDWORD lpBinaryType )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetBinaryType );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, LPDWORD >(pFunc, lpApplicationName, lpBinaryType);
        return bResult;
    }

    DWORD Kernel32::GetCompressedFileSize( LPCTSTR lpFileName, LPDWORD lpFileSizeHigh )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, GetCompressedFileSize );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD, LPCTSTR, LPDWORD >(pFunc, lpFileName, lpFileSizeHigh);
        return dwResult;
    }

    DWORD Kernel32::GetCompressedFileSizeTransacted( LPCTSTR lpFileName, LPDWORD lpFileSizeHigh, HANDLE hTransaction )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, GetCompressedFileSizeTransacted );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD, LPCTSTR, LPDWORD, HANDLE >(pFunc, lpFileName, lpFileSizeHigh, hTransaction);
        return dwResult;
    }

    DWORD Kernel32::GetFileAttributes( LPCTSTR lpFileName )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, GetFileAttributes );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD, LPCTSTR >(pFunc, lpFileName);
        return dwResult;
    }

    BOOL Kernel32::GetFileAttributesEx( LPCTSTR lpFileName, ::GET_FILEEX_INFO_LEVELS fInfoLevelId, void* lpFileInformation )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, GetFileAttributesEx );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, ::GET_FILEEX_INFO_LEVELS, LPVOID >(pFunc, lpFileName, fInfoLevelId, lpFileInformation);
        return bResult;
    }

    BOOL Kernel32::GetFileAttributesTransacted( LPCTSTR lpFileName, ::GET_FILEEX_INFO_LEVELS fInfoLevelId, LPVOID lpFileInformation, HANDLE hTransaction )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, GetFileAttributesTransacted );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, ::GET_FILEEX_INFO_LEVELS, LPVOID, HANDLE >(pFunc, lpFileName, fInfoLevelId, lpFileInformation, hTransaction);
        return bResult;
    }

    DWORD Kernel32::GetFullPathName( LPCTSTR lpFileName, DWORD nBufferLength, LPTSTR lpBuffer, LPTSTR* lpFilePart )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, GetFullPathName );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD, LPCTSTR, DWORD, LPTSTR, LPTSTR* >(pFunc, lpFileName, nBufferLength, lpBuffer, lpFilePart);
        return dwResult;
    }

    DWORD Kernel32::GetFullPathNameTransacted( LPCTSTR lpFileName, DWORD nBufferLength, LPTSTR lpBuffer, LPTSTR *lpFilePart, HANDLE hTransaction )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, GetFullPathNameTransacted );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD, LPCTSTR, DWORD, LPTSTR, LPTSTR*, HANDLE >(pFunc, lpFileName, nBufferLength, lpBuffer, lpFilePart, hTransaction);
        return dwResult;
    }

    DWORD Kernel32::GetLongPathName( LPCTSTR lpszShortPath, LPTSTR lpszLongPath, DWORD cchBuffer )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, GetLongPathName );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD, LPCTSTR, LPTSTR, DWORD >(pFunc, lpszShortPath, lpszLongPath, cchBuffer);
        return dwResult;
    }

    DWORD Kernel32::GetLongPathNameTransacted( LPCTSTR lpszShortPath, LPTSTR lpszLongPath, DWORD cchBuffer, HANDLE hTransaction )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, GetLongPathNameTransacted );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD, LPCTSTR, LPTSTR, DWORD, HANDLE >(pFunc, lpszShortPath, lpszLongPath, cchBuffer, hTransaction);
        return dwResult;
    }

    DWORD Kernel32::GetShortPathName( LPCTSTR lpszLongPath, LPTSTR lpszShortPath, DWORD cchBuffer )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, GetShortPathName );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD, LPCTSTR, LPTSTR, DWORD >(pFunc, lpszLongPath, lpszShortPath, cchBuffer);
        return dwResult;
    }

    UINT Kernel32::GetTempFileName( LPCTSTR lpPathName, LPCTSTR lpPrefixString, UINT uUnique, LPTSTR lpTempFileName )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, GetTempFileName );
        CheckReturn< UINT, CheckNonZero< UINT > >::TType uResult = Library::Call< UINT, LPCTSTR, LPCTSTR, UINT, LPTSTR >(pFunc, lpPathName, lpPrefixString, uUnique, lpTempFileName);
        return uResult;
    }

    DWORD Kernel32::GetTempPath( DWORD nBufferLength, LPTSTR lpBuffer )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, GetTempPath );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD, DWORD, LPTSTR >(pFunc, nBufferLength, lpBuffer);
        return dwResult;
    }

    BOOL Kernel32::MoveFile( LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, MoveFile );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, LPCTSTR >(pFunc, lpExistingFileName, lpNewFileName);
        return bResult;
    }

    BOOL Kernel32::MoveFileEx( LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, DWORD dwFlags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, MoveFileEx );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, LPCTSTR, DWORD >(pFunc, lpExistingFileName, lpNewFileName, dwFlags);
        return bResult;
    }

    BOOL Kernel32::MoveFileTransacted( LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, ::LPPROGRESS_ROUTINE lpProgressRoutine, LPVOID lpData, DWORD dwFlags, HANDLE hTransaction )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, MoveFileTransacted );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, LPCTSTR, ::LPPROGRESS_ROUTINE, LPVOID, DWORD, HANDLE >(pFunc, lpExistingFileName, lpNewFileName, lpProgressRoutine, lpData, dwFlags, hTransaction);
        return bResult;
    }

    BOOL Kernel32::MoveFileWithProgress( LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, ::LPPROGRESS_ROUTINE lpProgressRoutine, void* lpData, DWORD dwFlags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, MoveFileWithProgress );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, LPCTSTR, ::LPPROGRESS_ROUTINE, LPVOID, DWORD >(pFunc, lpExistingFileName, lpNewFileName, lpProgressRoutine, lpData, dwFlags);
        return bResult;
    }

    BOOL Kernel32::ReplaceFile( LPCTSTR lpReplacedFileName, LPCTSTR lpReplacementFileName, LPCTSTR lpBackupFileName, DWORD dwReplaceFlags, void* lpExclude, LPVOID lpReserved )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, ReplaceFile );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, LPCTSTR, LPCTSTR, DWORD, LPCVOID, LPVOID >(pFunc, lpReplacedFileName, lpReplacementFileName, lpBackupFileName, dwReplaceFlags, lpExclude, lpReserved);
        return bResult;
    }

    DWORD Kernel32::SearchPath( LPCTSTR lpPath, LPCTSTR lpFileName, LPCTSTR lpExtension, DWORD nBufferLength, LPTSTR lpBuffer, LPTSTR* lpFilePart )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, SearchPath );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD, LPCTSTR, LPCTSTR, LPCTSTR, DWORD, LPTSTR, LPTSTR* >(pFunc, lpPath, lpFileName, lpExtension, nBufferLength, lpBuffer, lpFilePart);
        return dwResult;
    }

    VOID Kernel32::SetFileApisToANSI(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetFileApisToANSI );
        Library::voidCall( pFunc );
    }

    VOID Kernel32::SetFileApisToOEM(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetFileApisToOEM );
        Library::voidCall( pFunc );
    }

    BOOL Kernel32::SetFileAttributes( LPCTSTR lpFileName, DWORD dwFileAttributes )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, SetFileAttributes );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, DWORD >(pFunc, lpFileName, dwFileAttributes);
        return bResult;
    }

    BOOL Kernel32::SetFileAttributesTransacted( LPCTSTR lpFileName, DWORD dwFileAttributes, HANDLE hTransaction )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, SetFileAttributesTransacted );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, DWORD, HANDLE >(pFunc, lpFileName, dwFileAttributes, hTransaction);
        return bResult;
    }

    BOOL Kernel32::Wow64DisableWow64FsRedirection( void** OldValue )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, Wow64DisableWow64FsRedirection );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, VOID** >(pFunc, OldValue);
        return bResult;
    }

    BOOLEAN Kernel32::Wow64EnableWow64FsRedirection( BOOLEAN Wow64FsEnableRedirection )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, Wow64EnableWow64FsRedirection );
        //the kernel32 declaration uses BOOLEAN (u_char) rather than BOOL, so we cannot use BoolCheck here
        CheckReturn< BOOLEAN, CheckNonZero< BOOLEAN > >::TType bResult = Library::Call< BOOLEAN, BOOLEAN >(pFunc, Wow64FsEnableRedirection);
        return bResult;
    }

    BOOL Kernel32::Wow64RevertWow64FsRedirection( void* OldValue )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, Wow64RevertWow64FsRedirection );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPVOID >(pFunc, OldValue);
        return bResult;
    }

    UINT Kernel32::SetHandleCount( UINT uNumber )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetHandleCount );
        CheckReturn< UINT, CheckNonZero< UINT > >::TType uResult = Library::Call< UINT, UINT >(pFunc, uNumber);
        return uResult;
    }

}}}//qor::win::api
