// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//=== api_layer conventions ========================================= Reviewed 7 Nov 2026
//   (T3 + T8 step 6e: Power Management functions - the power-settings pair and the suspend/resume pair, new TU)
//
//This TU is coverage added under D18 (docs/agent/DECISIONS.md): purpose.md states the library intends a full set of
//wrappers over the user32.dll exports, and registers/u18-coverage.md buckets the gap by family. The Shutdown / power
//bucket measures exactly 3 (CancelShutdown, RegisterSuspendResumeNotification, UnregisterSuspendResumeNotification) and
//it reconciles with nothing shed: all 3 are exported from user32.dll (ordinals 22, 2F6 and 3CD in the folder's capture
//tmp/_tmp_user32_exports.txt) AND declared in an SDK header, and the pre-scan (tmp/_tmp_t8_6e_pre.ps1 over the 41-name
//gap list written by tmp/_tmp_u18_coverage4.ps1) measures 0 function-like macros, 0 A/W siblings and 0 already declared
//in user32.h for every one of them: 3 in -> 3 wrapped, 0 excluded, nothing shed. 1 of the 3 (CancelShutdown) is a
//declaration into the existing systemshutdown.cpp; 2 of the 3 are this new TU.
//
//Signature rule (D18): the published signature IS the winuser.h prototype, parameter names included - 2/2 copy it
//exactly. SDK declaration lines: 4009 RegisterSuspendResumeNotification, 4017 UnregisterSuspendResumeNotification
//(winuser.h, um/10.0.26100.0). The SDK writes both names with a SPACE before the '(' - "RegisterSuspendResumeNotification (" -
//which is why every name scan in this folder allows \s* before the paren; without that, the pair measures 0 sites.
//The third name, CancelShutdown, is winuser.h:13587 BOOL WINAPI CancelShutdown(VOID);, wrapped in systemshutdown.cpp.
//
//RegisterSuspendResumeNotification (winuser.h:4009, HPOWERNOTIFY WINAPI, HANDLE + DWORD) and
//UnregisterSuspendResumeNotification (winuser.h:4017, BOOL WINAPI, one HPOWERNOTIFY). Both sit under the same
//guard stack the power pair above already stands under - #ifndef NOUSER, WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP),
//#if(WINVER >= 0x0500) and #if (_WIN32_WINNT >= 0x0502) - and the build pins neither: sdkddkver.h:228 defines
//_WIN32_WINNT 0x0A00 and :249 WINVER _WIN32_WINNT, and 0 of the 3,557 .h/.hpp/.cpp/.cmake/CMakeLists.txt files under
//src, cmake, examples and test define WINVER, _WIN32_WINNT, NTDDI_VERSION, NOUSER or NOGDI, so both are live and the
//layer adds no #if. Nothing in the tree defines _HPOWERNOTIFY_DEF_ either, which is the guard at winuser.h:3981 that
//owns the HPOWERNOTIFY typedef at :3985 - the type the pair speaks, and the reason the header could declare the power
//pair at all.

//Return conventions: the register call returns HPOWERNOTIFY, which the SDK typedefs PVOID at :3985 - a handle or NULL,
//and the SDK carries no _Check_return_ on it, so a discarded return leaves no trace on either side (U1/U2: no
//Win32ErrorHandler, no CheckReturn). The unregister call returns BOOL as a real success flag, same shape as UnregisterDeviceNotification
//above. Neither is a numeric -1-or-GetLastError convention, so there is nothing here that the U2 return-check idiom
//would add.

//The handle contract is the part a caller can get wrong, so it is stated, not coded. hRecipient is a HANDLE that must
//name a window or a service, and Flags selects which of the two it is - the SDK documents DEVICE_NOTIFY_WINDOW_HANDLE
//(winuser.h:3946, 0x00000000) and DEVICE_NOTIFY_SERVICE_HANDLE (:3947, 0x00000001); the values are not interchangeable,
//and 0 is a valid value, so a missing Flags argument is not something this layer can spot. The HPOWERNOTIFY the register
//call returns is the only way to unregister: passing anything else, or NULL, is the caller's error. The layer allocates
//nothing, sizes nothing and validates nothing on either call, and it never closes a notification on a caller's behalf.
//Liveness and signatures were COMPILED, not eyeballed: tmp/_tmp_t8_6e_protos.cpp declares function-pointer typedefs
//equal to the published signatures of CancelShutdown, RegisterSuspendResumeNotification and
//UnregisterSuspendResumeNotification and initialises each one with &<SDK name>. At /W4 with no QOR define and no
//version macro of its own it compiles clean (tmp/_tmp_t8_6e_protos3.log, exit 0, re-run this session) - a declaration
//compiled out by a guard would be undeclared and a mismatched signature would be C2440, so one run settles both
//questions. The SDK writes "RegisterSuspendResumeNotification (" and "UnregisterSuspendResumeNotification (" with a
//space before the paren, which is why the name scans in this folder allow \s* before the '('.

//Consumers: 0 LAYER and 0 BYPASS - 0 raw hits for all three names over the 3,228 .h/.hpp/.cpp/.cxx of src, examples and
//test (tmp/_tmp_t8_6e_consumers.ps1, re-run this session; the scan skips the user folder itself, so the one prose mention
//of the pair at devicemanagement.cpp:60 - which names both to say it deliberately does NOT wrap them there - is out of
//scope by construction, not by omission). CancelShutdown is wrapped here rather than in a new TU because
//this library already wraps ExitWindowsEx, its counterpart, and the header already has a //System Shutdwown functions
//section for it, so 2 of the 3 names get a TU of their own and 1 does not.

//Binding (D13): 2 qor_pp_useswinapi sites, both bare-stem, 2 qor_pp_fcontext, 2 Library::Call with explicit template
//arguments, 0 voidCall, 0 Win32ErrorHandler, 0 CheckReturn. No name here is a macro and none has an A/W family
//(tmp/_tmp_t8_6e_pre.ps1 prints aw_siblings empty for all three), so user32.h:9-17 gains no new #undef.

//Placement (U12): CancelShutdown joins the existing //System Shutdwown functions section at user32.h:700, between
//ExitWindowsEx and LockWorkStation, and the suspend/resume pair joins the existing //Power Management functions
//section at user32.h:786-787 after the power pair it belongs to - no new section is needed, and no stem in either family
//was already declared, so nothing pulls a bare declaration beside a wrapped sibling. The two inserts total 3 lines and
//move no user32.h self-citation: the header's only numeric citations are 4 comment lines at :610, :630, :651 and :653, all
//of them citing winuser.h / wingdi.h lines (14545, 6193, 15691, 2931, 3005, 3022, 3062, 3330, 3341) and all of them above
//both insert points, so no citation inside the header itself moves (tmp/_tmp_t8_6e_cites.ps1).

//Cross-TU citations (the T7 shift rule): the two inserts move 3 pre-existing citations in a closed TU. dpi.cpp:75 and
//:172 cite user32.h:712 and :260 cites :714, all three above the 1-line insert at user32.h:700, so each is stale by 1
//(712 -> :713, 714 -> :715). Closed TUs are write-once, so they stay stale by design; no citation predating this
//point at a line at or above 786, so the 2-line insert here shifts nothing (tmp/_tmp_t8_6e_cites2.ps1 over the 84
//user32.h citations the folder now carries - 78 of them predating this tranche, 6 written by this session's prose).
//
//Pairing (tmp/_tmp_t8_6e_pair2.ps1): the Power Management section holds 4 declarations at user32.h:784-:787 and this TU
//defines 2 of them, in header order, with 2 qor_pp_useswinapi / 2 qor_pp_fcontext / 2 Library::Call; the other 2 are
//RegisterPowerSettingNotification / UnregisterPowerSettingNotification, the T3 pair declared and defined nowhere, i.e.
//pre-existing U4 members this tranche neither creates nor removes. The System Shutdwown section in the other TU touched
//here pairs 6/6, and every Library::Call carries the return type plus one template argument per parameter.
//
//Gate: this TU standalone at /W4 exit 0 with 0 diagnostics (tmp/_tmp_t8_6e_single_pm3.log, and the edited systemshutdown
//TU at tmp/_tmp_t8_6e_single_sys3.log), then the folder 43/43 exit 0
//with 0 diagnostics (tmp/_tmp_t8_6e_layer3.log, compiled by _tmp_cl_all_u6e3.bat written by tmp/_tmp_t8_6e_genbat3.ps1 from
//the folder listing), all re-run after this banner and dated by tmp/obj/powermanagement.obj and tmp/obj/systemshutdown.obj
//being newer than their sources.
//cl /EP on this TU yields 2 binding literals, both against module literal "user32", each byte-equal to an export name and
//0 duplicates (digest tmp/_tmp_t8_6c_ppclass.ps1 over the ~453k-line captures tmp/_tmp_t8_6e_pp_powermanagement3.txt and
//tmp/_tmp_t8_6e_pp_systemshutdown.txt - both never opened, and both carrying every SDK header, so only the digest is read).
//Because user32.h changed, the all-libraries run was captured before and after and read as a DIFF rather than as an exit
//code: 110 -> 111 .cpp echoes, 304 -> 305 log lines, 166 diagnostic lines both sides (165 warnings + the one pre-existing
//fatal C1083 at opengl/wgl.cpp:31), exit 2 both sides, and a diagnostics set-difference of 0 new / 0 gone - the only content
//difference between the two logs is the added "powermanagement.cpp" echo (tmp/_tmp_t8_6e_apialldiff.ps1 over
//tmp/_tmp_t8_6e_pre_apiall.log and the post-banner leg tmp/_tmp_t8_6e_post_apiall2.log, digest tmp/_tmp_t8_6e_apiall_dig2.txt).
//The PRE leg's source list is the folder listing captured before the edit (tmp/_tmp_t8_6e_pregen.ps1 -> _tmp_u6_apiall_sources_pre6e.rsp,
//110, disk and git ls-files both 110 with powermanagement.cpp absent) and the POST list is regenerated from disk by the
//gate script itself (_tmp_u6_apiall_sources_post6e.rsp, 111) and name-set-checked against the PRE list: only_in_pre 0, only_in_post 1.

//Folder after this tranche: 43 sources / 604 members / 507 bindings, counted comment-stripped
//(tmp/_tmp_t8_6e_counts.ps1) because TU banners spell out the binding macros in prose. Coverage re-measure
//(tmp/_tmp_u18_coverage4.ps1 + tmp/_tmp_u18_buckets.ps1): 727 of the 1,018 export names covered, 38 declared-and-unwrapped
//raw, and the bucket instrument prints "0: Shutdown / power notification []" - the bucket is drained with nothing left
//unaccounted. The 38 remaining are legacy hook 4, legacy word / class-long 10 and Other 24.

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../library.h"

namespace qor { namespace win { namespace api {
    //Power-settings pair - the two declarations the header has held since the folder review; they are now static and
    //defined here. Both bind bare: no A/W spelling and no user32.h #undef for either stem.
    HPOWERNOTIFY User32::RegisterPowerSettingNotification(HANDLE hRecipient, LPCGUID PowerSettingGuid, DWORD Flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, RegisterPowerSettingNotification);
        return Library::Call< HPOWERNOTIFY >(pFunc, hRecipient, PowerSettingGuid, Flags);
    }

    //BOOL success flag: FALSE means the registration was not withdrawn. Handle is the HPOWERNOTIFY returned by the call above.
    BOOL User32::UnregisterPowerSettingNotification(HPOWERNOTIFY Handle)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UnregisterPowerSettingNotification);
        return Library::Call< BOOL >(pFunc, Handle);
    }

    //HPOWERNOTIFY, not a numeric: the SDK typedefs it PVOID at winuser.h:3985, so the answer is a handle or NULL and
    //NULL is not an error code - pass it through unchanged, and the U2 return-check idiom has nothing to add (U1/U2).
    //hRecipient must match Flags (DEVICE_NOTIFY_WINDOW_HANDLE 0x00000000 winuser.h:3946 for a window, DEVICE_NOTIFY_SERVICE_HANDLE
    //:3947 0x00000001 for a service); 0 is a legal Flags value, so the layer cannot spot a missing one and forwards both arguments
    //as the caller wrote them.
    HPOWERNOTIFY User32::RegisterSuspendResumeNotification(HANDLE hRecipient, DWORD Flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, RegisterSuspendResumeNotification);
        return Library::Call< HPOWERNOTIFY >(pFunc, hRecipient, Flags);
    }

    //BOOL, and FALSE means the notification was not withdrawn - a real success flag, not a Win32 error code, so it is
    //passed through and the caller reads it. Handle is the HPOWERNOTIFY the register call returned; anything else is the
    //caller's error, which this layer neither sees nor corrects.
    BOOL User32::UnregisterSuspendResumeNotification(HPOWERNOTIFY Handle)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UnregisterSuspendResumeNotification);
        return Library::Call< BOOL >(pFunc, Handle);
    }

}}}

