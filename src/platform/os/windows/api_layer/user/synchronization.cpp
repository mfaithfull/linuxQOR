// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//=== api_layer conventions ========================================= Added 9 Nov 2026
//   (T9 step 9e: the "//Synchronization functions" section declared 2 members non-static and no TU defined either of them - new TU)
//
//T9 (registers/tasks.md, D26) is the tranche queue for U4's remaining declared-but-undefined surface, and this TU lands
//step 9e's two halves together the way 9a's strings.cpp, 9b's rawinput.cpp, 9c's systeminfo.cpp and 9d's buttoncontrol.cpp
//did: it defines both members of user32.h's "//Synchronization functions" (user32.h:779-780) - MsgWaitForMultipleObjects
//MsgWaitForMultipleObjectsEx - and user32.h gains `static` on both declarations. U4's link gap (a caller of either member
//failed at LNK2019) and U18's non-static shape close together. Coverage stays 759 of 1,018: both were already declared, so
//the coverage instrument counted them and this step closes a link gap, never a coverage gap (the arithmetic D24 recorded for
//T6 and D27-D30 for 9a-9d). The section is 2/2 declared, 2/2 defined and 2/2 bound: the head is user32.h:778 and the next
//section "//Printing functions" is :782, so nothing sits outside this TU, and registers/u18-coverage.md names no
//Synchronization name as an exclusion, so this TU contributes no raw gap.
//
//One header edit beyond `static` is made here, and it moves no mangled name: the first member's third parameter is
//user32.h's `bWaitAll` where the SDK's own prototype at um\WinUser.h:6985 spells `fWaitAll`. D18's rule (a wrapper's
//published signature is the prototype of the export it binds, parameter names included) is what T8 executed across the
//folder - the 9c banner quotes the same rule for its 4 members - and U18's instrument paired DEFINED wrappers against the
//prototypes, so the two never-were-defined Sync members are the residual this step closes. Parameter names in a declaration
//are not part of a mangled name and there is no User32:: call site anywhere in the tree (see the consumers line), so the
//rename follows nothing; the all-libraries diff below measures that rather than assumes it.
//
//Binding spelling - D13 applied, and 2/2 take the bare form: 2 x qor_pp_useswinapi(user32, <literal>) and 0 x
//qor_pp_useswinapiAW. This is 9a's shape (34 bare / 0 AW), not 9b's or 9c's (6/1 and 3/1). The A/W half of D13 does not
//reach this section for the same two independent reasons it did not reach 9d's, both measured here: no stem macro switch -
//the probe over um\WinUser.h reads 0 "#define MsgWaitForMultipleObjects"/"#define MsgWaitForMultipleObjectsEx" lines, so
//G1's rewrite of the binding name before it is stringized does not fire and the literal the source spells is the literal
//GetProcAddress is given - and no A/W exports to bind: the standing table (tmp/_tmp_user32_exports.txt) and this session's
//fresh dumpbin dump (tmp/_tmp_t9_9e_exports_fresh.txt) both read the two rows bare, MsgWaitForMultipleObjects ordinal 2165
//hint 292 rva 00026090 and MsgWaitForMultipleObjectsEx ordinal 2166 hint 293 rva 00023010, with no A/W/T row for either.
//qor_pp_useswinapiAW is therefore not merely wrong here but unavailable: it pastes Stem##W at kernel32.h:77 and would bind
//two names user32.dll never exported (G2), caching a NULL pFunc for the process lifetime and throwing on every call (G5).
//Neither stem is #undef'd at user32.h:13-:21 either - that block lists RegisterDeviceNotification WinHelp SystemParametersInfo
//GetRawInputDeviceInfo DlgDirList DlgDirSelectEx DlgDirListComboBox DlgDirSelectComboBoxEx LoadBitmap and no
//Synchronization name. A live probe (tmp/_tmp_t9_9e_probe.cpp run by tmp/_tmp_t9_9e_probe.bat, output
//tmp/_tmp_t9_9e_probe.txt) resolves 2/2 bare at 00007FFF2CBF6090 / :CBF3010 over a user32.dll handle of 00007FFF2CBD0000 -
//each the handle plus the table's RVA - and reads MISSING for all six A/W spellings and both T spellings, while a
//deliberately bogus module name resolves to NULL, so the probe discriminates rather than reporting a vacuous hit.
//
//SDK availability, read at the declaration line in um\WinUser.h 10.0.26100.0 (16,475 lines): MsgWaitForMultipleObjects
//:6982, MsgWaitForMultipleObjectsEx :6992. Both sit at the same guard depth 3 - #ifndef _WINUSER_ (:13), #ifndef NOUSER
//(:64) and #if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP | WINAPI_PARTITION_PC_APP) (:6892) - and MS's own #endif
//comments in that stretch mislabel the pairing: by position the #endif at :6933 closes the #if(WINVER >= 0x0400) at :6912,
//the #endif at :6943 closes the "#pragma region Desktop Family" #if at :6937 and the #endif at :7000 closes :6892, so the
//live guard on these two is the DESKTOP | PC_APP one at :6892 rather than the DESKTOP-only region the comments suggest
//(_tmp_t9_9e_guards.ps1). NOUSER is defined nowhere in the tree (0 hits over the 3,566 .h/.cpp/.cmake/CMakeLists files under
//src/cmake/examples/test that _tmp_t9_9e_measure.ps1 searched) and nowhere in um\WinUser.h's own 16,475 lines, so D21's pin
//(kernel32.h:22-30, _WIN32_WINNT 0x0A00) is the live condition and user32.h adds no guard of its own.
//
//Signatures - D18's rule, measured against the SDK's own SAL-annotated prototypes at um\WinUser.h:6982-:6987 and
//:6992-:6997: apart from the bWaitAll/fWaitAll name above, both declarations already copy the prototype exactly, and the
//folder spells the SDK's CONST HANDLE * as const HANDLE* the same way 9c's SetSysColors spells CONST INT * as const INT*.
//The SAL annotation on pHandles is _In_reads_opt_(nCount), i.e. the array may be NULL when nCount is 0 - the one live
//caller in the tree (see the consumers line) passes exactly that shape, (0, NULL, FALSE, timeout_ms, QS_ALLINPUT).
//
//Return convention - G6's audit, done before choosing any wrapper type: um\WinUser.h carries no _Success_ and no
//_Check_return_ on either declaration (the scan over :6900-:6995 reads 0 such lines), so the compiler side is silent on
//the DWORD and the SDK claims nothing about it. The DWORD is a wait reason code, and 0 is a legal answer - a timeout is
//an answer, not a failure - which is exactly the test the session-27 soundness rule (registers/u2.md:14-:15, quoted by 9c at
//its :50-:53) applies: a checked-failure site is only sound where the failure value is not also a legal success value.
//The SDK's own wait-family spellings in um\WinBase.h:109-:115 keep the failure value at WAIT_FAILED 0xFFFFFFFF, i.e. not
//0, and the folder's sibling wait - processthread.cpp's WaitForInputIdle (:57-:59) - is deliberately unchecked for the
//same reason. So 0 Win32ErrorHandler and 0 CheckReturn in this TU, and every value comes back as the OS wrote it (U7).
//Adopting CheckReturn would also change run-time behaviour rather than mirror the SDK (G6: returncheck.h:19 tests
//qor_pp_parameter_checking == qor_pp_yes with neither macro defined anywhere in the repository, so the #if evaluates
//0 == 0 and CheckReturn<...>::TType is always the checked wrapper type) - it would raise a continuable issue on every
//timeout, which is an answer and not a failure.
//
//Consumers: the bypass scan over src/examples/test (tmp/_tmp_t9_9e_consumers.ps1, 3,227 files, every suffix variant of
//both stems fed at once) reads 3 hits for MsgWaitForMultipleObjects - user32.h:779/:780, the declarations themselves, and
//one BYPASS at src\platform\os\windows\ui\gl\platform.cpp:630 inside Platform::WaitEventsTimeout, which calls the SDK
//import ::MsgWaitForMultipleObjects and discards the reason code - and 1 hit for MsgWaitForMultipleObjectsEx, its own
//declaration. 0 hits for the T and the A/W spellings. So 0 LAYER calls exist and the static + rename edit follows no
//caller; the LAYER-vs-BYPASS split for U19 here is 0/1, and this step redirects nothing.
//
//Neighbours, measured so the section boundary is not assumed: um\WinUser.h's stretch that holds these two also declares
//GetInputState :6951, GetQueueStatus :6957, GetCapture :6964, SetCapture :6970 and ReleaseCapture :6976, all five
//exported (hint 165 / 1B4 / 125 / 320 / 2FE) and all five already declared static in user32.h (:299, :304, :563, :568,
//:567) and defined in message.cpp / mouse.cpp (tmp/_tmp_t9_9e_definers.ps1 over the 51 pre-edit sources). They belong to
//other user32.h sections and other TUs, so this TU's population is exactly the 2 members below.
//
//Census over this close (_tmp_static_census.ps1, run pre and post): user32.h keeps 634 member-declaration lines and stays
//855 lines, static 596 -> 598, non-static 38 -> 36, distinct defined member names / binding lines 595 -> 597 over 51 -> 52
//sources, with 0 defined-but-not-static and 0 defined-without-a-declaration both sides; _tmp_decl_no_def_repo_scan.ps1
//reads its declared-and-undefined count 4 -> 2, leaving exactly 9f's LoadBitmap (user32.h:776) and 9g's PrintWindow (:783),
//the 34 DDE ") const;" lines its regex misses by design being unchanged.
//
//Gate: synchronization.cpp standalone at /W4 exit 0, then the folder 52/52 at /W4 exit 0, cl /EP emitting the 2 literals
//above against the module literal L"user32", and - because user32.h moved - the all-libraries run as a diff against a
//pre-edit run (119 cpp echoes pre, 120 post, 166 diagnostic lines on both, 1 C1083 at opengl/wgl.cpp:31 on both, exit 2 on
//both), dated by cmd /c dir /T W rather than by pwsh (see the folder's gotchas note).

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //Wait for a message on any of nCount handles, or for dwMilliseconds to pass, or for one of the window events named
    //by dwWakeMask: fWaitAll asks for all the handles at once rather than any one, and the DWORD is the reason the wait
    //ended, not a success flag - so 0 is a legal answer and the layer hands it over as written (see the banner's G6
    //audit). pHandles is the caller's array of nCount handles; the SDK's own _In_reads_opt_(nCount) on it says the array
    //may be NULL when nCount is 0, which is the shape the one live caller in the tree passes. dwMilliseconds is
    //milliseconds - the caller converts its own seconds with (DWORD)(timeout * 1e3). dwWakeMask names one event in the
    //WM_* family the same header spells (:1983 WM_NULL, :1987 WM_SIZE, :2004 WM_PAINT, :2076 WM_WINDOWPOSCHANGED) or a
    //QS_* event-set macro (:7058 QS_ALLINPUT), and that caller passes QS_ALLINPUT.
    DWORD User32::MsgWaitForMultipleObjects(DWORD nCount, const HANDLE* pHandles, BOOL fWaitAll, DWORD dwMilliseconds, DWORD dwWakeMask)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, MsgWaitForMultipleObjects);
        return Library::Call< DWORD, DWORD, const HANDLE *, BOOL, DWORD, DWORD >(pFunc, nCount, pHandles, fWaitAll, dwMilliseconds, dwWakeMask);
    }

    //The Ex member of the same wait: the SDK's prototype at um\WinUser.h:6992-:6997 drops fWaitAll and adds dwFlags as
    //the fifth _In_ DWORD, so the caller states the wait's scope in that word rather than in an all-or-any flag - the
    //header declares no macro family for it (0 MSG_WAIT_* lines in um\WinUser.h), so nothing here names its values. The
    //DWORD is the same wait reason code as the base member's, 0 is a legal answer, and the export is bare (hint 293)
    //with no Ex A/W row; see the banner.
    DWORD User32::MsgWaitForMultipleObjectsEx(DWORD nCount, const HANDLE* pHandles, DWORD dwMilliseconds, DWORD dwWakeMask, DWORD dwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, MsgWaitForMultipleObjectsEx);
        return Library::Call< DWORD, DWORD, const HANDLE *, DWORD, DWORD, DWORD >(pFunc, nCount, pHandles, dwMilliseconds, dwWakeMask, dwFlags);
    }

}}}   //qor::win::api
