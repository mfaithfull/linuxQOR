// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    BOOL User32::FlashWindow(HWND hWnd, BOOL bInvert)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, FlashWindow);
        return Library::Call<BOOL, HWND, BOOL>(pFunc, hWnd, bInvert);
    }

    BOOL User32::FlashWindowEx(::PFLASHWINFO pfwi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, FlashWindowEx);
        return Library::Call<BOOL, ::PFLASHWINFO>(pFunc, pfwi);
    }

    BOOL User32::MessageBeep(UINT uType)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, MessageBeep);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, UINT >(
             pFunc, uType );
        return bResult;
    }

    void User32::SetLastErrorEx(DWORD dwErrCode, DWORD dwType)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetLastErrorEx);
        Library::voidCall< DWORD, DWORD >(pFunc, dwErrCode, dwType );
    }

}}}//qor::win::api
