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

    BOOL User32::AttachThreadInput(DWORD idAttach, DWORD idAttachTo, BOOL fAttach)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, AttachThreadInput);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, DWORD, DWORD, BOOL >(
             pFunc, idAttach, idAttachTo, fAttach );
        return bResult;
    }

    DWORD User32::GetGuiResources(HANDLE hProcess, DWORD uiFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetGuiResources);
		CheckReturn< DWORD, CheckNonZero< DWORD> >::TType dwResult = Library::Call<DWORD, HANDLE, DWORD>(
            pFunc, hProcess, uiFlags);
        return dwResult;
    }

    BOOL User32::IsWow64Message(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsWow64Message);
        return Library::Call< BOOL >(pFunc);
    }

    BOOL User32::UserHandleGrantAccess(HANDLE hUserHandle, HANDLE hJob, BOOL bGrant)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UserHandleGrantAccess);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, HANDLE, BOOL >(
             pFunc, hUserHandle, hJob, bGrant );
        return bResult;
    }

    DWORD User32::WaitForInputIdle(HANDLE hProcess, DWORD dwMilliseconds)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, WaitForInputIdle);
        return Library::Call< DWORD, HANDLE, DWORD >(pFunc, hProcess, dwMilliseconds );
    }

}}}//qor::win::api
