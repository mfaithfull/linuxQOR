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

    HANDLE Kernel32::CreateJobObjectT( ::LPSECURITY_ATTRIBUTES lpJobAttributes, LPCTSTR lpName )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, CreateJobObject );
        CheckReturn< HANDLE, HandleNullCheck >::TType hResult = Library::Call< HANDLE, ::LPSECURITY_ATTRIBUTES, LPCTSTR >(pFunc, lpJobAttributes, lpName);
        return hResult;
    }

    HANDLE Kernel32::OpenJobObjectT( DWORD dwDesiredAccess, BOOL bInheritHandles, LPCTSTR lpName )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, OpenJobObject );
        CheckReturn< HANDLE, HandleNullCheck >::TType hResult = Library::Call< HANDLE, DWORD, BOOL, LPCTSTR >(pFunc, dwDesiredAccess, bInheritHandles, lpName);
        return hResult;
    }

    BOOL Kernel32::AssignProcessToJobObject( HANDLE hJob, HANDLE hProcess )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, AssignProcessToJobObject );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, HANDLE >(pFunc, hJob, hProcess);
        return bResult;
    }

    BOOL Kernel32::TerminateJobObject( HANDLE hJob, UINT uExitCode )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, TerminateJobObject );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, UINT >(pFunc, hJob, uExitCode);
        return bResult;
    }

    BOOL Kernel32::QueryInformationJobObject( HANDLE hJob, ::JOBOBJECTINFOCLASS JobObjectInfoClass, void* lpJobObjectInfo, DWORD cbJobObjectInfoLength, LPDWORD lpReturnLength )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, QueryInformationJobObject );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, ::JOBOBJECTINFOCLASS, LPVOID, DWORD, LPDWORD >(pFunc, hJob, JobObjectInfoClass, lpJobObjectInfo, cbJobObjectInfoLength, lpReturnLength);
        return bResult;
    }

    BOOL Kernel32::SetInformationJobObject( HANDLE hJob, ::JOBOBJECTINFOCLASS JobObjectInfoClass, void* lpJobObjectInfo, DWORD cbJobObjectInfoLength )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetInformationJobObject );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, ::JOBOBJECTINFOCLASS, LPVOID, DWORD >(pFunc, hJob, JobObjectInfoClass, lpJobObjectInfo, cbJobObjectInfoLength);
        return bResult;
    }

    BOOL Kernel32::IsProcessInJob( HANDLE ProcessHandle, HANDLE JobHandle, PBOOL Result )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, IsProcessInJob );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, HANDLE, PBOOL >(pFunc, ProcessHandle, JobHandle, Result);
        return bResult;
    }

    BOOL Kernel32::QueryProcessAffinityUpdateMode( HANDLE ProcessHandle, LPDWORD lpdwFlags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, QueryProcessAffinityUpdateMode );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, LPDWORD >(pFunc, ProcessHandle, lpdwFlags);
        return bResult;
    }

    BOOL Kernel32::SetProcessAffinityUpdateMode( HANDLE ProcessHandle, DWORD dwFlags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetProcessAffinityUpdateMode );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, DWORD >(pFunc, ProcessHandle, dwFlags);
        return bResult;
    }

}}}//qor::win::api