// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//=== api_layer conventions ========================================= Added 9 Nov 2026
//   (T9 step 9f: the "//Bitmap functions" section declared 1 member non-static and no TU defined it - new TU)
//
//T9 (registers/tasks.md, D26) is the tranche queue for U4's remaining declared-but-undefined surface, and this TU lands
//step 9f's two halves together the way 9a's strings.cpp, 9b's rawinput.cpp, 9c's systeminfo.cpp, 9d's buttoncontrol.cpp,
//9e's synchronization.cpp and T6's listbox.cpp did: it defines the one member of user32.h's "//Bitmap functions" section
//(user32.h:776 - LoadBitmap) and user32.h gains static on that one declaration. U4's link gap (a caller of LoadBitmap
//failed at LNK2019) and U18's non-static shape close together. Coverage stays 759 of 1,018 - the member was already
//declared, so the coverage instrument already counted it; this closes a link gap, never a coverage gap (the arithmetic
//D24 recorded for T6 and D27-D31 for 9a-9e). The section is 1/1 declared, 1/1 defined and 1/1 bound: the head is
//user32.h:775 and the next section "//Synchronization functions" is :778, so no member and no blank sits outside this TU,
//and registers/u18-coverage.md names no Bitmap name as an exclusion, so this TU contributes no raw gap.
//
//Binding: 1 x qor_pp_useswinapiAW(user32, LoadBitmap) and 0 x bare - the only AW binding in this TU and the first
//step since 9b's and 9c's (6/1 and 3/1) to take that form; 9a, 9d and 9e were all-bare (34/0, 3/0, 2/0). The AW
//spelling is mandatory here, and three independent measurements say so. (1) The bare stem is not exported: this
//session's fresh dumpbin dump of the loaded user32.dll (tmp/_tmp_t9_9f_exports_fresh.txt, 1,198 rows) reads NO_ROW
//for LoadBitmap and rows LoadBitmapA ordinal 2103 hint 254 rva 0004D5F0 / LoadBitmapW ordinal 2104 hint 255 rva
//0002C5B0 - the standing table agrees exactly - and the live probe (tmp/_tmp_t9_9f_probe.cpp) resolves MISSING for
//the bare stem, for Ex/ExA/ExW and for T, and RESOLVED 00007FFF2CC1D5F0 / 00007FFF2CBFC5B0 for the A and W halves
//over a user32.dll handle of 00007FFF2CBD0000, each address the handle plus the table's RVA, while a deliberately
//bogus module name resolves to NULL - so the probe discriminates rather than reporting a vacuous hit. (2) The stem
//is one of the nine user32.h #undefs, at :21. (3) The SDK declares no bare prototype at all (tmp/_tmp_t9_9f_measure.ps1
//reads 0 bare prototype declaration lines for the stem) and puts the switch on it instead, verbatim at um\WinUser.h
//10.0.26100.0 :10598-:10602 "#ifdef UNICODE / #define LoadBitmap  LoadBitmapW / #else / #define LoadBitmap  LoadBitmapA
//#endif // !UNICODE". A bare qor_pp_useswinapi would therefore stringize a literal user32.dll never exported, cache a
//NULL pFunc for the process lifetime and throw on every call (G2 + G5).
//
//The two compile legs settle the macro half rather than trusting the source read. _tmp_t9_9f_bareprobe.cpp stops the
//chain at kernel32.h - before user32.h's #undef - and takes the address of the BARE spelling: exit 0, its #pragma
//message printing BARE_CHAIN_UNICODE_defined_the_gate_chain and its two static_asserts holding, so with kernel32.h:7-10
//(#if qor_pp_unicode / #define UNICODE 1, qor_pp_unicode being configuration.h:13) in force the switch maps the bare
//stem onto the W export. _tmp_t9_9f_bareundef.cpp repeats the same line with user32.h in the chain and fails with
//C2065 'LoadBitmap' undeclared identifier at exit 2 - past user32.h:21 the only LoadBitmap of that name is the class
//member, so inside this TU the bare spelling is the member's own name and the AW spelling is what reaches the export.
//Note the instrument difference: tmp/_tmp_t9_9f_measure.ps1's own "define UNICODE in tree: 0" line is a regex artifact
//(its pattern misses kernel32.h:8, spelled "#\tdefine UNICODE 1" with a tab); the corrected read is the 1 site above.
//

//Signatures (D18: the published signature is the prototype of the export it binds, parameter names included), read as a
//forward pass at the declaration line in um\WinUser.h 10.0.26100.0 - the header the gate's /showIncludes resolves
//(tmp/_tmp_t9_9f_showinc.log:99 printing C:\Program Files (x86)\Windows Kits\10\include\10.0.26100.0\um\winuser.h; the
//ScopeCppSDK copy of the same header under VS 18 puts this block 354 lines earlier, at :10235/:10241/:10245/:10247, so
//the citation is the header the build actually reads). LoadBitmapA :10589 (HINSTANCE hInstance, LPCSTR lpBitmapName) and
//LoadBitmapW :10595 (HINSTANCE hInstance, LPCWSTR lpBitmapName), both WINUSERAPI HBITMAP WINAPI at guard depth 3 -
//#ifndef _WINUSER_ (:13), #ifndef NOUSER (:64) and #if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) (:10584),
//closed at :10604 by an #endif whose comment names that same test, so unlike 9e's stretch the #endif comments here do
//pair and the block is Desktop-only rather than Desktop-or-PC-APP. NOUSER is defined nowhere in the tree (the measure
//scan read 0 defines for NOUSER, NOBITMAP and NOGDI over 3,567 files), so D21's pin (kernel32.h:22-30, _WIN32_WINNT
//WINVER 0x0A00, NTDDI_VERSION 0x0A000010) is the live condition and user32.h adds no guard of its own.
//
//D18 needs no edit here: user32.h:776 already spells the SDK's parameter names hInstance and lpBitmapName, and the
//header's LPCTSTR is the wide spelling in this chain - um\ntdef.h puts "typedef LPCWSTR PCTSTR, LPCTSTR;" under the
//UNICODE branch - so the published (HINSTANCE, LPCTSTR) and the bound LoadBitmapW (HINSTANCE, LPCWSTR) are one type.
//That is compiled, not eyeballed: tmp/_tmp_t9_9f_protos2.cpp declares a function-pointer typedef of the published
//signature and one of the W export, initialises both with &LoadBitmapW and static_asserts is_same<LPCTSTR,LPCWSTR> and
//is_same<fPub,fW> at /W4 in the gate's own include chain - exit 0, with the negative twin tmp/_tmp_t9_9f_chainneg.cpp
//rejecting the narrow (HINSTANCE, LPCSTR) shape with C2440 at exit 2. One run settles liveness (a compiled-out
//declaration would leave LoadBitmapW undeclared) and signature (a wrong one would be C2440) together.
//
//Return convention (the G6 audit that decides the wrapper type, owed fresh here): the SDK carries no _Success_ and no
//_Check_return_ on either declaration, so nothing in the contract claims the return is a status. HBITMAP is a GDI bitmap
//handle and NULL is the failure value - the same shape as the six HICON acquirers in icon.cpp and as GetBitmapCursor/
//LoadCursor here - so a NULL folds onto a legitimate empty handle rather than onto an error, and a bad hInstance or an
//unfindable resource name is indistinguishable from that. Under the standing U1/U2 policy none of that is repaired here:
//no Win32ErrorHandler is installed, no CheckReturn is added, and the handle is handed over as written. The two parameters
//stay the caller's too - hInstance is _In_opt_ so NULL is a legal argument (the SDK itself reads the current process
//image in that case) and lpBitmapName is _In_, so the layer forwards both untouched and sizes nothing.
//

//U17 (the A/W suffix question): LoadBitmap is one of the eleven A/W members declared without the folder's T suffix, and
//it is not renamed here - the session-11 defer policy keeps the eleven as they are until one folder-wide pass, the same
//case ime.cpp's SendIMEMessageEx is in (binding already AW-correct, only the name out of step). The binding is AW-correct
//and the published name stays bare, which is also what keeps the mangled name of the declaration unchanged by the static
//edit: user32.h:776 moves only the storage-class specifier, so no name and no type moves and no caller follows.
//
//Liveness (U15/U19): 0 call sites for LoadBitmap in src, examples or test, and 0 direct-SDK bypass sites in built code.
//The bypass scan (tmp/_tmp_t9_9f_consumers.ps1 over 3,228 files) reads exactly 2 hits for the stem, both in user32.h
//itself - :21 the #undef and :776 the declaration - and 0 for the T/A/W and Ex/ExA/ExW spellings, so the static edit
//redirects nothing and this TU adds surface nothing above the layer calls yet. That is D14's exported-and-unused shape,
//which is not a finding.
//
//Census over this close (tmp/_tmp_static_census.ps1, pre vs post): user32.h keeps 634 member-declaration lines and stays
//855 lines, static 598 -> 599, non-static 36 -> 35 (the DDE block being the only non-static group left after 9g), and
//distinct defined member names / binding lines 597 -> 598 over 52 -> 53 sources, with 0 defined-but-not-static and 0
//defined-without-a-declaration both sides. tmp/_tmp_decl_no_def_repo_scan.ps1 reads its declared-and-undefined count
//2 -> 1: the pre-edit run over 3,228 files printed exactly user32.h:776 LoadBitmap and :783 PrintWindow, so the 9f name
//is gone and 9g's is all that remains; the 34 DDE ") const;" declaration lines are the scanner's known blind spot, not
//part of either count. Coverage stays 759 of 1,018.
//
//Gate: bitmap.cpp standalone at /W4 exit 0 with the log a single bitmap.cpp echo and 0 diagnostics; the folder 53/53 at
//W4 exit 0 with bitmap.cpp echoed once among the 53 and 0 diagnostics; cl /EP on this TU exit 0, emitting this TU's one
//binding literal "LoadBitmapW" against the module literal L"user32" (the other GetProcAddress literal in that output,
//"wsprintfW", is the header-inline wsprintfT body in user32.h, not a binding of this TU), 1 AW / 0 bare / 1 Library::Call
//<HBITMAP, HINSTANCE, LPCTSTR> / 1 qor_pp_fcontext / 1 definition / 0 other call forms, and the preprocessed SDK
//prototype this TU sees printed as "__declspec(dllimport) HBITMAP __stdcall LoadBitmapW( HINSTANCE hInstance, LPCWSTR
//lpBitmapName);" - so liveness and signature are read in the same run.
//
//The all-libraries leg is a diff, not an exit code: the standing 9e post log (120 cpp echoes, 166 diagnostics, exit 2)
//against the 9f pre re-run of the same 120-token rsp AFTER the user32.h:776 static edit landed (120 echoes, 166, exit 2)
//and the 9f post run of the 121-token rsp (121 echoes, 166, exit 2) - 0 diagnostics only-in-post, only-in-pre or only-in-mid,
//1 C1083 at opengl/wgl.cpp:31 on all three, kinds C1083 1 / C4100 149 / C4995 16 on all three, so the header edit made no
//diagnostic change in any other source. One instrument caveat: the added-file set difference reads empty and "removed -1"
//because the all-libraries set already contains a basename bitmap.cpp - src/platform/os/windows/api_layer/gdi/bitmap.cpp -
//and the logs echo basenames, so the post leg prints bitmap.cpp twice. The count 120 -> 121 and the standalone/folder legs
//carry the new TU instead, and tmp/obj/bitmap.obj is a shared scratch name for the same reason (dir /T W reads the obj
//later than the source, so the dating rule holds but is not exclusive to this TU).
//
//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //The one member of this section: load a named bitmap resource out of a module image and hand back its GDI handle.
    //hInstance is _In_opt_, so NULL is a legal argument and asks for the current process image rather than a foreign
    //module; lpBitmapName is the resource name the caller owns. The stem is an A/W family and the bare spelling is not
    //an export of the loaded user32.dll, so the binding is the AW form - under qor_pp_unicode 1 that reaches LoadBitmapW
    //and the narrow LoadBitmapA stays unresolved at run time, the shape every other AW stem in this library has.
    HBITMAP User32::LoadBitmap(HINSTANCE hInstance, LPCTSTR lpBitmapName)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, LoadBitmap);
        return Library::Call< HBITMAP, HINSTANCE, LPCTSTR >(pFunc, hInstance, lpBitmapName);
    }

}}}   //qor::win::api

