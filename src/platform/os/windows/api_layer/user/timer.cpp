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

    void User32::NotifyWinEvent(DWORD dwEvent, HWND hwnd, LONG idObject, LONG idChild)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, NotifyWinEvent);
        return Library::voidCall<DWORD, HWND, LONG, LONG>( pFunc, dwEvent, hwnd, idObject, idChild );
    }

}}}//qor::win::api
