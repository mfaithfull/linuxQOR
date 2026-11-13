// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "../kernel/errorhandler.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //GetSysColorBrush is a single user32.dll export - winuser.h:9694-9698 declares it directly (guarded
    //WINVER >= 0x0400) with no A/W family and no stem -> stemW macro, so the bare stem is the name
    //GetProcAddress is given. cl /EP on this TU resolves it to "GetSysColorBrush".
    //The returned brush is a system-cached object owned by USER32: it must be selected into a DC but never
    //deleted, so the handle is handed back raw and the caller is responsible for not releasing it.

    //Return policy (U2, table row GetSysColorBrush / brush.cpp, 11 Nov 2026): the row reads UNCHECKED with no checkClass,
    //and the table's verbatim docQuote is "The return value identifies a logical brush if the nIndex parameter is
    //supported by the current platform. Otherwise, it returns NULL ." - the NULL is a legal answer for an nIndex the
    //current platform does not support, not a failure value, so the session-27 soundness test (a checked-failure site is
    //only sound where the SDK's failure value is not also a legal success value) fails at that value and a
    //CheckNonZeroHandle< HBRUSH > would report a supported-platform answer and a not-supported answer alike. The page
    //names no failure value for this stem at all, so there is nothing for a check to key on. No token and no
    //../returncheck.h include here - this TU keeps its 0 CheckReturn sites. The row carries 1 in-tree caller, so the
    //audit that would precede any token is moot while the row stays unchecked.
    HBRUSH User32::GetSysColorBrush(int nIndex)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetSysColorBrush);
        return Library::Call<HBRUSH, int>(pFunc, nIndex);
    }

}}}//qor::win::api
