// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Drag and drop entry points of shell32.dll. See shell.h for the scope of this library and purpose.md for the audit trail
//that established that every name below is exported by the shell32.dll on the target system.

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

    //DragAcceptFiles is a void entry point, so there is nothing to check and the SDK annotation carries no failure state
    void Shell::DragAcceptFiles( ::HWND hWnd, BOOL fAccept )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, DragAcceptFiles );
        Library::voidCall< ::HWND, BOOL >( pFunc, hWnd, fAccept );
    }

    //DragFinish releases the drop handle and has no return value
    void Shell::DragFinish( ::HDROP hDrop )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, DragFinish );
        Library::voidCall< ::HDROP >( pFunc, hDrop );
    }

    //DragQueryFile is exported as the A and W variants separately, so each wrapper binds its own name rather than using
    //the AW stem macro. The return value is either the file count for iFile (UINT)-1 or the number of characters copied;
    //zero is a meaningful result rather than a failure state, so the return is passed through unchecked.
    UINT Shell::DragQueryFileA( ::HDROP hDrop, UINT iFile, LPSTR lpszFile, UINT cch )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, DragQueryFileA );
        return Library::Call< UINT, ::HDROP, UINT, LPSTR, UINT >(pFunc, hDrop, iFile, lpszFile, cch);
    }

    UINT Shell::DragQueryFileW( ::HDROP hDrop, UINT iFile, LPWSTR lpszFile, UINT cch )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, DragQueryFileW );
        return Library::Call< UINT, ::HDROP, UINT, LPWSTR, UINT >(pFunc, hDrop, iFile, lpszFile, cch);
    }

    //DragQueryPoint reports whether the drop point is inside the drop region, so FALSE is a valid answer rather than a
    //failure and the BOOL is passed through unchecked. The point itself is only valid when the return is TRUE.
    BOOL Shell::DragQueryPoint( ::HDROP hDrop, ::POINT* ppt )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, DragQueryPoint );
        return Library::Call< BOOL, ::HDROP, ::POINT* >(pFunc, hDrop, ppt);
    }

}}} //qor::win::api
