// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Shell notification, status and taskbar entry points of shell32.dll. See shell.h for the scope of this library and
//purpose.md for the audit trail that established that every name below is exported by the shell32.dll on the target
//system.

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "../kernel/errorhandler.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "shell.h" //shell.h must be the first windows header as it's the primary inclusion point for windows.h
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //Shell_NotifyIcon registers or updates the taskbar icon and reports success through the BOOL return. The A and W
    //variants take the matching NOTIFYICONDATA structure and are separate exports.
    BOOL Shell::Shell_NotifyIconA( DWORD dwMessage, ::PNOTIFYICONDATAA lpData )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, Shell_NotifyIconA );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, DWORD, ::PNOTIFYICONDATAA* >(pFunc, dwMessage, lpData);
        return bResult;
    }

    BOOL Shell::Shell_NotifyIconW( DWORD dwMessage, ::PNOTIFYICONDATAW lpData )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, Shell_NotifyIconW );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, DWORD, ::PNOTIFYICONDATAW* >(pFunc, dwMessage, lpData);
        return bResult;
    }

    //Shell_NotifyIconGetRect fills the RECT out parameter only when the identifier resolves to a registered icon, so the
    //HRESULT is the contract to test
    HRESULT Shell::Shell_NotifyIconGetRect( const ::NOTIFYICONIDENTIFIER* identifier, ::RECT* iconLocation )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, Shell_NotifyIconGetRect );
        CheckReturn< HRESULT, LongSuccessCheck >::TType hrResult = Library::Call< HRESULT, const ::NOTIFYICONIDENTIFIER*, ::RECT* >(pFunc, identifier, iconLocation);
        return hrResult;
    }

    HRESULT Shell::SHQueryUserNotificationState( ::QUERY_USER_NOTIFICATION_STATE* pquns )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHQueryUserNotificationState );
        CheckReturn< HRESULT, LongSuccessCheck >::TType hrResult = Library::Call< HRESULT, ::QUERY_USER_NOTIFICATION_STATE* >(pFunc, pquns);
        return hrResult;
    }

    HRESULT Shell::SHIsFileAvailableOffline( PCWSTR pwszPath, DWORD* pdwStatus )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHIsFileAvailableOffline );
        CheckReturn< HRESULT, LongSuccessCheck >::TType hrResult = Library::Call< HRESULT, PCWSTR, DWORD* >(pFunc, pwszPath, pdwStatus);
        return hrResult;
    }

    //SHAppBarMessage returns the DWORD_PTR encoded state of the taskbar item the message was sent for, so zero is a
    //meaningful answer rather than a failure state and the value is passed through unchecked. The dwMessage selects
    //which of the WM_TASKBARMessages is sent, and the pData structure carries both the in and out state.
    DWORD_PTR Shell::SHAppBarMessage( DWORD dwMessage, ::PAPPBARDATA pData )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHAppBarMessage );
        return Library::Call< DWORD_PTR, DWORD, ::PAPPBARDATA* >(pFunc, dwMessage, pData);
    }

    //InitNetworkAddressControl opens the network address control dialog and reports failure through the BOOL return
    BOOL Shell::InitNetworkAddressControl( void )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, InitNetworkAddressControl );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL >(pFunc);
        return bResult;
    }

}}} //qor::win::api
