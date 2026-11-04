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

    //Five wrappers - the whole "user32.h:601 //System Shutdwown functions" section (the header misspells its own
    //label). Declared :602-:606, all static, all defined here and in header order: :16 ExitWindowsEx, :23
    //LockWorkStation, :30 ShutdownBlockReasonCreate, :37 ShutdownBlockReasonDestroy, :44 ShutdownBlockReasonQuery.
    //5/5 declarations pair with a definition, so this TU contributes 0 U4 (declared, never defined), 0 U11
    //(definition not qualified with User32::), 0 U13, 0 U17 (A/W family without the folder's T suffix) and 0 U18
    //(return type contradicting the SDK); tmp/_tmp_sigcmp.ps1 -Tu systemshutdown.cpp -First 602 -Last 606 reports
    //5/5 decl==def on return type, parameter types, parameter names and Library::Call<> arity.
    //
    //Binding (D13): all five stems are single exports with no A/W family - tmp/_tmp_user32_exports.txt has
    //ExitWindowsEx (10F), LockWorkStation (26C), ShutdownBlockReasonCreate (39C), ShutdownBlockReasonDestroy (39D)
    //and ShutdownBlockReasonQuery (39E), a family walk over the 1,018-name table gives exactly 1 export per stem, and
    //the um headers carry 0 "#define <Stem>" alias sites, so the bare qor_pp_useswinapi at :19, :26, :33, :40 and :47
    //is the only spelling that can bind them at all. cl /EP over the TU emits exactly those five literals against
    //L"user32" (tmp/_tmp_s31_pp_pre.txt, read through the tmp/_tmp_s31_dig.ps1 digest): 5/5 in the export table and
    //no macro to rewrite the name (G1), hence 0 P1, 0 P2, 0 P6.
    //
    //Calling convention - fixed here 4 Nov 2026 (review session 31). Until then :37 and :44 read
    //"BOOL WINAPI User32::ShutdownBlockReason...", the only two WINAPI-qualified User32:: definition heads in all 38
    //TUs, while user32.h:605/:606 declare those members with no convention token. MSVC rejects the disagreement when
    //compiled for x86: "error C2373: redefinition; different type modifiers" at both sites (the note names the
    //declaration), so the token is not a redundant spelling - the TU is ill-formed. On x64, the only arch this tree
    //configures, the modifier is dropped: the symbol MSVC emits is the same __cdecl decoration a caller of the header
    //reference resolves to (?ShutdownBlockReasonDestroy@User32@api@win@qor@@SAHPEAUHWND__@@@Z on both sides of
    //tmp/_tmp_s31_winapi_s31.txt), and dumpbin /disasm of this TU's object is identical before and after the removal
    //(42,310 lines each, 2 differing lines, both only the dump header: tmp/_tmp_s31_x64_pre_s31.asm/_post_s31.asm).
    //The SDK does spell the five entry points "WINUSERAPI BOOL WINAPI" (winuser.h:3639, :14916, :15924, :15931,
    //:15939) - but those are free functions, where the token belongs. A wrapper's own convention is the C++ one its
    //in-class declaration states; the entry point's __stdcall is carried by Library::Call's fPtr (closed item U6).
    //
    //SDK visibility (U16): 5/5 prototypes are WINAPI_PARTITION_DESKTOP and exactly one is version-gated -
    //LockWorkStation under #if(_WIN32_WINNT >= 0x0500) at winuser.h:14912. ExitWindowsEx (winuser.h:3639, also inside
    //#ifndef NOUSER) and the ShutdownBlockReason trio (the #pragma region Desktop Family block at :15919, which also
    //defines MAX_STR_BLOCKREASON 256) carry no version floor at all. No suppression macro is defined in this build,
    //so all five pair with always-visible declarations on the arch that is built.
    //
    //Callers (U19 / D14): none anywhere. The comment-stripped scan over src, examples and test with all five stems
    //reports 0 through-layer, 0 intermediate-qualified, 0 SDK-bypass and 0 member-call - published surface, which
    //D14 says is not a finding, so no U15 or U19 row is recorded for this TU.
    //
    //Return policy (U1/U2): 5/5 return BOOL, 0 Win32ErrorHandler and 0 CheckReturn in all five wrappers, and no SDK
    //prototype carries _Check_return_, so a discarded FALSE leaves no trace on either side - including
    //ShutdownBlockReasonDestroy's documented ERROR_SHUTDOWN_IN_PROGRESS / ERROR_NOT_FOUND distinction and
    //ExitWindowsEx's failure when the shutdown is not permitted. :11 includes ../returncheck.h and uses none of it
    //(one of the 36 dead includes in this folder; left in place).
    //
    //Build note: this TU cannot be compiled for x86 for a reason outside itself - kernel32.h:92 names
    //PUNWIND_HISTORY_TABLE, which the SDK typedefs only under #if defined(_AMD64_)||defined(_ARM_)||defined(_ARM64_)
    //(winnt.h:21939-:21963), and that single line makes 93 of the 106 api_layer TUs fail with C2061 on x86
    //(tmp/_tmp_s31_x86_apiall_s31.txt) - recorded in registers/u6.md's session-31 addendum. It also hid the C2373 pair
    //above from every check until this session compiled for x86.

    BOOL User32::ExitWindowsEx(UINT uFlags, DWORD dwReason)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ExitWindowsEx);
        return Library::Call< BOOL, UINT, DWORD >(pFunc, uFlags, dwReason);
    }

    BOOL User32::LockWorkStation(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, LockWorkStation);
        return Library::Call< BOOL >(pFunc);
    }

    BOOL User32::ShutdownBlockReasonCreate(HWND hWnd, LPCWSTR pwszReason)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ShutdownBlockReasonCreate);
        return Library::Call< BOOL, HWND, LPCWSTR >(pFunc, hWnd, pwszReason);
    }

    BOOL User32::ShutdownBlockReasonDestroy(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ShutdownBlockReasonDestroy);
        return Library::Call< BOOL, HWND >(pFunc, hWnd);
    }

    BOOL User32::ShutdownBlockReasonQuery(HWND hWnd, LPWSTR pwszBuff, DWORD* pcchBuff)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ShutdownBlockReasonQuery);
        return Library::Call< BOOL, HWND, LPWSTR, DWORD* >(pFunc, hWnd, pwszBuff, pcchBuff);
    }

}}}//qor::win::api
