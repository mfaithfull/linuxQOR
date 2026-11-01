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

    //The "//Hook functions" section (user32.h:315-319): four declarations, all static, all four defined here - in the
    //header's order, all User32::qualified, and all four Call<> sites explicit about the SDK argument types (U14:
    //4/4 explicit, 4/4 spaced style). 0 U11, 0 U13, 0 U17 - the two A/W wrappers do carry the folder's T suffix, which
    //is the census norm (108 members) rather than one of the 11 U17 deviations.
    //Bindings, all four verified three ways (cl /EP literal + export table + live GetProcAddress): CallMsgFilterT
    //resolves to "CallMsgFilterW", CallNextHookEx to "CallNextHookEx", SetWindowsHookExT to "SetWindowsHookExW" and
    //UnhookWindowsHookEx to "UnhookWindowsHookEx" (tmp/_tmp_hook_pp.txt). The table
    //(tmp/_tmp_user32_exports.txt:1532-1535, :2405-2407, :2452) has CallMsgFilter 1C / CallMsgFilterA 1D /
    //CallMsgFilterW 1E, CallNextHookEx 1F, SetWindowsHookExA 38A / SetWindowsHookExAW 38B / SetWindowsHookExW 38C and
    //UnhookWindowsHookEx 3BF. The live probe resolves nine of those spellings and reports bare SetWindowsHookEx
    //MISSING, so the two AW bindings are load-bearing (a bare binding there asks for a name that does not exist once
    //G1's SDK macro is out of scope) while the two bare ones are the D13 spelling: there is no CallNextHookExA/W or
    //UnhookWindowsHookExA/W in the table.
    //The trap this TU contributes to pre-scan P6: bare CallMsgFilter IS exported (1C) at RVA 00088CF0, the same RVA as
    //CallMsgFilterA - the bare stem is a hidden alias of the narrow entry point, so a mis-set binding here would run the
    //ANSI variant on wide arguments instead of throwing "Missing library function." as every other wrong name in this
    //review does. The same table also has a real export named SetWindowsHookExAW (38B) that no header declares, which
    //is why the P5 wrong-stem scan (428 bindings, 4 flags, 0 in this TU) is the only instrument that catches an
    //A-suffixed stem handed to the AW macro: that spelling resolves too.
    //Signatures match the SDK exactly: CallMsgFilterW(LPMSG, int) winuser.h:5401-5403 (A twin :5395-5397),
    //SetWindowsHookExW(int, HOOKPROC, HINSTANCE, DWORD) :10384-10388, UnhookWindowsHookEx(HHOOK) :10399-10400,
    //CallNextHookEx(HHOOK, int, WPARAM, LPARAM) :10405-10409. Return types agree too (BOOL / LRESULT / HHOOK / BOOL),
    //so no U18 instance, and none of the four returns is a U24-shaped value: CallNextHookEx's LRESULT is hook-specific
    //by contract, UnhookWindowsHookEx's BOOL and CallMsgFilter's BOOL are plain success flags. Optional-input shape:
    //hhk is _In_opt_ at :10406 and hmod is _In_opt_ at :10387, both admitted by the wrapper's handle types; idHook,
    //lpfn, nCode, wParam and lParam are _In_.
    //One parameter-name divergence from the SDK, fixed here: user32.h:318 wrote hMod where winuser.h:10379 has hmod
    //(header, definition and Call<> site), because the folder otherwise copies the SDK's spellings verbatim - compare
    //the hmodWinEventProc of the SetWinEventHook wrapper at user32.h:662.
    //Ownership - the first section where the folder's "an acquired handle is returned for the caller to release" rule
    //clearly applies: SetWindowsHookExT acquires an HHOOK that only UnhookWindowsHookEx releases, and the layer has no
    //RAII holder for it. Two further contracts the caller owns: the HOOKPROC passed in must stay valid for the hook's
    //whole lifetime, and for a global hook (dwThreadId 0 with a non-NULL hmod) it must live in the module named by
    //hmod - this wrapper admits that shape unchanged - and the HHOOK is only meaningful in the process and window
    //station that created it. Since nothing calls this section (U15 4/4), no leak site exists today.
    //U16: 0 of 4 wrappers pair with a version-guarded prototype, but this is the section whose types are opt-out
    //sensitive. LPMSG/MSG is itself inside #ifndef NOMSG (winuser.h:1869, typedef :1877-1887) exactly like the
    //CallMsgFilter prototypes (:5390), and HOOKPROC sits inside #ifndef NOUSER (:64) plus the Desktop partition
    //(:119 STRICT arm, :162 !STRICT arm), so NOMSG or NOUSER in a consumer build leaves user32.h:316/:318 ill-formed
    //rather than merely unprototyped. The two handles escape every opt-out (DECLARE_HANDLE(HHOOK) is
    //shared\windef.h:40, outside NOMSG/NOWH/NOUSER) but not the STRICT switch - see U27 below. Guard stacks measured
    //with tmp/_tmp_hook_guards.ps1 (8 hits over 16475 lines; the two extra CallNextHookEx hits :10416/:10419 are the
    //DefHookProc compatibility macro, not declarations).
    //New open item U27 from that corner: STRICT changes the identity of both HOOKPROC (typed callback at
    //winuser.h:119 vs FARPROC at :162) and every DECLARE_HANDLE type (ntdef.h:851-861: opaque struct pointer when
    //STRICT, plain HANDLE/void* when not), so it changes the published (mangled) name. tmp/_tmp_hookproc_probe.cpp
    //+ tmp/_tmp_hookproc_probe.bat compile the same four declarations in both arms: 3 of the 4 names differ -
    //?CallNextHookEx@...@@SA_JPEAUHHOOK__@@H_K_J@Z against ...SA_JPEAXH_K_J@Z, ?UnhookWindowsHookEx@...@@SAHPEAUHHOOK__@@@Z
    //against ...@@SAHPEAX@Z, and SetWindowsHookExT moving its return, its HINSTANCE and its callback all at once
    //(...@@SAPEAUHHOOK__@@HP6A_JH_K_J@ZPEAUHINSTANCE__@@K@Z against ...@@SAPEAXHP6A_JXZPEAXK@Z). Only CallMsgFilterT is
    //arm-stable, because it names just MSG* and int. The library is built with STRICT on by default (minwindef.h:21-25
    //and ntdef.h:785-789 both #define STRICT 1 unless NO_STRICT), so a consumer that defines NO_STRICT compiles
    //against this header cleanly and ends with an unresolved external for a name that plainly matches it.
    //Section coverage: the SDK's #ifndef NOWH block also declares SetWindowsHookA/W (:10329, :10335, !STRICT twins
    //:10349, :10355) and UnhookWindowsHook (:10369); all three are exported (live probe RESOLVED) and named nowhere in
    //user32.h - 3 unwrapped entry points. The folder's "Hook functions" section also spans two SDK blocks, since
    //CallMsgFilter lives in the NOMSG block at :5390 rather than in NOWH. The rest of the hook surface sits in three
    //other TUs: the WinEvent trio in accessibility.cpp, GetMessageExtraInfo/SetMessageExtraInfo in message.cpp and the
    //shell-hook pair, declared at user32.h:147/:194 but defined in window.cpp:145 (DeregisterShellHookWindow) and
    //:474 (RegisterShellHookWindow) - a U21/U12-shaped placement note, nothing moved here.
    //Liveness (U15 4/4 dead): tmp/_tmp_bypass_scan.ps1 over the four stems reports stems 4, LAYER 0, INDIRECT 0,
    //BYPASS 0, MEMBER 0 - the first section of the review with no caller anywhere and no SDK-import call either,
    //confirmed by a raw tree grep in which HHOOK and HOOKPROC appear in no file of src/examples/test at all.
    //U1 0 handlers and U2 0 CheckReturn over the 4 call sites (settled policy); ../returncheck.h is included and unused.
    BOOL User32::CallMsgFilterT(LPMSG lpMsg, int nCode)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, CallMsgFilter);
        return Library::Call< BOOL, LPMSG, int >(pFunc, lpMsg, nCode);
    }

    LRESULT User32::CallNextHookEx(HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CallNextHookEx);
        return Library::Call< LRESULT, HHOOK, int, WPARAM, LPARAM >(pFunc, hhk, nCode, wParam, lParam);
    }

    HHOOK User32::SetWindowsHookExT(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SetWindowsHookEx);
        return Library::Call< HHOOK, int, HOOKPROC, HINSTANCE, DWORD >(pFunc, idHook, lpfn, hmod, dwThreadId);
    }

    BOOL User32::UnhookWindowsHookEx(HHOOK hhk)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UnhookWindowsHookEx);
        return Library::Call< BOOL, HHOOK >(pFunc, hhk);
    }

}}}//qor::win::api
