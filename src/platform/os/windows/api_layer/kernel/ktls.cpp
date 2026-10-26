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

    //A zero or TLS_OUT_OF_INDEXES index is a valid index, so the return value is not checked.
    DWORD Kernel32::TlsAlloc(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, TlsAlloc );
        return Library::Call< DWORD >(pFunc);
    }

    BOOL Kernel32::TlsFree( DWORD dwTlsIndex )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, TlsFree );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, DWORD >(pFunc, dwTlsIndex);
        return bResult;
    }

    void* Kernel32::TlsGetValue( DWORD dwTlsIndex )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, TlsGetValue );
        return Library::Call< LPVOID, DWORD >(pFunc, dwTlsIndex);
    }

    BOOL Kernel32::TlsSetValue( DWORD dwTlsIndex, void* lpTlsValue )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, TlsSetValue );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, DWORD, LPVOID >(pFunc, dwTlsIndex, lpTlsValue);
        return bResult;
    }

    //A zero or TLS_OUT_OF_INDEXES index is a valid index, so the return value is not checked.
    DWORD Kernel32::FlsAlloc( ::PFLS_CALLBACK_FUNCTION lpCallback )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, FlsAlloc );
        return Library::Call< DWORD, ::PFLS_CALLBACK_FUNCTION >(pFunc, lpCallback);
    }

    BOOL Kernel32::FlsFree( DWORD dwFlsIndex )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, FlsFree );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, DWORD >(pFunc, dwFlsIndex);
        return bResult;
    }

    void* Kernel32::FlsGetValue( DWORD dwFlsIndex )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, FlsGetValue );
        return Library::Call< LPVOID, DWORD >(pFunc, dwFlsIndex);
    }

    BOOL Kernel32::FlsSetValue( DWORD dwFlsIndex, void* lpFlsData )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, FlsSetValue );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, DWORD, LPVOID >(pFunc, dwFlsIndex, lpFlsData);
        return bResult;
    }

    //A zero index is valid, so the return value is not checked.
    DWORD Kernel32::GetCurrentProcessorNumber(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetCurrentProcessorNumber );
        return Library::Call< DWORD >(pFunc);
    }

}}}//qor::win::api