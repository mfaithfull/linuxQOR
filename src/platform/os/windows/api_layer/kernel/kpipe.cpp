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

    BOOL Kernel32::CreatePipe(PHANDLE hReadPipe, PHANDLE hWritePipe, LPSECURITY_ATTRIBUTES lpPipeAttributes, DWORD nSize)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CreatePipe);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, PHANDLE, PHANDLE, LPSECURITY_ATTRIBUTES, DWORD>(pFunc, hReadPipe, hWritePipe, lpPipeAttributes, nSize);
        return result;
    }

    BOOL Kernel32::CallNamedPipe(LPCTSTR lpNamedPipeName, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesRead, DWORD nTimeOut)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, CallNamedPipe);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, LPCTSTR, LPVOID, DWORD, LPVOID, DWORD, LPDWORD, DWORD >(pFunc, lpNamedPipeName, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesRead, nTimeOut);
        return result;
    }

    BOOL Kernel32::ConnectNamedPipe(HANDLE hNamedPipe, LPOVERLAPPED lpOverlapped)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, ConnectNamedPipe);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, LPOVERLAPPED>(pFunc, hNamedPipe, lpOverlapped);
        return result;
    }

    HANDLE Kernel32::CreateNamedPipe(LPCTSTR lpName, DWORD dwOpenMode, DWORD dwPipeMode, DWORD nMaxInstances, DWORD nOutBufferSize, DWORD nInBufferSize, DWORD nDefaultTimeOut, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, CreateNamedPipe);
        CheckReturn< HANDLE, HandleCheck >::TType result = Library::Call<HANDLE, LPCTSTR, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPSECURITY_ATTRIBUTES>(pFunc, lpName, dwOpenMode, dwPipeMode, nMaxInstances, nOutBufferSize, nInBufferSize, nDefaultTimeOut, lpSecurityAttributes);
        return result;
    }

    BOOL Kernel32::DisconnectNamedPipe(HANDLE hNamedPipe)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, DisconnectNamedPipe);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE>(pFunc, hNamedPipe);
        return result;
    }

    BOOL Kernel32::GetNamedPipeClientComputerName(HANDLE Pipe, LPTSTR ClientComputerName, ULONG ClientComputerNameLength)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, GetNamedPipeClientComputerName);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, LPTSTR, ULONG>(pFunc, Pipe, ClientComputerName, ClientComputerNameLength);
        return result;
    }

    BOOL Kernel32::GetNamedPipeClientProcessId(HANDLE Pipe, PULONG ClientProcessId)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetNamedPipeClientProcessId);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, PULONG>(pFunc, Pipe, ClientProcessId);
        return result;
    }

    BOOL Kernel32::GetNamedPipeClientSessionId(HANDLE Pipe, PULONG ClientSessionId)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetNamedPipeClientSessionId);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, PULONG>(pFunc, Pipe, ClientSessionId);
        return result;
    }

    BOOL Kernel32::GetNamedPipeHandleState(HANDLE hNamedPipe, LPDWORD lpState, LPDWORD lpCurInstances, LPDWORD lpMaxCollectionCount, LPDWORD lpCollectDataTimeout, LPTSTR lpUserName, DWORD nMaxUserNameSize)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, GetNamedPipeHandleState);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, LPDWORD, LPDWORD, LPDWORD, LPDWORD, LPTSTR, DWORD >(pFunc, hNamedPipe, lpState, lpCurInstances, lpMaxCollectionCount, lpCollectDataTimeout, lpUserName, nMaxUserNameSize);
        return result;
    }

    BOOL Kernel32::GetNamedPipeInfo(HANDLE hNamedPipe, LPDWORD lpFlags, LPDWORD lpOutBufferSize, LPDWORD lpInBufferSize, LPDWORD lpMaxInstances)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetNamedPipeInfo);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, LPDWORD, LPDWORD, LPDWORD, LPDWORD>(pFunc, hNamedPipe, lpFlags, lpOutBufferSize, lpInBufferSize, lpMaxInstances);
        return result;
    }

    BOOL Kernel32::GetNamedPipeServerProcessId(HANDLE Pipe, PULONG ServerProcessId)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetNamedPipeServerProcessId);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, PULONG>(pFunc, Pipe, ServerProcessId);
        return result;
    }

    BOOL Kernel32::GetNamedPipeServerSessionId(HANDLE Pipe, PULONG ServerSessionId)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetNamedPipeServerSessionId);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, PULONG>(pFunc, Pipe, ServerSessionId);
        return result;
    }

    BOOL Kernel32::ImpersonateNamedPipeClient(HANDLE hNamedPipe)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, ImpersonateNamedPipeClient);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE>(pFunc, hNamedPipe);
        return result;
    }

    BOOL Kernel32::PeekNamedPipe(HANDLE hNamedPipe, LPVOID lpBuffer, DWORD nBufferSize, LPDWORD lpBytesRead, LPDWORD lpTotalBytesAvail, LPDWORD lpBytesLeftThisMessage)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, PeekNamedPipe);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, LPVOID, DWORD, LPDWORD, LPDWORD, LPDWORD>(pFunc, hNamedPipe, lpBuffer, nBufferSize, lpBytesRead, lpTotalBytesAvail, lpBytesLeftThisMessage);
        return result;
    }

    BOOL Kernel32::SetNamedPipeHandleState(HANDLE hNamedPipe, LPDWORD lpMode, LPDWORD lpMaxCollectionCount, LPDWORD lpCollectDataTimeout)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetNamedPipeHandleState);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, LPDWORD, LPDWORD, LPDWORD>(pFunc, hNamedPipe, lpMode, lpMaxCollectionCount, lpCollectDataTimeout);
        return result;
    }

    BOOL Kernel32::TransactNamedPipe(HANDLE hNamedPipe, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesRead, LPOVERLAPPED lpOverlapped)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, TransactNamedPipe);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, LPVOID, DWORD, LPVOID, DWORD, LPDWORD, LPOVERLAPPED>(pFunc, hNamedPipe, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesRead, lpOverlapped);
        return result;
    }

    BOOL Kernel32::WaitNamedPipe(LPCTSTR lpNamedPipeName, DWORD nTimeOut)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(kernel32, WaitNamedPipe);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, LPCTSTR, DWORD>(pFunc, lpNamedPipeName, nTimeOut);
        return result;
    }
    
}}}//qor::win::api

