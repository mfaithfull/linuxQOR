// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "../kernel/errorhandler.h"

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
    //U1 0 handlers and U2 0 CheckReturn over the 4 call sites (settled policy); its dead ../returncheck.h include went (T7).
    //Addendum, 12 Nov 2026 (U2 Small TU batch S3, item hook sites): that sentence is the review's record and stands
    //superseded - the endpoint table now reads SetWindowsHookEx (CheckNonZeroHandle< HHOOK >) and UnhookWindowsHookEx
    //(BoolCheck) checked and only CallMsgFilter and CallNextHookEx unchecked, so this TU carries 2 CheckReturn sites and
    //gained its one ../returncheck.h include (0 -> 2). The decision is the table's, not re-derived here; each row's verbatim
    //docQuote and class sits at the site below, and the two unchecked rows carry their written reason.
    //BOOL, but the two values are answers to the message chain's question rather than a status word.
    //Return policy (U2, table row CallMsgFilter / hook.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, and
    //the table's verbatim docQuote is "Type: BOOL If the application should process the message further, the return value
    //is zero. If the application should not process the message further, the return value is nonzero." - both values are
    //answers the caller acts on, so neither is a failure value and a BoolCheck there would report a legal "process it
    //further" zero as a failure; the session-27 test fails at both before any check is considered. The row is an AW
    //binding with NO-PROTOTYPE in the census and BOOL published; no token from this row and the flag comes back as
    //the SDK wrote it.
    BOOL User32::CallMsgFilterT(LPMSG lpMsg, int nCode)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, CallMsgFilter);
        return Library::Call< BOOL, LPMSG, int >(pFunc, lpMsg, nCode);
    }

    //The hook chain's own result word - the value the next hook procedure hands back.
    //Return policy (U2, table row CallNextHookEx / hook.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, and
    //the table's verbatim docQuote is "Type: LRESULT This value is returned by the next hook procedure in the chain. The
    //current hook procedure must also return this value. The meaning of the return value depends on the hook type. For
    //more information, see the descriptions of the individual hook procedures." - the page reserves no failure value at
    //all, the LRESULT means whatever the hook type says it means, so any check would report a legal hook answer as a
    //failure and the session-27 test fails at every value; the word is handed over unchanged, which is also what the
    //hook-chain contract needs. No token from this row.
    LRESULT User32::CallNextHookEx(HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CallNextHookEx);
        return Library::Call< LRESULT, HHOOK, int, WPARAM, LPARAM >(pFunc, hhk, nCode, wParam, lParam);
    }

    //The registered hook's handle - the caller keeps it and passes it to UnhookWindowsHookEx to remove the hook.
    //Return policy (U2, table row SetWindowsHookEx / hook.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HHOOK >, and the table's verbatim docQuote is "Type: HHOOK If the function succeeds, the return
    //value is the handle to the hook procedure. If the function fails, the return value is NULL . To get extended error
    //information, call GetLastError ." - NULL is reserved for failure and the answer this member hands back is the hook
    //handle the caller needs for the matching UnhookWindowsHookEx call, for which the page gives no legal NULL, so the
    //session-27 soundness test (a checked-failure site is only sound where the SDK's failure value is not also a legal
    //success value) passes at every value. The row is an AW binding with NO-PROTOTYPE in the census, so the class base is
    //the published HHOOK the wrapper holds rather than an SDK prototype line. CheckNonZeroHandle (returncheck.h:136,
    //Test at :144 is (value == 0x00000000) ? false : true) is the handle template the vocabulary already offers - no new
    //check class. 0 in-tree callers for the exact stem (the U15 scan above names 0 LAYER / 0 BYPASS / 0 MEMBER), so
    //nothing built today reaches this site; G6 keeps the check live on every call, so an unsuccessful registration now
    //raises a continuable issue instead of travelling as a NULL handle to UnhookWindowsHookEx (status.md U7).
    HHOOK User32::SetWindowsHookExT(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, SetWindowsHookEx);
        CheckReturn< HHOOK, CheckNonZeroHandle< HHOOK > >::TType hResult = Library::Call< HHOOK, int, HOOKPROC, HINSTANCE, DWORD >(pFunc, idHook, lpfn, hmod, dwThreadId);
        return hResult;
    }

    //BOOL success flag for the withdrawal: FALSE means the hook named by hhk is still installed. hhk is an in-parameter,
    //so this member answers nothing other than that success/failure pair.
    //Return policy (U2, table row UnhookWindowsHookEx / hook.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is nonzero. If
    //the function fails, the return value is zero. To get extended error information, call GetLastError ." - zero is
    //reserved for failure and this member answers nothing else, so the session-27 soundness test passes at every value
    //and BOOL + zero-means-failure takes the BoolCheck the library already has (returncheck.h:94, value ? true : false)
    //- no new check class. 0 in-tree callers for the exact stem (D14 published surface); G6 keeps the check live on every
    //call, so a failed withdrawal now raises a continuable issue the Win32ErrorHandler above reports rather than travelling
    //as a silent success (status.md U7).
    BOOL User32::UnhookWindowsHookEx(HHOOK hhk)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, UnhookWindowsHookEx);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HHOOK >(pFunc, hhk);
        return bResult;
    }

}}}//qor::win::api
