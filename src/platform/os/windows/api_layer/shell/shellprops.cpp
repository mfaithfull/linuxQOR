// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Shell object localisation and property store entry points of shell32.dll. See shell.h for the scope of this library and
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

    //SHSetLocalizedName binds a display name to a shell path through a resource module and resource id, which is the
    //mechanism the shell uses for the localised names shown in Explorer
    HRESULT Shell::SHSetLocalizedName( PCWSTR pszPath, PCWSTR pszResModule, INT idsRes )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHSetLocalizedName );
        CheckReturn< HRESULT, LongSuccessCheck >::TType hrResult = Library::Call< HRESULT, PCWSTR, PCWSTR, INT >(pFunc, pszPath, pszResModule, idsRes);
        return hrResult;
    }

    //SHRemoveLocalizedName detaches the display name from a shell path
    HRESULT Shell::SHRemoveLocalizedName( PCWSTR pszPath )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHRemoveLocalizedName );
        CheckReturn< HRESULT, LongSuccessCheck >::TType hrResult = Library::Call< HRESULT, PCWSTR >(pFunc, pszPath);
        return hrResult;
    }

    //SHGetLocalizedName writes the resource module name into the caller supplied buffer and reports the resource id for it,
    //so the buffer size in cch bounds the only out parameter that can overflow
    HRESULT Shell::SHGetLocalizedName( PCWSTR pszPath, PWSTR pszResModule, UINT cch, INT* pidsRes )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHGetLocalizedName );
        CheckReturn< HRESULT, LongSuccessCheck >::TType hrResult = Library::Call< HRESULT, PCWSTR, PWSTR, UINT, INT* >(pFunc, pszPath, pszResModule, cch, pidsRes);
        return hrResult;
    }

    //AssocCreateForClasses builds the file association COM object for the set of shell object classes described by the
    //ASSOCIATIONELEMENT array
    HRESULT Shell::AssocCreateForClasses( const ::ASSOCIATIONELEMENT* rgClasses, ULONG cClasses, REFIID riid, void** ppv )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, AssocCreateForClasses );
        CheckReturn< HRESULT, LongSuccessCheck >::TType hrResult = Library::Call< HRESULT, const ::ASSOCIATIONELEMENT*, ULONG, REFIID, void** >(pFunc, rgClasses, cClasses, riid, ppv);
        return hrResult;
    }

    //SHGetPropertyStoreForWindow resolves the shell property store interface for a window, which is how the shell exposes
    //the taskbar and pinned item properties to the caller
    HRESULT Shell::SHGetPropertyStoreForWindow( ::HWND hwnd, REFIID riid, void** ppv )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(shell32, SHGetPropertyStoreForWindow );
        CheckReturn< HRESULT, LongSuccessCheck >::TType hrResult = Library::Call< HRESULT, ::HWND, REFIID, void** >(pFunc, hwnd, riid, ppv);
        return hrResult;
    }

}}} //qor::win::api
