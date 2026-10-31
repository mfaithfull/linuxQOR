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
    //The "Console functions" section (user32.h:689-690): one wrapper, one binding. Section coverage is complete -
    //ConsoleControl is the only *Console*-named entry point in the user32.dll export table (ordinal 1594, hint 55,
    //RVA 00028FD0, and not one of the four exports the table marks "forwarded to").
    //ConsoleControl has NO SDK declaration at any guard level, and none of the console-internal types it takes -
    //CONSOLECONTROL plus the five structures at user32.h:19-62 - is defined by the installed SDK either (the tokens
    //CONSOLECONTROL / _CONSOLEENDTASK / CONSOLE_CARET_INFO occur nowhere under Include\10.0.26100.0 nor
    //Include\10.0.10240.0, so console.cpp and desktop.cpp cannot collide with SDK definitions of them - status.md
    //U10). The types are therefore the project's only definition of them, mirrored from the de-facto published
    //internal definition: there is no Microsoft prototype to pair this signature against, and the export table is
    //the only authority for the name. The enumeration stops at ConsoleEndTask (7) with Reserved1/2/3 holes, and
    //whether the running console adds control codes past that cannot be checked from anything installed here.
    //There is no A/W family (live probe: ConsoleControl RESOLVED, ConsoleControlA and ConsoleControlW MISSING), so
    //the bare qor_pp_useswinapi is the D13 spelling and cl /EP confirms the literal "ConsoleControl" reaches
    //GetProcAddress; no T suffix, which is correct for a single export.
    //The buffer pair is opaque and API-validated: ConsoleInformation must point at the structure that belongs to the
    //chosen CONSOLECONTROL value (the enumerator names pair with the structure names at user32.h:31-62) and
    //ConsoleInformationLength must be that structure's size. The API reads and writes the buffer, which stays
    //caller-owned - the call allocates nothing and frees nothing.
    //The return is NTSTATUS (the SDK typedef LONG NTSTATUS from winnt.h, annotated _Return_type_success_(return >= 0)):
    //zero is success, positive is an informational success, only negative is failure, so "== 0" is not a valid
    //failure test and returncheck.h offers no NTSTATUS-aware check. Per the settled policy (status.md U1/U2) no
    //Win32ErrorHandler and no CheckReturn is used here, so every failure status travels silently; note that
    //Library::Call turns a swallowed SEH fault into a default-constructed NTSTATUS of 0, i.e. this wrapper reports a
    //fault as STATUS_SUCCESS (status.md U7).
    //Several control codes are security- and console-ownership-sensitive (ConsoleSetForeground,
    //ConsoleSetWindowOwner, ConsoleEndTask) and fail with an access-denied status for a caller that is not the
    //console owner.

    NTSTATUS User32::ConsoleControl(CONSOLECONTROL Command, PVOID ConsoleInformation, DWORD ConsoleInformationLength)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ConsoleControl);
        return Library::Call<NTSTATUS, CONSOLECONTROL, PVOID, DWORD>(pFunc, Command, ConsoleInformation, ConsoleInformationLength);
    }

}}}//qor::win::api
