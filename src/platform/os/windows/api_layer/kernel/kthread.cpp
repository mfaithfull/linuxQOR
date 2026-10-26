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

    BOOL Kernel32::BindIoCompletionCallback( HANDLE FileHandle, ::LPOVERLAPPED_COMPLETION_ROUTINE Function, ULONG Flags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, BindIoCompletionCallback );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, ::LPOVERLAPPED_COMPLETION_ROUTINE, ULONG >(pFunc, FileHandle, Function, Flags);
        return bResult;
    }

    HANDLE Kernel32::CreateThread( ::LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, ::LPTHREAD_START_ROUTINE lpStartAddress, void* lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CreateThread );
        CheckReturn< HANDLE, HandleNullCheck >::TType hResult = Library::Call< HANDLE, ::LPSECURITY_ATTRIBUTES, SIZE_T, ::LPTHREAD_START_ROUTINE, void*, DWORD, LPDWORD >(pFunc, lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
        return hResult;
    }

    VOID Kernel32::ExitThread( DWORD dwExitCode )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, ExitThread );
        Library::voidCall< DWORD >( pFunc, dwExitCode );
    }

    HANDLE Kernel32::GetCurrentThread(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetCurrentThread );
        CheckReturn< HANDLE, HandleNullCheck >::TType h = Library::Call< HANDLE >( pFunc );
        return h;
    }

    DWORD Kernel32::GetCurrentThreadId(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetCurrentThreadId );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD >(pFunc);
        return dwResult;
    }

    DWORD Kernel32::GetThreadId( HANDLE Thread )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetThreadId );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD, HANDLE >(pFunc, Thread);
        return dwResult;
    }

    BOOL Kernel32::GetExitCodeThread( HANDLE hThread, LPDWORD lpExitCode )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetExitCodeThread );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, LPDWORD >(pFunc, hThread, lpExitCode);
        return bResult;
    }

    BOOL Kernel32::GetThreadIOPendingFlag( HANDLE hThread, PBOOL lpIOIsPending )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetThreadIOPendingFlag );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, PBOOL >(pFunc, hThread, lpIOIsPending);
        return bResult;
    }

    int Kernel32::GetThreadPriority( HANDLE hThread )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetThreadPriority );
        CheckReturn< int, CheckNonZero< int > >::TType nResult = Library::Call< int, HANDLE >(pFunc, hThread);
        return nResult;
    }

    BOOL Kernel32::GetThreadPriorityBoost( HANDLE hThread, PBOOL pDisablePriorityBoost )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetThreadPriorityBoost );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, PBOOL >(pFunc, hThread, pDisablePriorityBoost);
        return bResult;
    }

    BOOL Kernel32::GetThreadTimes( HANDLE hThread, ::LPFILETIME lpCreationTime, ::LPFILETIME lpExitTime, ::LPFILETIME lpKernelTime, ::LPFILETIME lpUserTime )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetThreadTimes );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, ::LPFILETIME, ::LPFILETIME, ::LPFILETIME, ::LPFILETIME >(pFunc, hThread, lpCreationTime, lpExitTime, lpKernelTime, lpUserTime);
        return bResult;
    }

    HANDLE Kernel32::OpenThread( DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwThreadId )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, OpenThread );
        CheckReturn< HANDLE, HandleNullCheck >::TType hResult = Library::Call< HANDLE, DWORD, BOOL, DWORD >(pFunc, dwDesiredAccess, bInheritHandle, dwThreadId);
        return hResult;
    }

    BOOL Kernel32::QueryThreadCycleTime( HANDLE ThreadHandle, PULONG64 CycleTime )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, QueryThreadCycleTime );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, PULONG64 >(pFunc, ThreadHandle, CycleTime);
        return bResult;
    }

    BOOL Kernel32::QueueUserWorkItem( LPTHREAD_START_ROUTINE Function, void* Context, ULONG Flags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, QueueUserWorkItem );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPTHREAD_START_ROUTINE, void*, ULONG >(pFunc, Function, Context, Flags);
        return bResult;
    }

    DWORD Kernel32::ResumeThread( HANDLE hThread )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, ResumeThread );
        CheckReturn< DWORD, TCheckFailureValue< DWORD, static_cast< DWORD >(-1) > >::TType dwResult = Library::Call< DWORD, HANDLE >(pFunc, hThread);
        return dwResult;
    }

    DWORD_PTR Kernel32::SetThreadAffinityMask( HANDLE hThread, DWORD_PTR dwThreadAffinityMask )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetThreadAffinityMask );
        CheckReturn< DWORD_PTR, CheckNonZero< DWORD_PTR > >::TType dwResult = Library::Call< DWORD_PTR, HANDLE, DWORD_PTR >(pFunc, hThread, dwThreadAffinityMask);
        return dwResult;
    }

    DWORD Kernel32::SetThreadIdealProcessor( HANDLE hThread, DWORD dwIdealProcessor )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetThreadIdealProcessor );
        CheckReturn< DWORD, TCheckFailureValue< DWORD, static_cast< DWORD >(-1) > >::TType dwResult = Library::Call< DWORD, HANDLE, DWORD >(pFunc, hThread, dwIdealProcessor);
        return dwResult;
    }

    BOOL Kernel32::SetThreadPriority( HANDLE hThread, int nPriority )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetThreadPriority );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, int >(pFunc, hThread, nPriority);
        return bResult;
    }

    BOOL Kernel32::SetThreadPriorityBoost( HANDLE hThread, BOOL bDisablePriorityBoost )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetThreadPriorityBoost );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, BOOL >(pFunc, hThread, bDisablePriorityBoost);
        return bResult;
    }

    BOOL Kernel32::SetThreadStackGuarantee( PULONG StackSizeInBytes )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetThreadStackGuarantee );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, PULONG >(pFunc, StackSizeInBytes);
        return bResult;
    }

    VOID Kernel32::Sleep( DWORD dwMilliseconds )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, Sleep );
        Library::voidCall< DWORD >( pFunc, dwMilliseconds );
    }

    DWORD Kernel32::SleepEx( DWORD dwMilliseconds, BOOL bAlertable )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SleepEx );
        CheckReturn< DWORD, TCheckFailureValue< DWORD, static_cast< DWORD >(-1) > >::TType dwResult = Library::Call< DWORD, DWORD, BOOL >(pFunc, dwMilliseconds, bAlertable);
        return dwResult;
    }

    DWORD Kernel32::SuspendThread( HANDLE hThread )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SuspendThread );
        CheckReturn< DWORD, TCheckFailureValue< DWORD, static_cast< DWORD >(-1) > >::TType dwResult = Library::Call< DWORD, HANDLE >(pFunc, hThread);
        return dwResult;
    }

    BOOL Kernel32::SwitchToThread(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SwitchToThread );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL >(pFunc);
        return bResult;
    }

    BOOL Kernel32::TerminateThread( HANDLE hThread, DWORD dwExitCode )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, TerminateThread );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, DWORD >(pFunc, hThread, dwExitCode);
        return bResult;
    }

    DWORD Kernel32::Wow64SuspendThread( HANDLE hThread )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, Wow64SuspendThread );
        CheckReturn< DWORD, TCheckFailureValue< DWORD, static_cast< DWORD >(-1) > >::TType dwResult = Library::Call< DWORD, HANDLE >(pFunc, hThread);
        return dwResult;
    }

}}}//qor::win::api

