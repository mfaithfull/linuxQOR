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

    HANDLE Kernel32::GetProcessHeap()
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetProcessHeap);
        CheckReturn< HANDLE, CheckNonZeroHandle<HANDLE> >::TType result = Library::Call<HANDLE>(pFunc);
        return result;
    }

    DWORD Kernel32::GetProcessHeaps(DWORD NumberOfHeaps, PHANDLE ProcessHeaps)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetProcessHeaps);
        CheckReturn< DWORD, CheckNonZero<DWORD> >::TType result = Library::Call<DWORD, DWORD, PHANDLE>(pFunc, NumberOfHeaps, ProcessHeaps);
        return result;
    }

    LPVOID Kernel32::HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, HeapAlloc);
        CheckReturn< LPVOID, CheckNonZero<LPVOID> >::TType result = Library::Call<LPVOID, HANDLE, DWORD, SIZE_T>(pFunc, hHeap, dwFlags, dwBytes);
        return result;
    }

    SIZE_T Kernel32::HeapCompact(HANDLE hHeap, DWORD dwFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, HeapCompact);
        SIZE_T result = Library::Call<SIZE_T, HANDLE, DWORD>(pFunc, hHeap, dwFlags);
        return result;
    }

    HANDLE Kernel32::HeapCreate(DWORD flOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, HeapCreate);
        CheckReturn< HANDLE, HandleNullCheck >::TType result = Library::Call<HANDLE, DWORD, SIZE_T, SIZE_T>(pFunc, flOptions, dwInitialSize, dwMaximumSize);
        return result;
    }

    BOOL Kernel32::HeapDestroy(HANDLE hHeap)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, HeapDestroy);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE>(pFunc, hHeap);
        return result;
    }

    BOOL Kernel32::HeapFree(HANDLE hHeap, DWORD dwFlags, _Frees_ptr_opt_ LPVOID lpMem)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, HeapFree);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, DWORD, LPVOID>(pFunc, hHeap, dwFlags, lpMem);
        return result;
    }

    BOOL Kernel32::HeapLock(HANDLE hHeap)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, HeapLock);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE>(pFunc, hHeap);
        return result;
    }

    BOOL Kernel32::HeapQueryInformation(HANDLE HeapHandle, HEAP_INFORMATION_CLASS HeapInformationClass, PVOID HeapInformation, SIZE_T HeapInformationLength, PSIZE_T ReturnLength)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, HeapQueryInformation);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, HEAP_INFORMATION_CLASS, PVOID, SIZE_T, PSIZE_T>(pFunc, HeapHandle, HeapInformationClass, HeapInformation, HeapInformationLength, ReturnLength);
        return result;
    }

    LPVOID Kernel32::HeapReAlloc(HANDLE hHeap, DWORD dwFlags, _Frees_ptr_opt_ LPVOID lpMem, SIZE_T dwBytes)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, HeapReAlloc);
        CheckReturn< LPVOID, CheckNonZero<LPVOID> >::TType result = Library::Call<LPVOID, HANDLE, DWORD, LPVOID, SIZE_T>(pFunc, hHeap, dwFlags, lpMem, dwBytes);
        return result;
    }

    BOOL Kernel32::HeapSetInformation(HANDLE HeapHandle, HEAP_INFORMATION_CLASS HeapInformationClass, PVOID HeapInformation, SIZE_T HeapInformationLength)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, HeapSetInformation);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, HEAP_INFORMATION_CLASS, PVOID, SIZE_T>(pFunc, HeapHandle, HeapInformationClass, HeapInformation, HeapInformationLength);
        return result;
    }

    SIZE_T Kernel32::HeapSize(HANDLE hHeap, DWORD dwFlags, LPCVOID lpMem)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, HeapSize);
        CheckReturn< SIZE_T, TCheckFailureValue<SIZE_T, (size_t)(-1)> >::TType result = Library::Call<SIZE_T, HANDLE, DWORD, LPCVOID>(pFunc, hHeap, dwFlags, lpMem);
        return result;
    }

    BOOL Kernel32::HeapUnlock(HANDLE hHeap)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, HeapUnlock);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE>(pFunc, hHeap);
        return result;
    }

    BOOL Kernel32::HeapValidate(HANDLE hHeap, DWORD dwFlags, LPCVOID lpMem)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, HeapValidate);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, DWORD, LPCVOID>(pFunc, hHeap, dwFlags, lpMem);
        return result;
    }

    BOOL Kernel32::HeapWalk(HANDLE hHeap, LPPROCESS_HEAP_ENTRY lpEntry)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, HeapWalk);
        CheckReturn< BOOL, BoolCheck >::TType result = Library::Call<BOOL, HANDLE, LPPROCESS_HEAP_ENTRY>(pFunc, hHeap, lpEntry);
        return result;
    }
    
}}}//qor::win::api