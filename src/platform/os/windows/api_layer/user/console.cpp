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

#undef WinHelp

namespace qor { namespace win { namespace api {

    NTSTATUS User32::ConsoleControl(CONSOLECONTROL Command, PVOID ConsoleInformation, DWORD ConsoleInformationLength)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ConsoleControl);
        return Library::Call<NTSTATUS, CONSOLECONTROL, PVOID, DWORD>(pFunc, Command, ConsoleInformation, ConsoleInformationLength);
    }

}}}//qor::win::api
