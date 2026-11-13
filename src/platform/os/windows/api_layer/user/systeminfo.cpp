// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "../kernel/errorhandler.h"

//=== api_layer conventions ========================================= Added 9 Nov 2026
//   (T9 step 9c: the "//System Information functions" section declared 4 members non-static and no TU defined any of them - new TU)
//
//T9 (registers/tasks.md, D26) is the tranche queue for U4's remaining declared-but-undefined surface, and this TU lands
//step 9c's two halves together the way 9a's strings.cpp, 9b's rawinput.cpp and T6's listbox.cpp did: it defines all 4
//members of user32.h's "//System Information functions" (user32.h:753-756) - GetSysColor GetSystemMetrics SetSysColors
//SystemParametersInfo - and user32.h gains `static` on all 4 declarations. U4's link gap (a caller of any of the 4
//failed at LNK2019) and U18's non-static shape close together. Coverage stays 759 of 1,018 - all 4 were already declared,
//so the coverage instrument counted them; this closes a link gap, never a coverage gap (the arithmetic D24 recorded for
//T6 and D27/D28 for 9a/9b). The section is 4/4 declared, 4/4 defined and 4/4 bound, and it contributes no raw gap:
//registers/u18-coverage.md names no System Information name as excluded.
//
//The AW spelling is load-bearing, not stylistic: um\WinUser.h:13268-:13272 is the stem macro switch "#ifdef UNICODE /
//#define SystemParametersInfo SystemParametersInfoW / #else / #define SystemParametersInfo SystemParametersInfoA /
//#endif", and user32.h:15 #undef's that stem before the class is declared, so the member speaks a name the SDK macro
//no longer rewrites and user32.dll never exported (G2). A bare qor_pp_useswinapi would stringize that un-exported stem,
//cache a NULL pFunc for the process lifetime and throw on every call (G5). The other 3 stems are neither #undef'd at
//user32.h:13-:21 nor macro switches, so the bare form is the D13 spelling for them, and qor_pp_useswinapiAW (which pastes
//Stem##W at kernel32.h:77) would bind GetSysColor to the non-existent export GetSysColorW - the table has GetSysColorBrush
//1C6 and no GetSysColorW row. cl /EP on this TU resolves the 4 bindings to the literals "GetSysColor" "GetSystemMetrics"
//"SetSysColors" "SystemParametersInfoW" against the module literal L"user32".
//
//SDK availability, read at the declaration line in um\WinUser.h 10.0.26100.0: GetSystemMetrics :7410, GetSysColor :9690,
//SetSysColors :9706, SystemParametersInfoA :13254, SystemParametersInfoW :13263. The GetSysColor/SetSysColors pair sit at
//depth 4 - #ifndef _WINUSER_ (:13), #ifndef NOUSER (:64), #ifndef NOCOLOR (:9621) and the Desktop Family partition
//(#pragma region :9684 / #if :9685); GetSystemMetrics shares that stack with NOSYSMETRICS (:7242) in place of NOCOLOR and
//DESKTOP at :7404/:7405; the SystemParametersInfo pair sits at depth 3 - #ifndef _WINUSER_, #ifndef NOSYSPARAMSINFO
//(:12433) and DESKTOP (:13072). NOUSER/NOCOLOR/NOSYSMETRICS/NOSYSPARAMSINFO are defined nowhere in the tree (0 hits over
//the 4,762 #define lines in src measured by _tmp_t9_9c_shrink.ps1, and 0 in um\WinUser.h's own 3,398), so D21's pin (kernel32.h:22-30, _WIN32_WINNT 0x0A00) is the live condition
//and user32.h adds no guard of its own.
//
//Signatures - D18's rule: the published signature is the prototype of the export this member binds, parameter names
//included. All 4 declarations already copy it exactly - SetSysColors spells the SDK's CONST INT * / CONST COLORREF *
//as const INT* / const COLORREF*, and SystemParametersInfo's member is the A/W pair's shared spelling - so the header
//edit is `static` only: no type and no parameter name moves, so no mangled name moves and no caller follows.
//
//SAL and return conventions (U1/U2) - 0 of the 5 SDK prototype blocks carry _Check_return_, so the compiler side is
//silent on a discarded failure; the qualifiers are _In_ on the two index members, _In_reads_(cElements) on both SetSysColors
//arrays (:9707-:9709) and _Pre_maybenull_ _Post_valid_ on SystemParametersInfo's pvParam (:13257/:13266). 2 of the 5 do
//carry _Success_(return != FALSE) - SystemParametersInfoA :13251 and SystemParametersInfoW :13260 - so FALSE is the
//SDK's own statement of failure there, while GetSysColor/GetSystemMetrics/SetSysColors carry no such claim. This TU has
//0 Win32ErrorHandler and 0 CheckReturn: the session-27 soundness test ("only sound where the failure value is not also
//a legal success value") bars a CheckNonZero at GetSysColor (a COLORREF, and 0 is a colour) and at GetSystemMetrics (a
//metric in pixels, and 0 is a legal metric), and adopting CheckReturn changes run-time behaviour rather than mirroring
//the SDK (G6 - returncheck.h:19 always selects the checked wrapper type). Every value comes back as the OS wrote it (U7).
//
//Binding - 3 x qor_pp_useswinapi(user32, <literal>) and 1 x qor_pp_useswinapiAW(user32, SystemParametersInfo). The loaded
//user32.dll table (_tmp_user32_exports.txt, 1018 names) has one row for each of the 3 bare literals - GetSysColor 1C5
//(1957), GetSystemMetrics 1C9 (1961), SetSysColors 368 (2372) - and none for the stem SystemParametersInfo: that stem's
//exports are the pair SystemParametersInfoA 3A8 (2430) / SystemParametersInfoW 3AA (2432), so no binding in this TU can
//cache a NULL pFunc except the one the AW form fixes.
//
//U17: SystemParametersInfo is one of the eleven A/W members declared without the folder's T suffix. It is not renamed
//here - the session-11 defer policy keeps the eleven as they are until one folder-wide pass, and rawinput.cpp's
//GetRawInputDeviceInfo is the identical case.

//Liveness (U19): 0 call sites of any of the 4 names go through the layer. _tmp_t9_9c_bypassw.ps1 (fresh stems over
//src/examples/test, every suffix variant fed) reads 0 LAYER / 0 INDIRECT / 8 BYPASS / 0 MEMBER: GetSystemMetrics x3 at
//ui/gl/windowimpl.cpp:521 /:1626 /:1627 and SystemParametersInfoW x3 at :453 /:454 /:480 reach the SDK import thunk
//instead of this layer, and SystemParametersInfo x2 at examples/experimental/osspecific/windows/retropad/retropad.cpp:598
//and viewmodel.cpp:326 call the bare stem, which the SDK macro at :13269 rewrites to the W export - not this member.
//
//Record: the 4 declarations needed only `static`, so this TU is the whole of step 9c. The section is 4/4 declared,
//4/4 defined and 4/4 bound; the folder moves 49 -> 50 sources, 589 -> 593 static, 45 -> 41 non-static and 588 -> 592
//bindings over the close (_tmp_static_census.ps1), and _tmp_decl_no_def_repo_scan.ps1 reads its declared-and-undefined
//count 11 -> 7.
//
//Gate: systeminfo.cpp standalone at /W4 exit 0, then the folder 50/50 at /W4 exit 0, cl /EP emitting the 4 literals
//above, and - because user32.h moved - the all-libraries run as a diff against a pre-edit run (117 cpp echoes, 166
//diagnostic lines, 1 C1083 at opengl/wgl.cpp:31, exit 2).
//
//Addendum, 12 Nov 2026 (U2 Small TU batch S4, item systeminfo sites): the U1/U2 paragraphs above are the review's
//record and stand superseded on three rows - the endpoint table now reads GetSystemMetrics checked with checkClass
//CheckNonZero< int >, SetSysColors checked BoolCheck and SystemParametersInfo checked BoolCheck, with only GetSysColor
//unchecked, so this TU carries 3 CheckReturn sites and gained its one ../returncheck.h include (0 -> 1; this TU had
//no such include before this pass, and the checks that make it live are here now). The decision is the table's, not
//re-derived here; each row's verbatim docQuote and its written reason sits at the site below. The one row the review
//expected unchecked and the table puts on a check is GetSystemMetrics: the page reserves zero for that stem and names
//no legal zero metric for it, while the GetSysColor row is the sibling whose page says outright that zero is a legal
//RGB value - that one stays unchecked. This item lands 3 tokens and 1 written reason.

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //The colour of one system colour slot: nIndex is a COLOR_* index (the COLOR_* macros run :9635-:9680 in um\WinUser.h,
    //36 of them) and the DWORD is the COLORREF the OS holds for that slot. The prototype carries no _Success_ and 0 is a
    //legal colour, so the layer hands the value over as written and a bad index is indistinguishable from black to the
    //caller - which is why there is no CheckReturn here.
    //Return policy (U2, table row GetSysColor / systeminfo.cpp, 12 Nov 2026): the row reads UNCHECKED with no check
    //class, and the table's verbatim docQuote is "Type: DWORD The function returns the red, green, blue (RGB) color
    //value of the given element. If the nIndex parameter is out of range, the return value is zero. Because zero is
    //also a valid RGB value, you cannot use GetSysColor to determine whether a system color is supported by the
    //current platform." - the page names the collision itself, so the documented failure value zero is a legal colour
    //and the session-27 soundness test (a checked-failure site is only sound where the SDK's failure value is not
    //also a legal success value) fails at that value: a check here would report a black slot as a failed call. The
    //row's callers column reads 0 in-tree callers (D14: an uncalled export is published surface), so there was no
    //call site to re-read. This site keeps 0 CheckReturn and hands the COLORREF over as written - bare qor_pp_useswinapi
    //binding, DWORD.
    DWORD User32::GetSysColor(int nIndex)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetSysColor);
        return Library::Call< DWORD, int >(pFunc, nIndex);
    }

    //One screen/desktop metric in pixels: nIndex is an SM_* index (the SM_* macros run :7248-:7398, 105 of them, in
    //um\WinUser.h) and the int is the metric. No _Success_ here either, and 0 is a legal metric, so the same "as written"
    //rule applies - the live callers below read SM_CYCAPTION / SM_CXICON / SM_CYICON / SM_CXSMICON / SM_CYSMICON through
    //the SDK import, not through this member.
    //Return policy (U2, table row GetSystemMetrics / systeminfo.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZero< int >, and the table's verbatim docQuote is "Type: int If the function succeeds, the return value is
    //the requested system metric or configuration setting. If the function fails, the return value is 0. GetLastError
    //does not provide extended error information." - the metric this stem answers with leaves through the return and
    //nothing else, so the return carries the verdict: the page reserves 0 for failure and names no legal zero metric
    //for this row, so the session-27 soundness test passes at every value. sdkRetType is int, so the class base is the
    //SDK's own int and the check class is the CheckNonZero the numeric rows already use (returncheck.h:186, Test at
    //:194 is (value == 0) ? false : true) - no new check class. The row's callers column reads 0 in-tree callers for
    //the exact stem: the SM_* reads at ui/gl/windowimpl.cpp:521 /:1626 /:1627 that the U19 paragraph above names reach
    //the SDK import thunk instead of this member, so no call site had to be re-read. G6 keeps the check live on every
    //call, and Library::Call's swallowed-fault default is a zero int, which now reads as the documented failure.
    int User32::GetSystemMetrics(int nIndex)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetSystemMetrics);
        CheckReturn< int, CheckNonZero< int > >::TType nResult = Library::Call< int, int >(pFunc, nIndex);
        return nResult;
    }

    //Repaint cElements of the colour slots: lpaElements names the slots and lpaRgbValues the colours to set them to, and
    //_In_reads_(cElements) at :9708/:9709 is the SDK's own statement that the two arrays are cElements long - the layer
    //forwards both as written and neither sizes nor pairs them. This BOOL carries no _Success_, so the layer takes no
    //hand off the return.
    //Return policy (U2, table row SetSysColors / systeminfo.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is a
    //nonzero value. If the function fails, the return value is zero. To get extended error information, call
    //GetLastError" - the colours this stem paints are the caller's own lpaRgbValues, which leave through the slots
    //rather than through the return, so the returned value is pure status and zero is reserved for failure; the
    //session-27 soundness test passes at every value, and BOOL + zero-means-failure takes the BoolCheck the library
    //already has (returncheck.h:94, Test at :99 is value ? true : false) - no new check class. The row's callers
    //column reads 0 in-tree callers (D14: an uncalled export is published surface), so no call site had to be re-read.
    //The wrapper still forwards cElements/lpaElements/lpaRgbValues exactly as written, and G6 keeps the check live
    //on every call.
    BOOL User32::SetSysColors(int cElements, const INT* lpaElements, const COLORREF* lpaRgbValues)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetSysColors);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, int, const INT *, const COLORREF * >(pFunc, cElements, lpaElements, lpaRgbValues);
        return bResult;
    }

    //The system-wide get/set switch: uiAction picks the SPI_* action (SPI_GETICONTITLELOGFONT :12466, SPI_SETMOUSETRAILS
    //:12512, SPI_GETMOUSETRAILS :12513 are the three the live callers use), uiParam is the action's size/flag argument,
    //pvParam the payload the action reads or writes (_Pre_maybenull_ so NULL is legal for the actions that take none) and
    //fWinIni requests the INI write-back. This is the one BOOL here whose FALSE the SDK itself calls a failure
    //(_Success_(return != FALSE) at :13251/:13260) - and the one binding here that must be the AW spelling; see the banner.
    //Return policy (U2, table row SystemParametersInfo / systeminfo.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "Type: BOOL If the function succeeds, the return value is a
    //nonzero value. If the function fails, the return value is zero. To get extended error information, call
    //GetLastError" - the payload this action reads or writes leaves through the caller's own pvParam, so the return is
    //pure status and zero is reserved for failure; the SDK states the reservation twice for this family, _Success_(return
    //!= FALSE) at :13251/:13260, so the session-27 soundness test passes at every value. This is an AW row whose
    //prototype the census could not resolve (NO-PROTOTYPE), so the class base is the published BOOL this wrapper
    //declares rather than an SDK spelling, and BOOL + zero-means-failure takes the BoolCheck the library already has
    //(returncheck.h:94) - no new check class. The row's callers column reads 0 in-tree callers for the exact stem: the
    //SystemParametersInfoW reads the U19 paragraph above names reach the SDK import thunk, and the bare-stem reads at
    //examples/experimental/osspecific/windows/retropad/retropad.cpp:598 and viewmodel.cpp:326 are rewritten by the SDK
    //macro to the W export, so neither reaches this member and no call site had to be re-read. The binding stays the
    //AW spelling and G6 keeps the check live on every call.
    BOOL User32::SystemParametersInfoT(UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, SystemParametersInfo);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, UINT, UINT, PVOID, UINT >(pFunc, uiAction, uiParam, pvParam, fWinIni);
        return bResult;
    }

}}}   //qor::win::api

