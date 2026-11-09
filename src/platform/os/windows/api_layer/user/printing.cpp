// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//=== api_layer conventions ========================================= Added 9 Nov 2026
//   (T9 step 9g: the "//Printing functions" section declared 1 member non-static and no TU defined it - new TU)
//
//T9 (registers/tasks.md, D26) is the tranche queue for U4's remaining declared-but-undefined surface, and this TU lands
//step 9g's two halves together the way 9a's strings.cpp, 9b's rawinput.cpp, 9c's systeminfo.cpp, 9d's buttoncontrol.cpp,
//9e's synchronization.cpp and 9f's bitmap.cpp did: it defines the one member of user32.h's "//Printing functions" section
//(user32.h:783 - PrintWindow) and user32.h gains static on that one declaration. U4's link gap (a caller of PrintWindow
//failed at LNK2019, not at compile time) and U18's non-static half of the same shape close together here. Coverage stays
//759 of 1,018 - the member was already declared, so the coverage instrument already counted it; this closes a LINK gap,
//never a coverage gap, the arithmetic D24 recorded for T6 and D27-D32 for 9a-9f. The section is 1/1 declared, 1/1 defined
//and 1/1 bound: the head is user32.h:782 and the next section "//Device Management functions" is :785, so no member and no
//blank sits outside this TU, and registers/u18-coverage.md names no Printing name as an exclusion (0 hits for the stem in
//that file), so this TU contributes no raw gap.
//
//Binding: 1 x BARE qor_pp_useswinapi(user32, PrintWindow) and 0 x qor_pp_useswinapiAW - 9a's (34/0), 9d's (3/0) and 9e's
//(2/0) all-bare shape, the INVERSE of 9f's (1 AW / 0 bare), whose stem had the opposite measurement. The AW form is
//unavailable here, not merely wrong, and three measurements say so. (1) The export row IS the bare stem: this session's
//fresh dumpbin /exports C:\Windows\System32\user32.dll dump (tmp/_tmp_t9_9g_exports_fresh.txt, 1,198 rows) reads the row
//PrintWindow ordinal 2197 hint 2B2 rva 0002F620 and NO_ROW for PrintWindowA, PrintWindowW, PrintWindowEx, PrintWindowExA,
//PrintWindowExW and PrintWindowT; the standing table (tmp/_tmp_user32_exports.txt) agrees exactly, so qor_pp_useswinapiAW
//would paste (Stem ## W at kernel32.h:77) a name user32.dll never exported. (2) There is no stem switch to lean on: the
//switch probe over um\WinUser.h 10.0.26100.0 (16,475 lines) reads 0 "#define PrintWindow" lines, so G1's rewrite of the
//binding name before it is stringized never fires and the literal GetProcAddress is given is the one the source spells.
//(3) The stem is NOT one of the nine #undefs at user32.h:13-:21 (that block lists RegisterDeviceNotification WinHelp
//SystemParametersInfo GetRawInputDeviceInfo DlgDirList DlgDirSelectEx DlgDirListComboBox DlgDirSelectComboBoxEx LoadBitmap
//- tmp/_tmp_t9_9g_measure.ps1 reads undef_PrintWindow=0), so unlike 9f's LoadBitmap the SDK's own spelling survives here.
//The live probe (tmp/_tmp_t9_9g_probe.cpp) resolves PrintWindow RESOLVED 00007FFF2CBFF620 over a user32.dll handle of
//00007FFF2CBD0000 - the handle plus the table's rva 0002F620 - and MISSING for every other spelling, while a deliberately
//bogus module name resolves to NULL, so the probe discriminates rather than reporting a vacuous hit. The stem also stays
//bare for U17: it is a single export with no A/W family, so the T-suffix policy is satisfied as declared and nothing is
//renamed (the session-11 defer policy is unchanged).
//
//Signature (D18, docs/agent/DECISIONS.md): the published signature IS the prototype of the export it binds, parameter names
//included, and it is satisfied AS DECLARED - the header already spells the SDK's own names hwnd / hdcBlt / nFlags, so the
//header edit for this step is the single word static and no mangled name moves. um\WinUser.h 10.0.26100.0 declares PrintWindow
//at :4617, verbatim WINUSERAPI / BOOL / WINAPI / PrintWindow( _In_ HWND hwnd, _In_ HDC hdcBlt, _In_ UINT nFlags ); at GUARD
//DEPTH 5: #ifndef _WINUSER_ :13, #ifndef NOUSER :64, #if(_WIN32_WINNT >= 0x0500) :4540, #if(_WIN32_WINNT >= 0x0501) :4593
//and #if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) :4596 (tmp/_tmp_t9_9g_guards.ps1 replays the nesting; the region's
//own declarations are UpdateLayeredWindowIndirect :4584, GetLayeredWindowAttributes :4601, PrintWindow :4617, SetLayeredWindow
//Attributes :4633, ShowWindowAsync :4675). kernel32.h:22-30 pins _WIN32_WINNT / WINVER at 0x0A00 (D21), so the prototype is
//live in this chain, and NOUSER / NOGDI / NOPRINT are defined nowhere in the tree (0 hits over the 3,568 files tmp/_tmp_t9_
//9g_measure.ps1 searched). Unlike 9f's block this one sits under TWO version tests, so the floor is measured rather than
//asserted: tmp/_tmp_t9_9g_protos_neg.cpp bypasses kernel32.h and pins 0x0500, and taking &PrintWindow then fails with 1 C2065
//'PrintWindow' undeclared at exit 2 - the U16 half the D21 pin already covers. The positive leg tmp/_tmp_t9_9g_protos.cpp
//compiles at exit 0 with a single echo: two function pointers typed exactly as user32.h:783 spells the member and exactly as
//the export spells it both initialise from &PrintWindow, so liveness and signature are settled by the compiler, and tmp/_tmp_
//t9_9g_protos_neg3.cpp (a DWORD-shaped third parameter, unsigned long on x64 against the export's UINT) is REJECTED with 1
//C2440 at exit 2 whose text prints the export's own shape BOOL (HWND,HDC,UINT) - the agreement is load-bearing, not vacuous.
//tmp/_tmp_t9_9g_protos_neg2.cpp (&PrintWindowW) fails with 1 C2065 at exit 2: the AW spelling names something no SDK header
//declares, which is the compile-side twin of the NO_ROW export rows above.
//
//Return convention (the G6 audit this step owes fresh, not inherited from 9f): the SDK carries exactly the three _In_
//annotations on the declaration and NO _Success_ and no _Check_return_ - the region scan reads 3 _Out_opt_ lines, all three
//of them at :4603-:4605 of the PRECEDING GetLayeredWindowAttributes declaration, and 0 _Success_ / _Check_return_ anywhere in
//the window. BOOL here is nevertheless a genuine success flag, unlike 9f's HBITMAP handle where NULL folds onto a legitimate
//empty handle, so the wrapper-type question had to be re-read here. The standing U1/U2 policy still adds no CheckReturn and
//no Win32ErrorHandler: adopting CheckReturn would raise a continuable issue on every FALSE (G6 reads return checking as
//silently always on), and the layer passes the flag through for the caller to read - the same shape security.cpp's two BOOL
//user-object wrappers and listbox.cpp's DlgDirSelectEx stand under.
//
//Liveness (U15/U19): 0 LAYER / 0 BYPASS. The comment-stripped scan over src / examples / test (tmp/_tmp_t9_9g_consumers.ps1,
//3,229 files, EVERY suffix variant of the stem at once - PrintWindow PrintWindowT PrintWindowA PrintWindowW PrintWindowEx
//PrintWindowExA PrintWindowExW) reads 1 hit and 0 call sites: the user32.h:783 declaration itself. A raw grep reads 4, and
//each extra is measured out - src/platform/os/windows/ui/gui/controllers/windowcontroller.h:291 spells a same-named
//WindowController member, but tmp/_tmp_comment_state.ps1 reads that line COMMENT: it sits inside the /* */ at :290-:292,
//so it is commented-out code (that member has 0 definitions and 0 call sites in src); the other two are banner prose in
//bitmap.cpp:95 and synchronization.cpp:92. So the static edit redirects nothing (U19's split for 9g is 0/0) and D14's
//exported-and-unused shape is not a finding.
//
//Census moves over this step (tmp/_tmp_static_census.ps1, pre tmp/_tmp_t9_9g_census_pre.txt vs post
//tmp/_tmp_t9_9g_census_post.txt): user32.h keeps 634 member-declaration lines and stays 855 lines - the static went onto
//a declaration line only - static 599 -> 600, non-static 35 -> 34 (the "//Dynamic Data Exchange functions" block being
//the only non-static group left), distinct defined member names / binding lines 598 -> 599 over 53 -> 54 sources, with
//0 defined-but-not-static and 0 defined-without-a-declaration on both sides. tmp/_tmp_decl_no_def_repo_scan.ps1 reads
//its declared-and-undefined count 1 -> 0: the pre-edit run printed exactly user32.h:783 PrintWindow, which is now gone,
//and the 34 DDE ") const;" declaration lines are that scanner's known blind spot (its regex anchors on );), not part of
//either count. Coverage stays 759 of 1,018.
//
//Gate: printing.cpp standalone at /W4 exit 0 with printing.cpp echoed once and 0 diagnostics, dated by tmp/obj/printing.obj
//being newer than the source; then the whole folder 54/54 at /W4 exit 0, with printing.cpp echoed once among the 54 and 0
//diagnostics (tmp/_tmp_t9_9g_folder_gate.log: 54 cpp echoes, 0 warning/error lines, exit 0). cl /EP on this TU exit 0,
//emitting this TU's one binding literal "PrintWindow" against the module literal L"user32" (the other GetProcAddress
//literal in that output, "wsprintfW", is the header-inline wsprintfT body in user32.h, not a binding of this TU), 1 bare
//qor_pp_useswinapi / 0 AW / 1 Library::Call< BOOL, HWND, HDC, UINT > / 1 qor_pp_fcontext / 1 definition / 0 other call forms,
//and the preprocessed SDK prototype this TU sees printed as "__declspec(dllimport) BOOL __stdcall PrintWindow( HWND hwnd,
//HDC hdcBlt, UINT nFlags);" - so liveness and signature are read in the same run as the binding (tmp/_tmp_t9_9g_epcheck.ps1).
//The all-libraries leg is a diff, not an exit code: the standing 9f post log (121 cpp echoes, 166 diagnostics, exit 2)
//against the 9g pre re-run of the same 121-token rsp AFTER the user32.h:783 static edit landed (121 echoes, 166, exit 2)
//and the 9g post run of the 122-token rsp (122 echoes, 166, exit 2) - 0 diagnostics only-in-post, only-in-pre or only-in-mid,
//1 C1083 at opengl/wgl.cpp:31 on all three, kinds C1083 1 / C4100 149 / C4995 16 on all three, so the header edit made no
//diagnostic change in any other source. One instrument caveat, the same one 9f recorded: the added-file set difference reads
//empty and "removed -1" because the all-libraries set already contains a basename printing.cpp - src/platform/os/windows/
//api_layer/gdi/printing.cpp - and the logs echo basenames, so the post leg prints printing.cpp twice (tmp/_tmp_t9_9g_shapecount
//.ps1: pre 1, post 2). The count 121 -> 122 and the standalone/folder legs carry the new TU, and tmp/obj/printing.obj is a
//shared scratch name for the same reason.
//

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //The window named by hwnd is painted into the device context named by hdcBlt, and hdcBlt may be a memory DC the caller
    //owns rather than a window DC - that is the part a caller can get wrong, and nothing in the wrapper can see it. nFlags
    //selects the PW_* variants the SDK spells at um\WinUser.h:4607 PW_CLIENTONLY 0x00000001 and :4610 PW_RENDERFULLCONTENT
    //0x00000002 (the second only under #if(_WIN32_WINNT >= 0x0603), and kernel32.h:22-30 pins 0x0A00, so both are visible
    //here); the layer forwards all three arguments exactly as the caller wrote them.
    BOOL User32::PrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, PrintWindow);
        return Library::Call< BOOL, HWND, HDC, UINT >(pFunc, hwnd, hdcBlt, nFlags);
    }

}}}   //qor::win::api

