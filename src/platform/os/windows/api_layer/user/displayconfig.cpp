// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//=== api_layer conventions ========================================= Reviewed 6 Nov 2026
//   (T8 step 6, tranche 6d: the Display configuration family - 7 wrappers, new TU)
//
//This TU is coverage added under D18 (docs/agent/DECISIONS.md): purpose.md states the library intends a full set of
//wrappers over the user32.dll exports, and registers/u18-coverage.md buckets the gap by family. The Display config
//bucket measures exactly 7 and it reconciles with nothing shed: all 7 are exported from user32.dll AND declared in an
//SDK header, and the pre-scan (tmp/_tmp_t8_6d_pre.ps1 over the 48-name gap list written by tmp/_tmp_u18_coverage4.ps1)
//measures 0 function-like macros, 0 A/W siblings and 0 already declared in user32.h for every one of them: 7 in ->
//7 wrapped, 0 excluded, nothing shed. This is the first tranche of the backlog whose bucket filter needs no judgement
//call - the names are unambiguous, and unlike 6a there is no second family that re-matches the same name.
//
//Signature rule (D18): the published signature IS the winuser.h prototype, parameter names included - 7/7 copy it
//exactly. SDK declaration lines: 13203 GetDisplayConfigBufferSizes, 13211 SetDisplayConfig, 13221 QueryDisplayConfig,
//13234 DisplayConfigGetDeviceInfo, 13240 DisplayConfigSetDeviceInfo, 16063 GetDisplayAutoRotationPreferences,
//16077 SetDisplayAutoRotationPreferences. Export ordinals in the folder's capture: 1698, 1699, 1837, 1838, 2205,
//2320, 2321.
//
//The 6c-close pre-flag claim about the types is wrong and the correction changes where the family's guard story sits.
//The pre-flags said every DISPLAYCONFIG_* type is declared in winuser.h, citing :13213, :13215 and :13229 - those are
//the parameter USE sites inside the SetDisplayConfig and QueryDisplayConfig parameter lists, which is what a first-hit
//search finds. A definition scan over all 1,798 um + shared headers of SDK 10.0.26100.0 (tmp/_tmp_t8_6d_types.ps1)
//puts them in wingdi.h: DISPLAYCONFIG_MODE_INFO :2931, DISPLAYCONFIG_PATH_SOURCE_INFO :2950, PATH_TARGET_INFO :2973,
//PATH_INFO :3005, TOPOLOGY_ID :3022, DEVICE_INFO_TYPE :3053, DEVICE_INFO_HEADER :3062. And windisp.h - the header the
//online documentation names for this family - does not exist in this SDK at all: Test-Path False, and 0 um headers
//include it. The QDC_* and SDC_* flag groups the flags parameters take are in wingdi.h too (:3330-3335 and
//:3341-3360), not in winuser.h, which is why those two parameter lists cannot be read from winuser.h alone.
//
//Guarding: 5 of the 7 sit inside #ifdef _WINGDI_ ;; #ifndef NOGDI ;; #if(WINVER >= 0x0601) in winuser.h (the WINVER
//block opens at :13198 and closes at :13243, inside the enclosing NOGDI / _WINGDI_ pair that closes at :13246 / :13247),
//and the DISPLAYCONFIG_* types they take sit inside the same
//#ifndef NOGDI in wingdi.h - so NOGDI removes the types and the prototypes together, and user32.h declares these five
//unconditionally: a consumer that defines NOGDI would not compile this header. 0 in-tree sources define NOGDI
//(measured over src and cmake). The 2 rotation names sit only under #if(WINVER >= 0x0602) (winuser.h block closes :16083) and
//take ORIENTATION_PREFERENCE, which is a winuser.h enum at :16038 with DEFINE_ENUM_FLAG_OPERATORS at :16051. All 7
//additionally sit inside #if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP). WINVER defaults to 0x0A00
//(sdkddkver.h) and nothing in this build sets it, so all 7 are live and the layer adds no #if - but a caller that
//pins WINVER below 0x0602 loses the rotation pair, and one below 0x0601 loses the other five.
//
//Return conventions, kept as the SDK has them (D15 / D18 pass-through - 0 CheckReturn in this TU): 5 of the 7 return
//LONG, and that LONG is a Win32 error code, not a truthy value - QueryDisplayConfig is even annotated
//_Success_(return == ERROR_SUCCESS) (:13219). A CheckReturn-shaped reflex on these five would invert the meaning of
//every failure. The 2 rotation names return BOOL as a real success flag whose out-parameter side effect the layer
//neither sees nor validates.
//
//The buffer contracts are the part a caller can get wrong, so they are stated, not coded. QueryDisplayConfig's two
//count parameters are _Inout_ (:13223, :13225) - capacity in, entries-written out - over arrays the SDK annotates
//_Out_writes_to_(*count, *count) (:13224, :13226); GetDisplayConfigBufferSizes exists only to fill them (:13205-
//:13206, both _Out_), and its flags argument must match the one passed to the query. SetDisplayConfig's counts are
//plain _In_ (:13212, :13214) with the arrays _In_reads_opt_(count) (:13213, :13215), so null is legal there. The
//last QueryDisplayConfig parameter is conditioned on another: _When_(!(flags & QDC_DATABASE_CURRENT), _Pre_null_)
//_When_(flags & QDC_DATABASE_CURRENT, _Out_) (:13227-:13229), i.e. non-null with QDC_DATABASE_CURRENT and null
//without it are both ERROR_INVALID_PARAMETER - the caller's error, not one this layer can see. SetDisplayConfig is
//the only wrapper in this library that changes machine state: with SDC_APPLY and without SDC_SAVE_TO_DATABASE the
//change is undone at the next logon, and SDC_VALIDATE alone is the read-only path. The layer allocates nothing, sizes
//nothing and validates nothing on any of the three.
//
//Liveness and signatures were COMPILED, not eyeballed: tmp/_tmp_t8_6d_protos.cpp declares 7 function-pointer typedefs
//equal to the 7 published signatures and initialises each one with &<SDK name>. At /W4 with no QOR define and no
//version macro of its own it compiles clean (tmp/_tmp_t8_6d_protos.log, exit 0) - a declaration compiled out by a
//guard would be undeclared and a mismatched signature would be C2440, so that one run settles both questions.
//
//Binding (D13): 7 qor_pp_useswinapi sites, all bare-stem, 7 qor_pp_fcontext, 7 Library::Call with explicit template
//arguments, 0 voidCall, 0 Win32ErrorHandler (U1 stands folder-wide) and 0 mismatched stems in tmp/_tmp_wrong_stem_scan.ps1
//(42 files, 504 bindings scanned, only the 2 standing CopyCursorT / CreateWindowT ghosts). No name here is a macro and
//none has an A/W family, so user32.h:9-17 gains no new #undef.
//
//Consumers: 0 LAYER and 0 BYPASS - 0 raw hits for all 7 names over the 3,228 .h/.hpp/.cpp/.cxx of src, examples and
//test (tmp/_tmp_t8_6d_consumers.ps1), so there is no U19 split to book here; U19 is FROZEN at session 34 in any case.
//
//Placement (U12): a new section //Display configuration functions at user32.h:648-662, placed after //Touch and
//gesture functions and before //Font and Text functions - where the SDK has the block, and where no stem in this
//family was already declared, so nothing pulls a bare declaration beside a wrapped sibling. The 16-line insert moves
//9 later self-citations in user32.h down by 16 (console.cpp, devicemanagement.cpp x3, dpi.cpp x3, hook.cpp,
//rectangle.cpp); closed TUs are write-once, so those citations are stale by 16 by design, not repaired here - the
//T7 citation-shift rule.
//
//Gate: this TU standalone at /W4 exit 0, then the folder 42/42 exit 0 with no diagnostics, both re-run after this
//banner and dated by tmp/obj/displayconfig.obj being newer than the source. cl /EP on this TU yields 7 binding
//literals, all with module literal "user32", each byte-equal to an export name, 0 duplicates (digest
//tmp/_tmp_t8_6c_ppclass.ps1 over the post-banner capture tmp/_tmp_t8_6d_pp_displayconfig2.txt, which is never opened). Because
//user32.h changed, the all-libraries run was captured before and after and read as a DIFF rather than as an exit
//code: 109 -> 110 .cpp echoes, 165 warning lines both sides, the one pre-existing fatal C1083 at opengl/wgl.cpp:31
//both sides, exit 2 both sides, and a diagnostics set-difference of 0 new / 0 gone - the only content difference
//between the two logs is the added "displayconfig.cpp" echo. The PRE leg's source list is regenerated from git HEAD
//(tmp/_tmp_t8_6d_pregen.ps1) and name-set-checked against the PRE log, because the shared api_layer .rsp is rewritten
//for the POST leg mid-session.
//
//Folder after this tranche: 42 sources / 601 members / 504 bindings, counted comment-stripped
//(tmp/_tmp_t8_6d_counts.ps1) because TU banners spell out the binding macros in prose. Coverage re-measure
//(tmp/_tmp_u18_coverage4.ps1 + tmp/_tmp_u18_buckets.ps1): 724 of the 1,018 export names covered, 41 declared-and-
//unwrapped raw and the buckets instrument prints "0: Display config family []", i.e. the family is drained with
//nothing left unaccounted - the 41 are Shutdown / power 3, legacy hook 4, legacy word / class-long 10 and 24 Other.


//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../library.h"

namespace qor { namespace win { namespace api {
    //LONG, not BOOL: the return is a Win32 error code (ERROR_SUCCESS when the call worked), so it is passed through
    //unchanged - a CheckReturn-shaped reading would invert the meaning, since non-zero IS the answer (D15 / D18).
    //requestPacket is _Inout_ (winuser.h:13234-:13235): the caller fills the DISPLAYCONFIG_DEVICE_INFO_HEADER prefix - type /
    //size / adapterId / id (wingdi.h:3062-3067) - and that same block is the out buffer, so its size must describe
    //the whole derived packet (DISPLAYCONFIG_GET_DEVICE_INFO_HEADER and friends), which nothing here can validate.
    LONG User32::DisplayConfigGetDeviceInfo(DISPLAYCONFIG_DEVICE_INFO_HEADER* requestPacket)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DisplayConfigGetDeviceInfo);
        return Library::Call< LONG, DISPLAYCONFIG_DEVICE_INFO_HEADER* >(pFunc, requestPacket);
    }

    //The _In_ counterpart (winuser.h:13240): the packet is fully formed by the caller and nothing is written back, so this is
    //the one member of the pair that could take a const pointer - the SDK does not say const, so neither does the
    //published signature. Same LONG error-code rule as the Get above; the type / size / adapterId / id prefix is what
    //selects which device-info request is being made.
    LONG User32::DisplayConfigSetDeviceInfo(DISPLAYCONFIG_DEVICE_INFO_HEADER* setPacket)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DisplayConfigSetDeviceInfo);
        return Library::Call< LONG, DISPLAYCONFIG_DEVICE_INFO_HEADER* >(pFunc, setPacket);
    }

    //One of the two BOOL members of the family: this one really is a success flag. pOrientation is _Out_ and the value
    //written is an ORIENTATION_PREFERENCE (an enum, winuser.h:16038, not a DWORD - the ORIENTATION_PREFERENCE_* values
    //are 0x0/0x1/0x2/0x4/0x8 and are flag-shaped, so DEFINE_ENUM_FLAG_OPERATORS at :16051 lets them combine). WINVER
    //>= 0x0602 name, and unlike the five below it is NOT inside the NOGDI / _WINGDI_ region.
    BOOL User32::GetDisplayAutoRotationPreferences(ORIENTATION_PREFERENCE* pOrientation)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDisplayAutoRotationPreferences);
        return Library::Call< BOOL, ORIENTATION_PREFERENCE* >(pFunc, pOrientation);
    }

    //The sizing call the caller is expected to make first: both count parameters are _Out_ (:13205-:13206), i.e. this
    //wrapper only reports how many DISPLAYCONFIG_PATH_INFO / DISPLAYCONFIG_MODE_INFO entries the following
    //QueryDisplayConfig needs. flags is the QDC_* group (wingdi.h:3330-3335 - QDC_ALL_PATHS / QDC_ONLY_ACTIVE_PATHS /
    //QDC_DATABASE_CURRENT / QDC_VIRTUAL_MODE_AWARE / QDC_INCLUDE_HMD / QDC_VIRTUAL_REFRESH_RATE_AWARE, not
    //winuser.h). LONG return is ERROR_SUCCESS / ERROR_INVALID_PARAMETER, again passed through.
    LONG User32::GetDisplayConfigBufferSizes(UINT32 flags, UINT32* numPathArrayElements, UINT32* numModeInfoArrayElements)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDisplayConfigBufferSizes);
        return Library::Call< LONG, UINT32, UINT32*, UINT32* >(pFunc, flags, numPathArrayElements, numModeInfoArrayElements);
    }

    //The read call of the family. Both count parameters are _Inout_ (:13223, :13225), each array being
    //_Out_writes_to_(*count, *count) (:13224, :13226) - the count carries the capacity of its array IN and the number
    //of entries actually written OUT - which is exactly why GetDisplayConfigBufferSizes
    //exists: size the two arrays, then query, then re-read the counts. Too small a capacity is ERROR_INSUFFICIENT_-
    //BUFFER, and the counts are left at the required size. The last parameter is conditionally in/out by another
    //one: _When_(!(flags & QDC_DATABASE_CURRENT), _Pre_null_) _When_(flags & QDC_DATABASE_CURRENT, _Out_) (:13227-
    //:13229), so passing non-null with QDC_DATABASE_CURRENT set, or null without it, is ERROR_INVALID_PARAMETER -
    //the caller's error, stated here, not coded. Nothing here sizes or validates either array.
    LONG User32::QueryDisplayConfig(UINT32 flags, UINT32* numPathArrayElements, DISPLAYCONFIG_PATH_INFO* pathArray, UINT32* numModeInfoArrayElements, DISPLAYCONFIG_MODE_INFO* modeInfoArray, DISPLAYCONFIG_TOPOLOGY_ID* currentTopologyId)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, QueryDisplayConfig);
        return Library::Call< LONG, UINT32, UINT32*, DISPLAYCONFIG_PATH_INFO*, UINT32*, DISPLAYCONFIG_MODE_INFO*, DISPLAYCONFIG_TOPOLOGY_ID* >(pFunc, flags, numPathArrayElements, pathArray, numModeInfoArrayElements, modeInfoArray, currentTopologyId);
    }

    //The second BOOL member: a plain success flag. orientation is an ORIENTATION_PREFERENCE value (:16038) and this
    //is a per-process preference only - it does not reconfigure the display, unlike SetDisplayConfig below. WINVER
    //>= 0x0602 name, outside the NOGDI / _WINGDI_ region.
    BOOL User32::SetDisplayAutoRotationPreferences(ORIENTATION_PREFERENCE orientation)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetDisplayAutoRotationPreferences);
        return Library::Call< BOOL, ORIENTATION_PREFERENCE >(pFunc, orientation);
    }

    //The only wrapper in this library that changes machine state: it applies a display topology to the console
    //session, and with SDC_APPLY (wingdi.h:3350) but without SDC_SAVE_TO_DATABASE (:3352) the change is undone at the next
    //logon. Both arrays are _In_reads_opt_(count) (winuser.h:13213, :13215), so null is legal when the count is 0 or when
    //SDC_USE_DATABASE_CURRENT (:3346) is in flags - and the two counts are plain _In_ here, NOT the _Inout_ pair
    //QueryDisplayConfig has. flags is the SDC_* group (wingdi.h:3341-3360): the SDC_TOPOLOGY_* set, SDC_VALIDATE,
    //SDC_APPLY, SDC_SAVE_TO_DATABASE and the rest. SDC_VALIDATE alone is the read-only way to test a topology
    //without applying it, which matters because the wrapper cannot tell the caller which of the two it asked for.
    //LONG return: ERROR_SUCCESS, ERROR_INVALID_PARAMETER, ERROR_GEN_FAILURE or a device IoCtrl code.
    LONG User32::SetDisplayConfig(UINT32 numPathArrayElements, DISPLAYCONFIG_PATH_INFO* pathArray, UINT32 numModeInfoArrayElements, DISPLAYCONFIG_MODE_INFO* modeInfoArray, UINT32 flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetDisplayConfig);
        return Library::Call< LONG, UINT32, DISPLAYCONFIG_PATH_INFO*, UINT32, DISPLAYCONFIG_MODE_INFO*, UINT32 >(pFunc, numPathArrayElements, pathArray, numModeInfoArrayElements, modeInfoArray, flags);
    }

}}}//qor::win::api

