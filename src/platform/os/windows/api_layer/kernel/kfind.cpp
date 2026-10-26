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

    BOOL Kernel32::FindClose( HANDLE hFindFile )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, FindClose );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE >(pFunc, hFindFile);
        return bResult;
    }

    HANDLE Kernel32::FindFirstFile( LPCTSTR lpFileName, ::LPWIN32_FIND_DATA lpFindFileData )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, FindFirstFile );
        CheckReturn< HANDLE, HandleNullCheck >::TType hResult = Library::Call< HANDLE, LPCTSTR, ::LPWIN32_FIND_DATA >(pFunc, lpFileName, lpFindFileData);
        return hResult;
    }

    HANDLE Kernel32::FindFirstFileEx( LPCTSTR lpFileName, ::FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, ::FINDEX_SEARCH_OPS fSearchOp, void* lpSearchFilter, DWORD dwAdditionalFlags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, FindFirstFileEx );
        CheckReturn< HANDLE, HandleNullCheck >::TType hResult = Library::Call< HANDLE, LPCTSTR, ::FINDEX_INFO_LEVELS, LPVOID, ::FINDEX_SEARCH_OPS, LPVOID, DWORD >(pFunc, lpFileName, fInfoLevelId, lpFindFileData, fSearchOp, lpSearchFilter, dwAdditionalFlags);
        return hResult;
    }

    HANDLE Kernel32::FindFirstFileNameTransactedW( LPCWSTR lpFileName, DWORD dwFlags, LPDWORD StringLength, PWCHAR LinkName, HANDLE hTransaction )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, FindFirstFileNameTransactedW );
        CheckReturn< HANDLE, HandleNullCheck >::TType hResult = Library::Call< HANDLE, LPCWSTR, DWORD, LPDWORD, PWCHAR, HANDLE >(pFunc, lpFileName, dwFlags, StringLength, LinkName, hTransaction);
        return hResult;
    }

    HANDLE Kernel32::FindFirstFileNameW( LPCWSTR lpFileName, DWORD dwFlags, LPDWORD StringLength, PWCHAR LinkName )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, FindFirstFileNameW );
        CheckReturn< HANDLE, HandleNullCheck >::TType hResult = Library::Call< HANDLE, LPCWSTR, DWORD, LPDWORD, PWCHAR >(pFunc, lpFileName, dwFlags, StringLength, LinkName);
        return hResult;
    }

    HANDLE Kernel32::FindFirstFileTransacted( LPCTSTR lpFileName, ::FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, ::FINDEX_SEARCH_OPS fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags, HANDLE hTransaction )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, FindFirstFileTransacted );
        CheckReturn< HANDLE, HandleNullCheck >::TType hResult = Library::Call< HANDLE, LPCTSTR, ::FINDEX_INFO_LEVELS, LPVOID, ::FINDEX_SEARCH_OPS, LPVOID, DWORD, HANDLE >(pFunc, lpFileName, fInfoLevelId, lpFindFileData, fSearchOp, lpSearchFilter, dwAdditionalFlags, hTransaction);
        return hResult;
    }

    HANDLE Kernel32::FindFirstStreamTransactedW( LPCWSTR lpFileName, ::STREAM_INFO_LEVELS InfoLevel, LPVOID lpFindStreamData, DWORD dwFlags, HANDLE hTransaction )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, FindFirstStreamTransactedW );
        CheckReturn< HANDLE, HandleNullCheck >::TType hResult = Library::Call< HANDLE, LPCWSTR, ::STREAM_INFO_LEVELS, LPVOID, DWORD, HANDLE >(pFunc, lpFileName, InfoLevel, lpFindStreamData, dwFlags, hTransaction);
        return hResult;
    }

    HANDLE Kernel32::FindFirstStreamW( LPCWSTR lpFileName, ::STREAM_INFO_LEVELS InfoLevel, LPVOID lpFindStreamData, DWORD dwFlags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, FindFirstStreamW );
        CheckReturn< HANDLE, HandleNullCheck >::TType hResult = Library::Call< HANDLE, LPCWSTR, ::STREAM_INFO_LEVELS, LPVOID, DWORD >(pFunc, lpFileName, InfoLevel, lpFindStreamData, dwFlags);
        return hResult;
    }

    BOOL Kernel32::FindNextFile( HANDLE hFindFile, ::LPWIN32_FIND_DATA lpFindFileData )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, FindNextFile );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, ::LPWIN32_FIND_DATA >(pFunc, hFindFile, lpFindFileData);
        return bResult;
    }

    BOOL Kernel32::FindNextFileNameW( HANDLE hFindStream, LPDWORD StringLength, PWCHAR LinkName )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, FindNextFileNameW );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, LPDWORD, PWCHAR >(pFunc, hFindStream, StringLength, LinkName);
        return bResult;
    }

    BOOL Kernel32::FindNextStreamW( HANDLE hFindStream, void* lpFindStreamData )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, FindNextStreamW );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, LPVOID >(pFunc, hFindStream, lpFindStreamData);
        return bResult;
    }

}}}//qor::win::api