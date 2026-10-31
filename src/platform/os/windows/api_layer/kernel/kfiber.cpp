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

    BOOL Kernel32::ConvertFiberToThread(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, ConvertFiberToThread );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL >(pFunc);
        return bResult;
    }

    void* Kernel32::ConvertThreadToFiber( void* lpParameter )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, ConvertThreadToFiber );
        CheckReturn< LPVOID, CheckNonZero< LPVOID > >::TType pResult = Library::Call< LPVOID, LPVOID >(pFunc, lpParameter);
        return pResult;
    }

    LPVOID Kernel32::ConvertThreadToFiberEx( void* lpParameter, DWORD dwFlags )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, ConvertThreadToFiberEx );
        CheckReturn< LPVOID, CheckNonZero< LPVOID > >::TType pResult = Library::Call< LPVOID, LPVOID, DWORD >(pFunc, lpParameter, dwFlags);
        return pResult;
    }

    void* Kernel32::CreateFiber( SIZE_T dwStackSize, ::LPFIBER_START_ROUTINE lpStartAddress, void* lpParameter )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CreateFiber );
        CheckReturn< LPVOID, CheckNonZero< LPVOID > >::TType pResult = Library::Call< LPVOID, SIZE_T, ::LPFIBER_START_ROUTINE, LPVOID >(pFunc, dwStackSize, lpStartAddress, lpParameter);
        return pResult;
    }

    void* Kernel32::CreateFiberEx( SIZE_T dwStackCommitSize, SIZE_T dwStackReserveSize, DWORD dwFlags, ::LPFIBER_START_ROUTINE lpStartAddress, void* lpParameter )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CreateFiberEx );
        CheckReturn< LPVOID, CheckNonZero< LPVOID > >::TType pResult = Library::Call< LPVOID, SIZE_T, SIZE_T, DWORD, ::LPFIBER_START_ROUTINE, LPVOID >(pFunc, dwStackCommitSize, dwStackReserveSize, dwFlags, lpStartAddress, lpParameter);
        return pResult;
    }

    VOID Kernel32::DeleteFiber( void* lpFiber )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, DeleteFiber );
        Library::voidCall< LPVOID >( pFunc, lpFiber );
    }

    VOID Kernel32::SwitchToFiber( void* lpFiber )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SwitchToFiber );
        Library::voidCall< LPVOID >( pFunc, lpFiber );
    }

    BOOL Kernel32::IsThreadAFiber(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, IsThreadAFiber );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL >(pFunc);
        return bResult;
    }

}}}//qor::win::api