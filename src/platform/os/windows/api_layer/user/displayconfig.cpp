// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "../kernel/errorhandler.h"

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


//Addendum, 12 Nov 2026 (U2 Small TU batch S5, item displayconfig sites): the paragraph above is the review's record
//and stands superseded - the endpoint table reads all 7 rows of this TU checked, so this TU lands 7 CheckReturn sites
//(5 TSuccessCheck< LONG > over the LONG error-code rows and 2 BoolCheck over the rotation pair) and gained its one
//../returncheck.h include (0 -> 1). The decision is the table's, not re-derived here: those pages state "ERROR_SUCCESS
//The function succeeded" and then list only failure codes, so 0 is the reserved success answer and the test must read
//value == 0 - the pass-through the review describes is exactly the value TSuccessCheck leaves in place, and what the
//token changes is that a non-zero (documented failure) answer now raises a continuable issue the Win32ErrorHandler
//above reports instead of being handed over as an answer. The review's warning against a nonzero reflex on these five
//stays true - the landed class is the opposite polarity, not that one. Each row's verbatim docQuote and class sits at
//the site below. 0 in-tree callers for all 7 stems, so 0 call site needed re-reading before the tokens moved.
//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {
    //LONG, not BOOL: the return is a Win32 error code (ERROR_SUCCESS when the call worked), so it is passed through
    //unchanged - a CheckReturn-shaped reading would invert the meaning, since non-zero IS the answer (D15 / D18).
    //requestPacket is _Inout_ (winuser.h:13234-:13235): the caller fills the DISPLAYCONFIG_DEVICE_INFO_HEADER prefix - type /
    //size / adapterId / id (wingdi.h:3062-3067) - and that same block is the out buffer, so its size must describe
    //the whole derived packet (DISPLAYCONFIG_GET_DEVICE_INFO_HEADER and friends), which nothing here can validate.
    //Return policy (U2, table row DisplayConfigGetDeviceInfo / displayconfig.cpp, 12 Nov 2026): the row reads checked
    //with checkClass TSuccessCheck< LONG >, and the table's verbatim docQuote opens "The function returns one of the
    //following return codes. Return code Description ERROR_SUCCESS The function succeeded." while its last two entries
    //are "ERROR_INSUFFICIENT_BUFFER The size of the packet that the caller passes is not big enough for the information
    //that the caller requests." and "ERROR_GEN_FAILURE An unspecified error occurred." - every code the row names after
    //ERROR_SUCCESS is a documented failure, so the reserved value is ERROR_SUCCESS itself and the test reads value == 0
    //rather than value != 0; the pass-through the banner above describes is exactly the value TSuccessCheck leaves in
    //place, and what the token changes is that a nonzero (documented failure) answer now raises a continuable issue the
    //Win32ErrorHandler above reports instead of being handed over as an answer. Session-27 soundness holds at every
    //value: this stem answers no error code on success, so no legal answer is a reserved one. LONG takes TSuccessCheck
    //(returncheck.h:174, Test at :179 is (value == 0) ? true : false) already in the vocabulary - LongSuccessCheck is
    //routed away because its Test(intptr_t&) does not override the base Test(long&) on x64 - so no new check class.
    //0 in-tree callers over src, re-measured before this token moved.
    LONG User32::DisplayConfigGetDeviceInfo(DISPLAYCONFIG_DEVICE_INFO_HEADER* requestPacket)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DisplayConfigGetDeviceInfo);
        CheckReturn< LONG, TSuccessCheck< LONG > >::TType lResult = Library::Call< LONG, DISPLAYCONFIG_DEVICE_INFO_HEADER* >(pFunc, requestPacket);
        return lResult;
    }

    //The _In_ counterpart (winuser.h:13240): the packet is fully formed by the caller and nothing is written back, so this is
    //the one member of the pair that could take a const pointer - the SDK does not say const, so neither does the
    //published signature. Same LONG error-code rule as the Get above; the type / size / adapterId / id prefix is what
    //selects which device-info request is being made.
    //Return policy (U2, table row DisplayConfigSetDeviceInfo / displayconfig.cpp, 12 Nov 2026): the row reads checked
    //with checkClass TSuccessCheck< LONG > - the same return-code table as the Get twin above, whose verbatim docQuote
    //opens "The function returns one of the following return codes. Return code Description ERROR_SUCCESS The function
    //succeeded." and ends "ERROR_INSUFFICIENT_BUFFER The size of the packet that the caller passes is not big enough."
    //then "ERROR_GEN_FAILURE An unspecified error occurred." so ERROR_SUCCESS is the reserved success answer and every
    //code it names is a documented failure; the test reads value == 0, session-27 soundness holds at every value, and
    //LONG takes the TSuccessCheck template (returncheck.h:174, Test at :179 is (value == 0) ? true : false) that is
    //already in the vocabulary. 0 in-tree callers over src, re-measured before this token moved.
    LONG User32::DisplayConfigSetDeviceInfo(DISPLAYCONFIG_DEVICE_INFO_HEADER* setPacket)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DisplayConfigSetDeviceInfo);
        CheckReturn< LONG, TSuccessCheck< LONG > >::TType lResult = Library::Call< LONG, DISPLAYCONFIG_DEVICE_INFO_HEADER* >(pFunc, setPacket);
        return lResult;
    }

    //One of the two BOOL members of the family: this one really is a success flag. pOrientation is _Out_ and the value
    //written is an ORIENTATION_PREFERENCE (an enum, winuser.h:16038, not a DWORD - the ORIENTATION_PREFERENCE_* values
    //are 0x0/0x1/0x2/0x4/0x8 and are flag-shaped, so DEFINE_ENUM_FLAG_OPERATORS at :16051 lets them combine). WINVER
    //>= 0x0602 name, and unlike the five below it is NOT inside the NOGDI / _WINGDI_ region.
    //Return policy (U2, table row GetDisplayAutoRotationPreferences / displayconfig.cpp, 12 Nov 2026): the row reads
    //checked with checkClass BoolCheck, and the table's verbatim docQuote is the whole status sentence "TRUE if the
    //method succeeds, otherwise FALSE." - the preference itself leaves through pOrientation, so the returned word is
    //the call's own status and FALSE is reserved for failure, never a legal answer. That is exactly BoolCheck's test
    //(returncheck.h:94, Test at :99 is value ? true : false) the library already uses for every BOOL status row - no
    //new check class. 0 in-tree callers over src, re-measured before this token moved, so the continuable issue this
    //site can now raise reaches only callers outside the tree.
    BOOL User32::GetDisplayAutoRotationPreferences(ORIENTATION_PREFERENCE* pOrientation)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetDisplayAutoRotationPreferences);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, ORIENTATION_PREFERENCE* >(pFunc, pOrientation);
        return bResult;
    }

    //The sizing call the caller is expected to make first: both count parameters are _Out_ (:13205-:13206), i.e. this
    //wrapper only reports how many DISPLAYCONFIG_PATH_INFO / DISPLAYCONFIG_MODE_INFO entries the following
    //QueryDisplayConfig needs. flags is the QDC_* group (wingdi.h:3330-3335 - QDC_ALL_PATHS / QDC_ONLY_ACTIVE_PATHS /
    //QDC_DATABASE_CURRENT / QDC_VIRTUAL_MODE_AWARE / QDC_INCLUDE_HMD / QDC_VIRTUAL_REFRESH_RATE_AWARE, not
    //winuser.h). LONG return is ERROR_SUCCESS / ERROR_INVALID_PARAMETER, again passed through.
    //Return policy (U2, table row GetDisplayConfigBufferSizes / displayconfig.cpp, 12 Nov 2026): the row reads checked
    //with checkClass TSuccessCheck< LONG >. The table's verbatim docQuote opens "The function returns one of the
    //following return codes. Return code Description ERROR_SUCCESS The function succeeded." and its last entry is
    //"ERROR_GEN_FAILURE An unspecified error occurred." - both counts are _Out_ parameters, so this LONG carries
    //nothing but a code and ERROR_SUCCESS is the reserved success answer; the test reads value == 0 and the session-27
    //soundness test passes at every value since no success answer of this stem is an error code. LONG takes TSuccessCheck
    //(returncheck.h:174, Test at :179 is (value == 0) ? true : false), already in the vocabulary, LongSuccessCheck
    //routed away. 0 in-tree callers over src, re-measured before this token moved.
    LONG User32::GetDisplayConfigBufferSizes(UINT32 flags, UINT32* numPathArrayElements, UINT32* numModeInfoArrayElements)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetDisplayConfigBufferSizes);
        CheckReturn< LONG, TSuccessCheck< LONG > >::TType lResult = Library::Call< LONG, UINT32, UINT32*, UINT32* >(pFunc, flags, numPathArrayElements, numModeInfoArrayElements);
        return lResult;
    }

    //The read call of the family. Both count parameters are _Inout_ (:13223, :13225), each array being
    //_Out_writes_to_(*count, *count) (:13224, :13226) - the count carries the capacity of its array IN and the number
    //of entries actually written OUT - which is exactly why GetDisplayConfigBufferSizes
    //exists: size the two arrays, then query, then re-read the counts. Too small a capacity is ERROR_INSUFFICIENT_-
    //BUFFER, and the counts are left at the required size. The last parameter is conditionally in/out by another
    //one: _When_(!(flags & QDC_DATABASE_CURRENT), _Pre_null_) _When_(flags & QDC_DATABASE_CURRENT, _Out_) (:13227-
    //:13229), so passing non-null with QDC_DATABASE_CURRENT set, or null without it, is ERROR_INVALID_PARAMETER -
    //the caller's error, stated here, not coded. Nothing here sizes or validates either array.
    //Return policy (U2, table row QueryDisplayConfig / displayconfig.cpp, 12 Nov 2026): the row reads checked with
    //checkClass TSuccessCheck< LONG >, and the census carries no sdkRetType for this stem, so the class base is the
    //published LONG the wrapper holds - the spelling the token and the row agree on. The table's verbatim docQuote
    //opens "The function returns one of the following return codes. Return code Description ERROR_SUCCESS The function
    //succeeded." and ends "ERROR_GEN_FAILURE An unspecified error occurred. ERROR_INSUFFICIENT_BUFFER The supplied path
    //and mode buffer are too small." - the counts and the two arrays answer through parameters, so the LONG is only
    //ever a code, ERROR_SUCCESS is the reserved success answer and the test reads value == 0; session-27 soundness
    //holds at every value. TSuccessCheck (returncheck.h:174, Test at :179 is (value == 0) ? true : false) is already in
    //the vocabulary. 0 in-tree callers over src, re-measured before this token moved.
    LONG User32::QueryDisplayConfig(UINT32 flags, UINT32* numPathArrayElements, DISPLAYCONFIG_PATH_INFO* pathArray, UINT32* numModeInfoArrayElements, DISPLAYCONFIG_MODE_INFO* modeInfoArray, DISPLAYCONFIG_TOPOLOGY_ID* currentTopologyId)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, QueryDisplayConfig);
        CheckReturn< LONG, TSuccessCheck< LONG > >::TType lResult = Library::Call< LONG, UINT32, UINT32*, DISPLAYCONFIG_PATH_INFO*, UINT32*, DISPLAYCONFIG_MODE_INFO*, DISPLAYCONFIG_TOPOLOGY_ID* >(pFunc, flags, numPathArrayElements, pathArray, numModeInfoArrayElements, modeInfoArray, currentTopologyId);
        return lResult;
    }

    //The second BOOL member: a plain success flag. orientation is an ORIENTATION_PREFERENCE value (:16038) and this
    //is a per-process preference only - it does not reconfigure the display, unlike SetDisplayConfig below. WINVER
    //>= 0x0602 name, outside the NOGDI / _WINGDI_ region.
    //Return policy (U2, table row SetDisplayAutoRotationPreferences / displayconfig.cpp, 12 Nov 2026): the row reads
    //checked with checkClass BoolCheck, and the table's verbatim docQuote is "TRUE if the method succeeds, otherwise
    //FALSE. See GetDisplayAutoRotationPreferences for an example of using this function." - the orientation is an
    //argument and nothing answers through the return, so the returned word is pure call status and FALSE is reserved
    //for failure. BoolCheck (returncheck.h:94, Test at :99 is value ? true : false) is the class the library already
    //gives every BOOL status row, so no new check class, and 0 in-tree callers over src reach this stem, re-measured
    //before this token moved.
    BOOL User32::SetDisplayAutoRotationPreferences(ORIENTATION_PREFERENCE orientation)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetDisplayAutoRotationPreferences);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, ORIENTATION_PREFERENCE >(pFunc, orientation);
        return bResult;
    }

    //The only wrapper in this library that changes machine state: it applies a display topology to the console
    //session, and with SDC_APPLY (wingdi.h:3350) but without SDC_SAVE_TO_DATABASE (:3352) the change is undone at the next
    //logon. Both arrays are _In_reads_opt_(count) (winuser.h:13213, :13215), so null is legal when the count is 0 or when
    //SDC_USE_DATABASE_CURRENT (:3346) is in flags - and the two counts are plain _In_ here, NOT the _Inout_ pair
    //QueryDisplayConfig has. flags is the SDC_* group (wingdi.h:3341-3360): the SDC_TOPOLOGY_* set, SDC_VALIDATE,
    //SDC_APPLY, SDC_SAVE_TO_DATABASE and the rest. SDC_VALIDATE alone is the read-only way to test a topology
    //without applying it, which matters because the wrapper cannot tell the caller which of the two it asked for.
    //LONG return: ERROR_SUCCESS, ERROR_INVALID_PARAMETER, ERROR_GEN_FAILURE or a device IoCtrl code.
    //Return policy (U2, table row SetDisplayConfig / displayconfig.cpp, 12 Nov 2026): the row reads checked with
    //checkClass TSuccessCheck< LONG >. The table's verbatim docQuote opens "The function returns one of the following
    //return codes. Return code Description ERROR_SUCCESS The function succeeded." and its last entry is "ERROR_BAD_
    //CONFIGURATION The function could not find a workable solution for the source and target modes that the caller did
    //not specify." after "ERROR_GEN_FAILURE An unspecified error occurred." - every parameter here is an input, so the
    //LONG answers with a code only and ERROR_SUCCESS is the reserved success answer; the test reads value == 0 and
    //session-27 soundness holds at every value. TSuccessCheck (returncheck.h:174, Test at :179 is (value == 0) ? true :
    //false) is already in the vocabulary, LongSuccessCheck routed away, and 0 in-tree callers over src reach this stem.
    LONG User32::SetDisplayConfig(UINT32 numPathArrayElements, DISPLAYCONFIG_PATH_INFO* pathArray, UINT32 numModeInfoArrayElements, DISPLAYCONFIG_MODE_INFO* modeInfoArray, UINT32 flags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetDisplayConfig);
        CheckReturn< LONG, TSuccessCheck< LONG > >::TType lResult = Library::Call< LONG, UINT32, DISPLAYCONFIG_PATH_INFO*, UINT32, DISPLAYCONFIG_MODE_INFO*, UINT32 >(pFunc, numPathArrayElements, pathArray, numModeInfoArrayElements, modeInfoArray, flags);
        return lResult;
    }

}}}//qor::win::api

