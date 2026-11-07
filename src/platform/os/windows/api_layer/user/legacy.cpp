// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../library.h"
//=== api_layer conventions ========================================= Reviewed 7 Nov 2026
//   (T8 step 6, tranche 6g: the held-open legacy bucket - 6 names / 5 members, new TU)
//
//This TU is the last tranche of the D18 coverage work (docs/agent/DECISIONS.md): purpose.md states the library intends a
//full set of wrappers over the user32.dll exports, registers/u18-coverage.md buckets the gap by family, and D19 decided
//the six held-open legacy names IN scope. The bucket is derived, not typed: tmp/_tmp_t8_6g_pre.ps1 filters the gap list
//written by tmp/_tmp_u18_coverage4.ps1 for everything outside the superseded families and prints
//held_open_count=6 bucket=DragObject,GetWindowWord,SetMessageQueue,SetWindowWord,wsprintfA,wsprintfW. All 6 are
//exported from user32.dll (ordinals CC/0002EC40, 1FB/00086890, 351/0008EF10, 388/0002FD20, 3F6/000869B0,
//3F7/000869E0 in tmp/_tmp_user32_exports.txt) AND declared in an SDK header, the pre-scan measures 0 function-like
//macros and 0 rows already in user32.h for every one of them, and none has a wrapped replacement (the instrument's
//replacement-stem-in-layer row is NONE for all 6). 6 in -> 5 members, 0 excluded: wsprintfA/W is the folder's last
//A/W pair, so under the T-suffix convention (D13, model windowclass.cpp GetClassLongPtrT / qor_pp_useswinapiAW) it is
//ONE wsprintfT member over qor_pp_useswinapiAW, and configuration.h:13 sets qor_pp_unicode 1, so this build binds
//wsprintfW (3F7) and wsprintfA (3F6) stays unresolved at run time - the same shape every other AW stem here has.
//
//Signature rule (D18): the published signature IS the winuser.h prototype, parameter names included. SDK declaration
//lines (WinUser.h, um/10.0.26100.0): wsprintfA :326, wsprintfW :333, SetMessageQueue :3500, DragObject :8145,
//GetWindowWord :9837, SetWindowWord :9844. 4/4 definitions here copy that prototype exactly; wsprintfT's two parameter
//names are the layer's own because the SDK gives the wsprintfA/W parameters NO names at all (winuser.h:326 writes
//"_Out_ LPSTR, _In_ _Printf_format_string_ LPCSTR, ..." - the same nameless-parameter shape as the six members recorded
//under T8 step 4, and there is nothing to align to). The members sit in a new //Legacy functions section at user32.h:821
//- wsprintfT declared and defined at :835-:840, the four ordinary declarations at :842-:845, in legacy.cpp's own
//definition order and not the SDK's (SetMessageQueue :3500 precedes DragObject :8145 in WinUser.h), and the WindowWord
//SetWindowLongT (:732) - GetWindowWord/SetWindowWord are those members' 16-bit siblings, already wrapped for the class
//form at GetClassWord (:726) / SetClassWord (:731).
//
//Guards, forward pass over the declaring header at the declaration line itself (tmp/_tmp_t8_6g_pre.ps1 - never a
//backward pop, and \s* before every paren): every name sits inside #ifndef NOUSER and the DESKTOP WINAPI_FAMILY
//partition; DragObject additionally behind #if(WINVER >= 0x0400) (opened :8107, closed :8162) and the WindowWord pair
//inside #ifndef NOWINOFFSETS (:9832-:9849). The build pins none of that: 0 files under src, cmake, examples and test
//define WINVER, _WIN32_WINNT, NTDDI_VERSION, NOUSER, NOMSG or NOWINOFFSETS, and sdkddkver.h:228 sets _WIN32_WINNT
//0x0A00 with :249 WINVER _WIN32_WINNT, so all 6 are live and the layer adds no #if of its own.
//
//Calling convention: wsprintfA/W is the ONLY WINAPIV export this folder wraps, and it is not a typo for WINAPI -
//minwindef.h:128 spells WINAPI as __stdcall while :129 spells WINAPIV as __cdecl, because a variadic callee cannot
//clean a stack of unknown width. Library::Call (library.h) synthesizes a __stdcall fPtr, which is the fix behind open
//item U6 and the wrong convention for a WINAPIV export on x86, so the pair goes through the new Library::varCall
//(library.h, DECISIONS.md D20) whose fPtr is __cdecl AND variadic: ret(__cdecl *fPtr)(MethodArgs..., ...). This build
//is x64-only (D17), where the two conventions are one, so the difference is invisible in this tree - the spelling is
//what a 32-bit build would need. The tail is undeclared BY THE SDK, which is why no fixed-arity fPtr can state the call:
//wsprintfA/W write '...' rather than a parameter list, so the fixed head is spelled and the tail is a pack.
//
//wsprintfT is the folder's ONE header-inline member (DECISIONS.md D20), and that is a measured decision, not a style
//choice: a member template is instantiated only where its body is visible, so declaring it in user32.h and defining the
//body in this TU leaves every consumer with unresolved symbols. tmp/_tmp_6g_member_h3.h + _tmp_6g_member_d.cpp +
//_tmp_6g_member_e.cpp measure that out-of-line shape as LNK2019 per tail shape and LNK1120 on the link, while
//tmp/_tmp_6g_varcall_probe3.cpp compiles the header-inline shape at /W4 exit 0 and runs 0-, 1- and 2-argument tails
//through a sentinel-terminated stand-in callee, printing 0/3/6 - the pack reaches the callee at every arity. Hence this
//TU carries 4 definitions and wsprintfT's body sits in user32.h beside its declaration. user32.h:11 gains the ../library.h
//include the body needs - it is the only header in this folder that includes it, and every other wrapper reaches
//Library::Call from the TU's own include line, which is also the order this TU uses. Two consequences are recorded
//rather than hidden: the binding static is a function-local static in a template, so it is one pFunc per instantiation,
//and the body carries no qor_pp_fcontext because user32.h does not include the functioncontext flyer - pulling a generic
//flyer into the published header is a folder-wide change, not a TU decision.
//
//Return conventions: 4 value-returning definitions plus the header-inline int. DragObject returns DWORD with no
//_Check_return_ on its SDK declaration and no documented GetLastError convention; SetMessageQueue returns BOOL as a
//real success flag, where FALSE says the queue was not resized, and it is the only member of this bucket whose return
//a caller has to read; GetWindowWord and SetWindowWord return WORD as the value of the slot read or written, not a
//flag; wsprintfT returns the int character count, where 0 is a legal answer. None carries _Check_return_ and none has
//a documented -1-or-GetLastError convention, so this TU adds 0 Win32ErrorHandler and 0 CheckReturn sites - U1 and U2
//stay open on this bucket, and this TU takes none of ../returncheck.h, which is why it does not include it.
//
//Consumers: tmp/_tmp_t8_6g_consumers.ps1 runs the U19 instrument over all EIGHT stem spellings - bare, A, W and T for
//each of the names, because the scan matches stem\s*\( and a bare-stem run hides every T-suffixed layer call and every
//bypass that calls the W export directly - and measures 0 LAYER, 0 INDIRECT, 0 BYPASS and 0 MEMBER over src, examples
//and test. No in-tree caller exists for any of the 6 names, so there is no consumer fix and no U19 split to book; the
//surface this TU adds is latent, which is also why wsprintfT's header-inline shape cannot break an existing caller.
//
//Gate: legacy.cpp standalone at /W4 exit 0 (tmp/_tmp_6g_legacy_one.bat, whose exit file reads 0), and the folder at 45/45. library.h changed,
//so the all-libraries run is read as a PRE/POST DIFF and never as an exit code: 112 -> 113 .cpp echoes, 306 -> 307 log
//lines, 166 diagnostic lines on both sides (165 warnings plus the one pre-existing fatal C1083 at opengl/wgl.cpp:31),
//exit 2 on both sides, and a diagnostics set-difference of 0 new / 0 gone - the only content difference between the two
//logs is the added legacy.cpp echo (tmp/_tmp_t8_6g_apiall_mk.ps1 + tmp/_tmp_t8_6g_apialldiff.ps1, clones of the 6f pair).
//The PRE list is the folder listing captured before the TU existed and the POST list is regenerated from disk by the
//marker script itself, then name-set checked against the PRE list: only_in_pre 0, only_in_post 1. Dating: the POST run
//compiled at 17:11 and this banner was edited after it, so the gate was re-run live at this close AFTER that last edit -
//legacy.cpp standalone at /W4 exit 0 and the folder gate over all 45 sources (tmp/_tmp_t8_6g_folder_gate.bat, generated
//by tmp/_tmp_t8_6g_folder_mk.ps1 from the CMake source list): 45 .cpp echoes, 0 diagnostics - the one warning/error
//grep hit is the filename errorhandling.cpp - exit 0, every tmp/obj .obj newer than its source, and the /EP capture and
//its digest re-run after the same edit, so the numbers quoted above and below date from that final pass.
//
//Binding literals: cl /EP over this TU (tmp/_tmp_t8_6g_pp_legacy.bat, digest tmp/_tmp_t8_6g_ppclass.ps1), re-run after
//that last banner edit, yields 5 binding literals, all against the module literal "user32", each byte-equal to an export
//name and 0 duplicates - DragObject, SetMessageQueue, GetWindowWord, SetWindowWord and wsprintfW. Four come from the
//definitions here and the fifth from the header-inline wsprintfT body that user32.h brings in, which is the only place
//in the build where the wsprintfW name is requested. The capture carries every SDK header, so only the digest is read.
//
//Folder after this tranche: 45 sources / 632 members / 535 bindings, counted comment-stripped (tmp/_tmp_t8_6g_counts.ps1
//re-run, since TU banners spell the binding macros out in prose - it reads 45 .cpp files, 534 TU binding lines and 534 TU
//User32:: definitions over the TUs, plus the 1 binding and the 1 member that live in user32.h beside the wsprintfT
//declaration. That instrument reads 633 header member declarations, one above the 632 real members: the header-inline
//body's qor_pp_useswinapiAW(user32, wsprintf) line matches its declaration pattern - an instrument count, not a member.
//Coverage re-measure (tmp/_tmp_u18_coverage4.ps1 THEN tmp/_tmp_u18_buckets.ps1 - the buckets script reads the gap file
//the coverage script writes, so the two are run in sequence and never in parallel; an earlier parallel pair at this close
//read the previous gap and was re-run that way before being quoted).
//757 of the 1,018 export names match a member stem after A/W + T normalization (751 before the tranche), 8 are
//declared-and-unwrapped and 253 have no prototype. The bucket instrument prints 0 for every family except the two that
//are all exclusions: "4: Legacy hook family [SetWindowsHookA, SetWindowsHookExAW, SetWindowsHookW, UnhookWindowsHook]" and
//"4: Legacy word / class-long family [GetClassLongA, GetClassLongW, SetClassLongA, SetClassLongW]" - the word/class-long
//row went 10 -> 4 and Other is 0, so the 6 legacy names are out of the gap and the 8 that remain all carry written
//exclusions: the 7 superseded twins whose *Ptr/Ex replacement this library wraps, and the artifact SetWindowsHookExAW,
//which no header declares. U18's close condition - every one of the 109 backlog names wrapped or excluded - is met.

namespace qor { namespace win { namespace api {

    //The handle contract is the part a caller can get wrong, so it is stated, not coded. hcur is _In_opt_ - the SDK
    //documents a legal NULL cursor - and the layer forwards it unchecked exactly as the standing U1/U2 policy has it.
    //hwndParent and hwndFrom are two window handles and the OS requires the second to be a descendant of the first; fmt
    //selects how the bytes behind data are interpreted. None of that is visible to a wrapper, so all five arguments are
    //forwarded as the caller wrote them and the DWORD answer is passed through unchanged.
    DWORD User32::DragObject(HWND hwndParent, HWND hwndFrom, UINT fmt, ULONG_PTR data, HCURSOR hcur)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DragObject);
        return Library::Call< DWORD, HWND, HWND, UINT, ULONG_PTR, HCURSOR >(pFunc, hwndParent, hwndFrom, fmt, data, hcur);
    }

    //BOOL as a real success flag - FALSE says the queue was not resized, and reading the return is the only way a caller
    //finds that out. cMessagesMax is the requested capacity; the OS caps it against its own limit, and the layer neither
    //sees nor corrects that. This is the one member of the bucket whose return is a flag.
    BOOL User32::SetMessageQueue(int cMessagesMax)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetMessageQueue);
        return Library::Call< BOOL, int >(pFunc, cMessagesMax);
    }

    //WORD, not a flag: the current value of the nIndex-th 16-bit slot in the window's extra-bytes record - the same
    //nIndex-into-that-record contract as the wrapped GetWindowLongT (user32.h:727) and the already-wrapped WORD class
    //sibling GetClassWord (:726). nIndex is the caller's index and the layer cannot see whether it is in range.
    WORD User32::GetWindowWord(HWND hWnd, int nIndex)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetWindowWord);
        return Library::Call< WORD, HWND, int >(pFunc, hWnd, nIndex);
    }

    //WORD again, and this time the value returned is the one put in place. wNewWord is a plain _In_ WORD, so a wider
    //caller value is already truncated by the caller, not by the layer. Twin of SetWindowLongT (user32.h:732) and of
    //SetClassWord (:731).
    WORD User32::SetWindowWord(HWND hWnd, int nIndex, WORD wNewWord)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetWindowWord);
        return Library::Call< WORD, HWND, int, WORD >(pFunc, hWnd, nIndex, wNewWord);
    }

}}}

