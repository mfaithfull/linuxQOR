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

    HDEVNOTIFY User32::RegisterDeviceNotification(HANDLE hRecipient, LPVOID notificationFilter, DWORD Flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, RegisterDeviceNotification);
        return Library::Call< HDEVNOTIFY, HANDLE, LPVOID, DWORD >(pFunc, hRecipient, notificationFilter, Flags);
    }

    BOOL User32::UnregisterDeviceNotification(HDEVNOTIFY handle)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UnregisterDeviceNotification);
        return Library::Call< BOOL, HDEVNOTIFY >(pFunc, handle);
    }

}}}//qor::win::api
