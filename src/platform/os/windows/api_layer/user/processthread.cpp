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

    //Five wrappers - the whole "user32.h:255 //Process and Thread functions" section: declared :256-:260, all static,
    //all defined here through the User32:: qualifier and in header order (in this TU: :81 AttachThreadInput, :90
    //GetGuiResources, :99 IsWow64Message, :106 UserHandleGrantAccess, :115 WaitForInputIdle). 5/5 declarations pair
    //with a definition here and 0 parameter names diverge - neither from the header nor from the SDK prototypes - so
    //this TU contributes 0 U4 (declared, never defined), 0 U11 (definition unqualified), 0 U13 (defined, declared
    //non-static), 0 U17 (A/W family without the folder's T suffix) and 0 U18 (return type contradicts the SDK):
    //tmp/_tmp_pt_pre_s27.txt (5/5 pair, 0 hdr-vs-def, 0 def-vs-sdk), tmp/_tmp_name_divergence_scan.ps1
    //(-Tu processthread.cpp: 0 of 5), tmp/_tmp_tsuffix_s27.txt (all 5 members in the 404 single-export class).
    //
    //Binding (D13): all five stems are single exports with no A/W family and no winuser.h #define for the stem, so
    //the bare qor_pp_useswinapi at :84, :93, :102, :109 and :118 is the only spelling that can bind them at all -
    //user32.dll exports AttachThreadInput (F / 0002E9B0), GetGuiResources (15F / 0002EF50), IsWow64Message
    //(24F / 0004C4F0), UserHandleGrantAccess (3D8 / 0002FEF0) and WaitForInputIdle (3E9 / 0002B8F0) in
    //tmp/_tmp_user32_exports.txt, and cl /EP over the TU emits exactly those five literals
    //(tmp/_tmp_pt_pp_pre_s27.txt): 5/5 present in the export table, 0 winuser.h macro, hence 0 P1, 0 P2 (there is no
    //macro to rewrite the name, G1) and 0 P6 (no bare export beside an A/W family) - tmp/_tmp_pt_class_s27.ps1. The
    //folder census counts all five bindings here as resolvable (428 bindings, 1 unresolvable, none in this TU -
    //tmp/_tmp_p1census_pre_s27.txt) and the wrong-stem scan has no processthread.cpp row
    //(_tmp_wrongstem_s27.txt: 3 rows, all the known-benign cursor.cpp and resource.cpp ones).
    //
    //Signatures: five members against five SDK prototypes, every return agreeing - AttachThreadInput BOOL
    //(winuser.h:4103), GetGuiResources DWORD (:12417), IsWow64Message BOOL with a VOID parameter list (:3679),
    //UserHandleGrantAccess BOOL (:14925), WaitForInputIdle DWORD (:4128) - tmp/_tmp_pt_ret_s27.ps1 (the SDK puts the
    //return type on its own line above the stem, session 22's caveat). Every member's parameter names match the SDK
    //verbatim, and all five Call template lists restate the definition's parameter types exactly
    //(tmp/_tmp_sigcmp.ps1 -Tu processthread.cpp -First 256 -Last 260: 5/5 "decl==def ok" and 5/5 call-site "ok").
    //
    //Version guards (U16, tmp/_tmp_pt_guards_s27.ps1): three of the five prototypes are version-guarded and all five
    //are WINAPI_PARTITION_DESKTOP only - GetGuiResources under #if(WINVER >= 0x0500) at winuser.h:12413,
    //UserHandleGrantAccess under #if(_WIN32_WINNT >= 0x0500) at winuser.h:14920, IsWow64Message under
    //#if(_WIN32_WINNT >= 0x0501) at winuser.h:3675 (the item's second 0x0501 case, after painting.cpp's
    //GetWindowRgnBox); AttachThreadInput and WaitForInputIdle sit in the winuser.h:4098-:4132 Desktop-Family
    //sub-block with no version guard of their own - the innermost enclosing one is the file-wide #ifndef NOUSER at
    //winuser.h:64. Note the empty #if (_WIN32_WINNT >= 0x602) / #endif pair at winuser.h:4121-:4122 between the two
    //declarations: it guards nothing, and a backward scan without a depth counter would report 0x0602 for
    //WaitForInputIdle. No build-wide version macro was touched - the pin stays deferred (session 5).
    //
    //Return conventions (recorded, not changed - U1/U2 policy): AttachThreadInput and UserHandleGrantAccess return
    //FALSE on failure and both carry CheckReturn< BOOL, BoolCheck >; GetGuiResources' DWORD handle count shares 0
    //with its SDK failure value and carries CheckReturn< DWORD, CheckNonZero< DWORD> >, which is the kernel layer's
    //own spelling (kconsole.cpp uses it five times); IsWow64Message returns a state, not a verdict; and
    //WaitForInputIdle returns WAIT_OBJECT_0 when an idle state is entered, WAIT_TIMEOUT on timeout and 0xFFFFFFFF on
    //failure - so a non-zero check there would call success a failure, and it is correctly unchecked. These are 3 of
    //the 4 CheckReturn sites in the library (the 4th is errorhandling.cpp MessageBeep); G6 keeps all three live on
    //every call, so a legitimate zero and a swallowed SEH fault stay indistinguishable to the caller (U7).
    //
    //Reachability (D14: an uncalled export is published surface, not a finding; measured here for U19): only
    //IsWow64Message has an in-tree caller - ui/gui/view/handlers/messagehandler.cpp:246
    //"return User32::IsWow64Message() ? true : false;", live code in a compiled target (ui/gui/CMakeLists.txt:60).
    //The U19 scan over the five stems plus their A/W/T spellings gives 1 LAYER / 0 INDIRECT / 0 BYPASS / 0 MEMBER
    //(tmp/_tmp_pt_bypass_s27.txt): nothing in-tree reaches any of these entry points through the SDK import, so the
    //four unchecked-failure paths above are only reachable through the published library surface.
    //
    //Coverage (method step 7): this section's five exports are all wrapped, so the TU adds nothing to the folder's
    //coverage gap, and the neighbouring thread/foreground entry points are wrapped in other TUs - user32.h:161
    //GetGUIThreadInfo (window.cpp:243), user32.h:132 AllowSetForegroundWindow (window.cpp:37), user32.h:187
    //LockSetForegroundWindow (window.cpp:425), user32.h:207 SwitchToThisWindow (window.cpp:565), user32.h:409
    //GetThreadDesktop (desktop.cpp:111), user32.h:414 SetThreadDesktop (desktop.cpp:146). The gap was measured
    //(tmp/_tmp_pt_coverage_s27.ps1): of 1,018 user32 export names, 370 have no member declaration in user32.h at all -
    //114 declared in the installed winuser.h (the DPI, pointer-input and display-config families) and 256 not declared
    //there. Two names adjacent to this TU's subject matter show both shapes: SetThreadInputBlocked (ordinal 371) is
    //declared nowhere in the header or in winuser.h, while DdeSetUserHandle (9C) is declared at user32.h:528 as one of
    //the 34 non-static const DDE members no TU defines (U4's largest block). Neither belongs to this section and no
    //header line was touched.
    //
    //Addendum, 12 Nov 2026 (U2 Small TU batch S4, item processthread sites): the U1/U2 paragraphs above are the
    //review's record and stand superseded on two rows - the endpoint table now reads AttachThreadInput checked
    //BoolCheck, GetGuiResources checked CheckNonZero< DWORD >, UserHandleGrantAccess checked BoolCheck, and
    //IsWow64Message and WaitForInputIdle unchecked with no check class, so this TU keeps exactly 3 CheckReturn
    //sites and its one ../returncheck.h include at :12 (unchanged at 1 - it was already live for the seeds). The
    //decision is the table's, not re-derived here; each row's verbatim docQuote and its written reason sits at the
    //site below. Both rows in play end unchecked, so this item lands 0 tokens; the 3 seeds for AttachThreadInput,
    //GetGuiResources and UserHandleGrantAccess are never re-edited.

    BOOL User32::AttachThreadInput(DWORD idAttach, DWORD idAttachTo, BOOL fAttach)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, AttachThreadInput);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, DWORD, DWORD, BOOL>(
            pFunc, idAttach, idAttachTo, fAttach);
        return bResult;
    }

    DWORD User32::GetGuiResources(HANDLE hProcess, DWORD uiFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetGuiResources);
        CheckReturn< DWORD, CheckNonZero< DWORD> >::TType dwResult = Library::Call<DWORD, HANDLE, DWORD>(
            pFunc, hProcess, uiFlags);
        return dwResult;
    }

    //Return policy (U2, table row IsWow64Message / processthread.cpp, 12 Nov 2026): the row reads UNCHECKED with no
    //check class, and the table's verbatim docQuote is "The function returns TRUE if the last message read from the
    //current thread's queue originated from a WOW64 process, and FALSE otherwise." - the value is the answer this
    //stem gives about the queue, not a verdict on the call: TRUE and FALSE are both legal outcomes, nothing is
    //reserved for failure, and the session-27 soundness test (a checked-failure site is only sound where the SDK's
    //failure value is not also a legal success value) fails here, so a check would report "the last message did not
    //come from a WOW64 process" as a call failure. The row's callers column reads 1, re-read before this verdict was
    //accepted: ui/gui/view/handlers/messagehandler.cpp:246 "return User32::IsWow64Message() ? true : false;" inside
    //bool MessageHandler::IsWow64(), which folds the BOOL into its own bool and never tests a failure spelling, so
    //nothing at that call site would change meaning under a check. This site keeps 0 CheckReturn and hands the state
    //over as written - bare qor_pp_useswinapi binding, BOOL.
    BOOL User32::IsWow64Message(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsWow64Message);
        return Library::Call<BOOL>(pFunc);
    }

    BOOL User32::UserHandleGrantAccess(HANDLE hUserHandle, HANDLE hJob, BOOL bGrant)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, UserHandleGrantAccess);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HANDLE, HANDLE, BOOL>(
            pFunc, hUserHandle, hJob, bGrant);
        return bResult;
    }

    //Return policy (U2, table row WaitForInputIdle / processthread.cpp, 12 Nov 2026): the row reads UNCHECKED with no
    //check class, and the table's verbatim docQuote is "The following table shows the possible return values for this
    //function. Return code/value Description 0 The wait was satisfied successfully. WAIT_TIMEOUT The wait was
    //terminated because the time-out interval elapsed. WAIT_FAILED An error occurred." - an inverted site in session-27
    //terms: the cell the page prints for the satisfied wait is 0, so a zero-check would call a successful wait a
    //failure, and the one documented error cell (WAIT_FAILED) is the odd value such a check would leave reading as
    //success; the soundness test fails at every value, so no check class can be put on this return without
    //misreporting a legal WAIT_* answer. The row's callers column reads 0 in-tree callers (D14: an uncalled export is
    //published surface), so there was no call site to re-read. This site keeps 0 CheckReturn and hands the WAIT_*
    //code over as written - bare qor_pp_useswinapi binding, DWORD.
    DWORD User32::WaitForInputIdle(HANDLE hProcess, DWORD dwMilliseconds)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, WaitForInputIdle);
        return Library::Call<DWORD, HANDLE, DWORD>(pFunc, hProcess, dwMilliseconds);
    }

}}}//qor::win::api
