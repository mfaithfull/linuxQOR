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

    //GetSysColorBrush is a single user32.dll export - winuser.h:9694-9698 declares it directly (guarded
    //WINVER >= 0x0400) with no A/W family and no stem -> stemW macro, so the bare stem is the name
    //GetProcAddress is given. cl /EP on this TU resolves it to "GetSysColorBrush".
    //The returned brush is a system-cached object owned by USER32: it must be selected into a DC but never
    //deleted, so the handle is handed back raw and the caller is responsible for not releasing it.

    HBRUSH User32::GetSysColorBrush(int nIndex)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetSysColorBrush);
        return Library::Call<HBRUSH, int>(pFunc, nIndex);
    }

}}}//qor::win::api
