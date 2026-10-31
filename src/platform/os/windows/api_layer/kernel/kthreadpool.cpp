// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0
//
// Note: InitializeThreadpoolEnvironment, DestroyThreadpoolEnvironment and the SetThreadpoolCallback* family are
// defined FORCEINLINE by the Windows SDK in WinBase.h - Windows Kits 10 Include 10.0.26100.0 um, lines 7913 to
// 7983 - over the Tp* helpers that winnt.h defines FORCEINLINE from line 25607, and those helpers only assign
// the TP_CALLBACK_ENVIRON fields. No kernel32.dll or ntdll.dll export exists for any of them, so the run-time
// qor_pp_useswinapi and GetProcAddress path used by every other wrapper in this file cannot reach them - it
// resolves null and Library::Call throws at run time rather than failing to link. The six below bind at compile
// time through the variant I helper Library::voidInlineCall in api_layer/library.h, marked with qor_pp_usesinlineapi,
// and emit no import. The design and its evidence are in DECISIONS.md D11. SetThreadpoolCallbackPriority, which the
// SDK gates on _WIN32_WINNT >= _WIN32_WINNT_WIN7, and SetThreadpoolCallbackPersistent, which the SDK defines only
// inside the DESKTOP WINAPI_FAMILY partition, are declared in kernel32.h but deliberately left unwrapped here.

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

    BOOL Kernel32::CallbackMayRunLong( ::PTP_CALLBACK_INSTANCE pci )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CallbackMayRunLong );
        return Library::Call< BOOL, ::PTP_CALLBACK_INSTANCE >(pFunc, pci);
    }

    VOID Kernel32::CancelThreadpoolIo( ::PTP_IO pio )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CancelThreadpoolIo );
        Library::voidCall< ::PTP_IO >( pFunc, pio );
    }

    VOID Kernel32::CloseThreadpool( ::PTP_POOL ptpp )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CloseThreadpool );
        Library::voidCall< ::PTP_POOL >( pFunc, ptpp );
    }

    VOID Kernel32::CloseThreadpoolCleanupGroup( ::PTP_CLEANUP_GROUP ptpcg )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CloseThreadpoolCleanupGroup );
        Library::voidCall< ::PTP_CLEANUP_GROUP >( pFunc, ptpcg );
    }

    VOID Kernel32::CloseThreadpoolCleanupGroupMembers( ::PTP_CLEANUP_GROUP ptpcg, BOOL fCancelPendingCallbacks, void* pvCleanupContext )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CloseThreadpoolCleanupGroupMembers );
        Library::voidCall< ::PTP_CLEANUP_GROUP, BOOL, LPVOID >( pFunc, ptpcg, fCancelPendingCallbacks, pvCleanupContext );
    }

    VOID Kernel32::CloseThreadpoolIo( ::PTP_IO pio )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CloseThreadpoolIo );
        Library::voidCall< ::PTP_IO >( pFunc, pio );
    }

    VOID Kernel32::CloseThreadpoolTimer( ::PTP_TIMER pti )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CloseThreadpoolTimer );
        Library::voidCall< ::PTP_TIMER >( pFunc, pti );
    }

    VOID Kernel32::CloseThreadpoolWait( ::PTP_WAIT pwa )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CloseThreadpoolWait );
        Library::voidCall< ::PTP_WAIT >( pFunc, pwa );
    }

    VOID Kernel32::CloseThreadpoolWork( ::PTP_WORK pwk )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CloseThreadpoolWork );
        Library::voidCall< ::PTP_WORK >( pFunc, pwk );
    }

    VOID Kernel32::DisassociateCurrentThreadFromCallback( ::PTP_CALLBACK_INSTANCE pci )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, DisassociateCurrentThreadFromCallback );
        Library::voidCall< ::PTP_CALLBACK_INSTANCE >( pFunc, pci );
    }

    VOID Kernel32::FreeLibraryWhenCallbackReturns( ::PTP_CALLBACK_INSTANCE pci, HMODULE mod )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, FreeLibraryWhenCallbackReturns );
        Library::voidCall< ::PTP_CALLBACK_INSTANCE, HMODULE >( pFunc, pci, mod );
    }

    VOID Kernel32::LeaveCriticalSectionWhenCallbackReturns( ::PTP_CALLBACK_INSTANCE pci, ::PCRITICAL_SECTION pcs )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, LeaveCriticalSectionWhenCallbackReturns );
        Library::voidCall< ::PTP_CALLBACK_INSTANCE, ::PCRITICAL_SECTION >( pFunc, pci, pcs );
    }

    VOID Kernel32::ReleaseMutexWhenCallbackReturns( ::PTP_CALLBACK_INSTANCE pci, HANDLE mut )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, ReleaseMutexWhenCallbackReturns );
        Library::voidCall< ::PTP_CALLBACK_INSTANCE, HANDLE >( pFunc, pci, mut );
    }

    VOID Kernel32::ReleaseSemaphoreWhenCallbackReturns( ::PTP_CALLBACK_INSTANCE pci, HANDLE sem, DWORD crel )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, ReleaseSemaphoreWhenCallbackReturns );
        Library::voidCall< ::PTP_CALLBACK_INSTANCE, HANDLE, DWORD >( pFunc, pci, sem, crel );
    }

    VOID Kernel32::SetEventWhenCallbackReturns( ::PTP_CALLBACK_INSTANCE pci, HANDLE evt )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetEventWhenCallbackReturns );
        Library::voidCall< ::PTP_CALLBACK_INSTANCE, HANDLE >( pFunc, pci, evt );
    }

    // Header-inline (variant I) wrappers - DECISIONS.md D11. These six entry points have no DLL export: WinBase.h
    // defines them FORCEINLINE over the FORCEINLINE Tp* helpers in winnt.h, so the qor_pp_useswinapi and GetProcAddress
    // path used by every other wrapper in this file resolves null and Library::Call throws at run time. They bind at
    // compile time through Library::voidInlineCall in api_layer/library.h instead, which emits no import. The :: prefix
    // selects the global SDK definition, not the Kernel32 member being defined here, so there is no recursion.
    VOID Kernel32::InitializeThreadpoolEnvironment( ::PTP_CALLBACK_ENVIRON pcbe )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_usesinlineapi( InitializeThreadpoolEnvironment );
        Library::voidInlineCall< ::PTP_CALLBACK_ENVIRON >( ::InitializeThreadpoolEnvironment, pcbe );
    }

    VOID Kernel32::DestroyThreadpoolEnvironment( ::PTP_CALLBACK_ENVIRON pcbe )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_usesinlineapi( DestroyThreadpoolEnvironment );
        Library::voidInlineCall< ::PTP_CALLBACK_ENVIRON >( ::DestroyThreadpoolEnvironment, pcbe );
    }

    VOID Kernel32::SetThreadpoolCallbackPool( ::PTP_CALLBACK_ENVIRON pcbe, ::PTP_POOL ptpp )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_usesinlineapi( SetThreadpoolCallbackPool );
        Library::voidInlineCall< ::PTP_CALLBACK_ENVIRON, ::PTP_POOL >( ::SetThreadpoolCallbackPool, pcbe, ptpp );
    }

    VOID Kernel32::SetThreadpoolCallbackLibrary( ::PTP_CALLBACK_ENVIRON pcbe, void* mod )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_usesinlineapi( SetThreadpoolCallbackLibrary );
        Library::voidInlineCall< ::PTP_CALLBACK_ENVIRON, void* >( ::SetThreadpoolCallbackLibrary, pcbe, mod );
    }

    VOID Kernel32::SetThreadpoolCallbackCleanupGroup( ::PTP_CALLBACK_ENVIRON pcbe, ::PTP_CLEANUP_GROUP ptpcg, ::PTP_CLEANUP_GROUP_CANCEL_CALLBACK pfng )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_usesinlineapi( SetThreadpoolCallbackCleanupGroup );
        Library::voidInlineCall< ::PTP_CALLBACK_ENVIRON, ::PTP_CLEANUP_GROUP, ::PTP_CLEANUP_GROUP_CANCEL_CALLBACK >( ::SetThreadpoolCallbackCleanupGroup, pcbe, ptpcg, pfng );
    }

    VOID Kernel32::SetThreadpoolCallbackRunsLong( ::PTP_CALLBACK_ENVIRON pcbe )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_usesinlineapi( SetThreadpoolCallbackRunsLong );
        Library::voidInlineCall< ::PTP_CALLBACK_ENVIRON >( ::SetThreadpoolCallbackRunsLong, pcbe );
    }

    ::PTP_POOL Kernel32::CreateThreadpool( void* reserved )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CreateThreadpool );
        ::PTP_POOL pResult = Library::Call< ::PTP_POOL, LPVOID >(pFunc, reserved);
        if (pResult == nullptr)
        {
            continuable("Windows API error");
        }
        return pResult;
    }

    ::PTP_CLEANUP_GROUP Kernel32::CreateThreadpoolCleanupGroup(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CreateThreadpoolCleanupGroup );
        ::PTP_CLEANUP_GROUP pResult = Library::Call< ::PTP_CLEANUP_GROUP >(pFunc);
        if (pResult == nullptr)
        {
            continuable("Windows API error");
        }
        return pResult;
    }

    ::PTP_IO Kernel32::CreateThreadpoolIo( HANDLE fl, ::PTP_WIN32_IO_CALLBACK pfnio, void* pv, ::PTP_CALLBACK_ENVIRON pcbe )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CreateThreadpoolIo );
        ::PTP_IO pResult = Library::Call< ::PTP_IO, HANDLE, ::PTP_WIN32_IO_CALLBACK, LPVOID, ::PTP_CALLBACK_ENVIRON >(pFunc, fl, pfnio, pv, pcbe);
        if (pResult == nullptr)
        {
            continuable("Windows API error");
        }
        return pResult;
    }

    ::PTP_TIMER Kernel32::CreateThreadpoolTimer( ::PTP_TIMER_CALLBACK pfnti, void* pv, ::PTP_CALLBACK_ENVIRON pcbe )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CreateThreadpoolTimer );
        ::PTP_TIMER pResult = Library::Call< ::PTP_TIMER, ::PTP_TIMER_CALLBACK, LPVOID, ::PTP_CALLBACK_ENVIRON >(pFunc, pfnti, pv, pcbe);
        if (pResult == nullptr)
        {
            continuable("Windows API error");
        }
        return pResult;
    }

    ::PTP_WAIT Kernel32::CreateThreadpoolWait( ::PTP_WAIT_CALLBACK pfnwa, void* pv, ::PTP_CALLBACK_ENVIRON pcbe )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CreateThreadpoolWait );
        ::PTP_WAIT pResult = Library::Call< ::PTP_WAIT, ::PTP_WAIT_CALLBACK, LPVOID, ::PTP_CALLBACK_ENVIRON >(pFunc, pfnwa, pv, pcbe);
        if (pResult == nullptr)
        {
            continuable("Windows API error");
        }
        return pResult;
    }

    ::PTP_WORK Kernel32::CreateThreadpoolWork( ::PTP_WORK_CALLBACK pfnwk, void* pv, ::PTP_CALLBACK_ENVIRON pcbe )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, CreateThreadpoolWork );
        ::PTP_WORK pResult = Library::Call< ::PTP_WORK, ::PTP_WORK_CALLBACK, LPVOID, ::PTP_CALLBACK_ENVIRON >(pFunc, pfnwk, pv, pcbe);
        if (pResult == nullptr)
        {
            continuable("Windows API error");
        }
        return pResult;
    }

    BOOL Kernel32::IsThreadpoolTimerSet( ::PTP_TIMER pti )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, IsThreadpoolTimerSet );
        return Library::Call< BOOL, ::PTP_TIMER >(pFunc, pti);
    }
    VOID Kernel32::SetThreadpoolThreadMaximum( ::PTP_POOL ptpp, DWORD cthrdMost )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetThreadpoolThreadMaximum );
        Library::voidCall< ::PTP_POOL, DWORD >( pFunc, ptpp, cthrdMost );
    }

    BOOL Kernel32::SetThreadpoolThreadMinimum( ::PTP_POOL ptpp, DWORD cthrdMic )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetThreadpoolThreadMinimum );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, ::PTP_POOL, DWORD >(pFunc, ptpp, cthrdMic);
        return bResult;
    }

    VOID Kernel32::SetThreadpoolTimer( ::PTP_TIMER pti, ::PFILETIME pftDueTime, DWORD msPeriod, DWORD msWindowLength )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetThreadpoolTimer );
        Library::voidCall< ::PTP_TIMER, ::PFILETIME, DWORD, DWORD >( pFunc, pti, pftDueTime, msPeriod, msWindowLength );
    }

    VOID Kernel32::SetThreadpoolWait( ::PTP_WAIT pwa, HANDLE h, ::PFILETIME pftTimeout )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SetThreadpoolWait );
        Library::voidCall< ::PTP_WAIT, HANDLE, ::PFILETIME >( pFunc, pwa, h, pftTimeout );
    }

    VOID Kernel32::StartThreadpoolIo( ::PTP_IO pio )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, StartThreadpoolIo );
        Library::voidCall< ::PTP_IO >( pFunc, pio );
    }

    VOID Kernel32::SubmitThreadpoolWork( ::PTP_WORK pwk )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, SubmitThreadpoolWork );
        Library::voidCall< ::PTP_WORK >( pFunc, pwk );
    }

    BOOL Kernel32::TrySubmitThreadpoolCallback( ::PTP_SIMPLE_CALLBACK pfns, void* pv, ::PTP_CALLBACK_ENVIRON pcbe )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, TrySubmitThreadpoolCallback );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, ::PTP_SIMPLE_CALLBACK, LPVOID, ::PTP_CALLBACK_ENVIRON >(pFunc, pfns, pv, pcbe);
        return bResult;
    }

    void Kernel32::WaitForThreadpoolIoCallbacks( ::PTP_IO pio, BOOL fCancelPendingCallbacks )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, WaitForThreadpoolIoCallbacks );
        Library::voidCall< ::PTP_IO, BOOL >( pFunc, pio, fCancelPendingCallbacks );
    }

    void Kernel32::WaitForThreadpoolTimerCallbacks( ::PTP_TIMER pti, BOOL fCancelPendingCallbacks )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, WaitForThreadpoolTimerCallbacks );
        Library::voidCall< ::PTP_TIMER, BOOL >( pFunc, pti, fCancelPendingCallbacks );
    }

    void Kernel32::WaitForThreadpoolWaitCallbacks( ::PTP_WAIT pwa, BOOL fCancelPendingCallbacks )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, WaitForThreadpoolWaitCallbacks );
        Library::voidCall< ::PTP_WAIT, BOOL >( pFunc, pwa, fCancelPendingCallbacks );
    }

    void Kernel32::WaitForThreadpoolWorkCallbacks( ::PTP_WORK pwk, BOOL fCancelPendingCallbacks )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, WaitForThreadpoolWorkCallbacks );
        Library::voidCall< ::PTP_WORK, BOOL >( pFunc, pwk, fCancelPendingCallbacks );
    }

}}}//qor::win::api
