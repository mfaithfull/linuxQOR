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

    BOOL Kernel32::CreateProcessT( LPCTSTR lpApplicationName, LPTSTR lpCommandLine, ::LPSECURITY_ATTRIBUTES lpProcessAttributes, ::LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, void* lpEnvironment, LPCTSTR lpCurrentDirectory, ::LPSTARTUPINFO lpStartupInfo, ::LPPROCESS_INFORMATION lpProcessInformation )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, CreateProcess );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPCTSTR, LPTSTR, ::LPSECURITY_ATTRIBUTES, ::LPSECURITY_ATTRIBUTES, BOOL, DWORD, void*, LPCTSTR, ::LPSTARTUPINFO, ::LPPROCESS_INFORMATION >(pFunc, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
        return bResult;
    }

    HANDLE Kernel32::CreateRemoteThread( HANDLE hProcess, ::LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, void* lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CreateRemoteThread );
        CheckReturn< HANDLE, HandleNullCheck >::TType hResult = Library::Call< HANDLE, HANDLE, ::LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, void*, DWORD, LPDWORD >(pFunc, hProcess, lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
        return hResult;
    }

    VOID Kernel32::DeleteProcThreadAttributeList( ::LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, DeleteProcThreadAttributeList );
        Library::voidCall< ::LPPROC_THREAD_ATTRIBUTE_LIST >( pFunc, lpAttributeList );
    }

    VOID Kernel32::ExitProcess( UINT uExitCode )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, ExitProcess );
        Library::voidCall< UINT >( pFunc, uExitCode );
    }

    VOID Kernel32::FlushProcessWriteBuffers(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, FlushProcessWriteBuffers );
        Library::voidCall( pFunc );
    }

    DWORD Kernel32::GetCurrentProcessId(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetCurrentProcessId );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD >(pFunc);
        return dwResult;
    }

    BOOL Kernel32::GetExitCodeProcess( HANDLE hProcess, LPDWORD lpExitCode )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetExitCodeProcess );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, LPDWORD >(pFunc, hProcess, lpExitCode);
        return bResult;
    }

    DWORD Kernel32::GetPriorityClass( HANDLE hProcess )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetPriorityClass );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD, HANDLE >(pFunc, hProcess);
        return dwResult;
    }

    BOOL Kernel32::GetProcessAffinityMask( HANDLE hProcess, PDWORD_PTR lpProcessAffinityMask, PDWORD_PTR lpSystemAffinityMask )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetProcessAffinityMask );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, PDWORD_PTR, PDWORD_PTR >(pFunc, hProcess, lpProcessAffinityMask, lpSystemAffinityMask);
        return bResult;
    }

    BOOL Kernel32::GetProcessHandleCount( HANDLE hProcess, PDWORD pdwHandleCount )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetProcessHandleCount );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, PDWORD >(pFunc, hProcess, pdwHandleCount);
        return bResult;
    }

    DWORD Kernel32::GetProcessId( HANDLE Process )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetProcessId );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD, HANDLE >(pFunc, Process);
        return dwResult;
    }

    DWORD Kernel32::GetProcessIdOfThread( HANDLE Thread )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetProcessIdOfThread );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD, HANDLE >(pFunc, Thread);
        return dwResult;
    }

    BOOL Kernel32::GetProcessIoCounters( HANDLE hProcess, ::PIO_COUNTERS lpIoCounters )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetProcessIoCounters );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, ::PIO_COUNTERS >(pFunc, hProcess, lpIoCounters);
        return bResult;
    }

    BOOL Kernel32::GetProcessPriorityBoost( HANDLE hProcess, PBOOL pDisablePriorityBoost )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetProcessPriorityBoost );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, PBOOL >(pFunc, hProcess, pDisablePriorityBoost);
        return bResult;
    }

    BOOL Kernel32::GetProcessShutdownParameters( LPDWORD lpdwLevel, LPDWORD lpdwFlags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetProcessShutdownParameters );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, LPDWORD, LPDWORD >(pFunc, lpdwLevel, lpdwFlags);
        return bResult;
    }

    BOOL Kernel32::GetProcessTimes( HANDLE hProcess, ::LPFILETIME lpCreationTime, ::LPFILETIME lpExitTime, ::LPFILETIME lpKernelTime, ::LPFILETIME lpUserTime )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetProcessTimes );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, ::LPFILETIME, ::LPFILETIME, ::LPFILETIME, ::LPFILETIME >(pFunc, hProcess, lpCreationTime, lpExitTime, lpKernelTime, lpUserTime);
        return bResult;
    }

    DWORD Kernel32::GetProcessVersion( DWORD ProcessId )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetProcessVersion );
        CheckReturn< DWORD, CheckNonZero< DWORD > >::TType dwResult = Library::Call< DWORD, DWORD >(pFunc, ProcessId);
        return dwResult;
    }

    BOOL Kernel32::GetProcessWorkingSetSize( HANDLE hProcess, PSIZE_T lpMinimumWorkingSetSize, PSIZE_T lpMaximumWorkingSetSize )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetProcessWorkingSetSize );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, PSIZE_T, PSIZE_T >(pFunc, hProcess, lpMinimumWorkingSetSize, lpMaximumWorkingSetSize);
        return bResult;
    }

    BOOL Kernel32::GetProcessWorkingSetSizeEx( HANDLE hProcess, PSIZE_T lpMinimumWorkingSetSize, PSIZE_T lpMaximumWorkingSetSize, PDWORD Flags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetProcessWorkingSetSizeEx );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, PSIZE_T, PSIZE_T, PDWORD >(pFunc, hProcess, lpMinimumWorkingSetSize, lpMaximumWorkingSetSize, Flags);
        return bResult;
    }

    BOOL Kernel32::InitializeProcThreadAttributeList( ::LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList, DWORD dwAttributeCount, DWORD dwFlags, PSIZE_T lpSize )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, InitializeProcThreadAttributeList );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, ::LPPROC_THREAD_ATTRIBUTE_LIST, DWORD, DWORD, PSIZE_T >(pFunc, lpAttributeList, dwAttributeCount, dwFlags, lpSize);
        return bResult;
    }

    BOOL Kernel32::IsWow64Process( HANDLE hProcess, PBOOL Wow64Process )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, IsWow64Process );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, PBOOL >(pFunc, hProcess, Wow64Process);
        return bResult;
    }

    //LoadModule returns 0 on success, so it is checked for success rather than for a non zero value
    DWORD Kernel32::LoadModule( LPCSTR lpModuleName, void* lpParameterBlock )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, LoadModule );
        CheckReturn< DWORD, TSuccessCheck< DWORD > >::TType dwResult = Library::Call< DWORD, LPCSTR, void* >(pFunc, lpModuleName, lpParameterBlock);
        return dwResult;
    }

    HANDLE Kernel32::OpenProcess( DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, OpenProcess );
        CheckReturn< HANDLE, HandleNullCheck >::TType hResult = Library::Call< HANDLE, DWORD, BOOL, DWORD >(pFunc, dwDesiredAccess, bInheritHandle, dwProcessId);
        return hResult;
    }

    BOOL Kernel32::QueryFullProcessImageNameT( HANDLE hProcess, DWORD dwFlags, LPTSTR lpExeName, PDWORD lpdwSize )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, QueryFullProcessImageName );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, DWORD, LPTSTR, PDWORD >(pFunc, hProcess, dwFlags, lpExeName, lpdwSize);
        return bResult;
    }

    BOOL Kernel32::QueryIdleProcessorCycleTime( PULONG BufferLength, PULONG64 ProcessorIdleCycleTime )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, QueryIdleProcessorCycleTime );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, PULONG, PULONG64 >(pFunc, BufferLength, ProcessorIdleCycleTime);
        return bResult;
    }

    BOOL Kernel32::QueryProcessCycleTime( HANDLE ProcessHandle, PULONG64 CycleTime )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, QueryProcessCycleTime );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, PULONG64 >(pFunc, ProcessHandle, CycleTime);
        return bResult;
    }

    BOOL Kernel32::SetPriorityClass( HANDLE hProcess, DWORD dwPriorityClass )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetPriorityClass );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, DWORD >(pFunc, hProcess, dwPriorityClass);
        return bResult;
    }

    BOOL Kernel32::SetProcessAffinityMask( HANDLE hProcess, DWORD_PTR dwProcessAffinityMask )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetProcessAffinityMask );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, DWORD_PTR >(pFunc, hProcess, dwProcessAffinityMask);
        return bResult;
    }

    BOOL Kernel32::SetProcessPriorityBoost( HANDLE hProcess, BOOL DisablePriorityBoost )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetProcessPriorityBoost );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, BOOL >(pFunc, hProcess, DisablePriorityBoost);
        return bResult;
    }

    BOOL Kernel32::SetProcessShutdownParameters( DWORD dwLevel, DWORD dwFlags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetProcessShutdownParameters );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, DWORD, DWORD >(pFunc, dwLevel, dwFlags);
        return bResult;
    }

    BOOL Kernel32::SetProcessWorkingSetSize( HANDLE hProcess, SIZE_T dwMinimumWorkingSetSize, SIZE_T dwMaximumWorkingSetSize )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetProcessWorkingSetSize );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, SIZE_T, SIZE_T >(pFunc, hProcess, dwMinimumWorkingSetSize, dwMaximumWorkingSetSize);
        return bResult;
    }

    BOOL Kernel32::SetProcessWorkingSetSizeEx( HANDLE hProcess, SIZE_T dwMinimumWorkingSetSize, SIZE_T dwMaximumWorkingSetSize, DWORD Flags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetProcessWorkingSetSizeEx );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, SIZE_T, SIZE_T, DWORD >(pFunc, hProcess, dwMinimumWorkingSetSize, dwMaximumWorkingSetSize, Flags);
        return bResult;
    }

    BOOL Kernel32::TerminateProcess( HANDLE hProcess, UINT uExitCode )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, TerminateProcess );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, UINT >(pFunc, hProcess, uExitCode);
        return bResult;
    }

    BOOL Kernel32::UpdateProcThreadAttribute( ::LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList, DWORD dwFlags, DWORD_PTR Attribute, void* lpValue, SIZE_T cbSize, void* lpPreviousValue, PSIZE_T lpReturnSize )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, UpdateProcThreadAttribute );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, ::LPPROC_THREAD_ATTRIBUTE_LIST, DWORD, DWORD_PTR, void*, SIZE_T, void*, PSIZE_T >(pFunc, lpAttributeList, dwFlags, Attribute, lpValue, cbSize, lpPreviousValue, lpReturnSize);
        return bResult;
    }

    //Note: InitializeThreadpoolEnvironment, DestroyThreadpoolEnvironment and the SetThreadpoolCallback* family are
    //header-inline in the Windows SDK WinBase.h and exported by no DLL, so they get no export wrapper. The six that are
    //wrapped use the compile-time inline variant and live in kthreadpool.cpp - see DECISIONS.md D11 and the note at the head of that file.

}}}//qor::win::api


