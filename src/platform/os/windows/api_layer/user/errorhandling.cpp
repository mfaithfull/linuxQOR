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

    //The "Error Handling functions" section (user32.h:613-617): four declarations, four definitions here, in the
    //header's order, all static, all qualified User32::, and all four call sites explicit about the SDK argument
    //types (U14: 4/4 explicit; inner spacing is split 2 tight / 2 spaced in this file against 250/179 folder-wide,
    //so it is left for the folder-wide U14(a) pass). The section is fully covered by this TU - no U4 member, no U11
    //unqualified definition, no U13 non-static member.
    //All four entry points are single exports with no A/W family, so the bare qor_pp_useswinapi is the D13 spelling
    //and the AW form would break all four. cl /EP on this TU resolves the literals "FlashWindow", "FlashWindowEx",
    //"MessageBeep" and "SetLastErrorEx" (tmp/_tmp_errh_pp.txt:453288, :453295, :453302, :453310 of the 31 Oct run -
    //cl /EP keeps one output line per source line, so those numbers shift with the weight of this comment block
    //while the literals do not), the export table gives
    //FlashWindow 115, FlashWindowEx 116, MessageBeep 283,
    //SetLastErrorEx 342, and the live probe (tmp/_tmp_name_resolve_probe.cpp) reports all four RESOLVED with every
    //*A and *W spelling MISSING. None of the four stems is #undef'd at user32.h:9-17, so G1 has nothing to expand
    //and the spelling in the source is the name handed to GetProcAddress. The wrong-stem scan
    //(tmp/_tmp_wrong_stem_scan.ps1) reports no binding-vs-member disagreement in this file.
    //The return conventions differ per wrapper and only one of them is checked. FlashWindow's BOOL is documented as
    //the window's active state BEFORE the call, not a success flag, so a BoolCheck there would report a successful
    //flash as a failure (see status.md U24 for the one in-tree caller that treats this value as a success flag);
    //FlashWindowEx returns FALSE on failure and is unchecked; MessageBeep returns FALSE on failure and is this TU's
    //single checked site - 1 of the 4 CheckReturn sites in this library, the other 3 in processthread.cpp - and per
    //G6 that check is always live: cl /EP keeps the checked branch of returncheck.h:19-23 (_tmp_errh_pp.txt:452867),
    //so a FALSE from MessageBeep raises a continuable issue on every call. SetLastErrorEx returns VOID
    //(winuser.h:13558-13563), so Library::voidCall matches the SDK and this TU adds no U18 instance.
    //Contracts passed through unchanged: FlashWindowEx reads pfwi->cbSize and fails unless it is sizeof(FLASHWINFO),
    //and the struct is the SDK's own - nothing is mirrored here and this library exports neither the FLASHW_* flags
    //(winuser.h:4702-4707) nor the SLE_* severities (winuser.h:13551-13553) that dwType must be one of; MessageBeep's
    //uType is a sound selector rather than a MessageBox style mask (the MB_* constants at winuser.h:9159-9182 are the
    //spellings a caller can reach, and the header defines no MB_ICONSIMPLE/MB_ICONCOMPLETION at all); SetLastErrorEx
    //sets the calling thread's last-error code, and the SLE_* severity is only read by a debugger.
    //U16 guard stack, measured on winuser.h 10.0.26100 with tmp/_tmp_sdk_guard_scan.ps1: FlashWindow :4683 under
    //WINAPI_PARTITION_DESKTOP (:4668) inside #ifndef NOUSER (:64); FlashWindowEx :4699 additionally under
    //#if(WINVER >= 0x0500) (:4687, closed :4709), and the FLASHWINFO/PFLASHWINFO typedef its signature names is
    //inside that same guard (:4688-4694) - so a consumer pinning WINVER below 0x0500 does not merely lose the
    //prototype, user32.h:615 becomes ill-formed on an unknown type, the HDEVNOTIFY/DEVMODE/MSGBOXPARAMSW sub-case;
    //MessageBeep :9365 under #ifndef NOMB (:9154) + Desktop (:9360); SetLastErrorEx :13561 under Desktop only
    //(:13556). No WINVER is pinned anywhere in this build, so all four are visible today.
    //Section coverage: user32.dll exports two more error-adjacent entry points that user32.h declares nowhere -
    //RegisterErrorReportingDialog 2E6 and SetDebugErrorLevel 32F, both RESOLVED in the probe - plus DdeGetLastError
    //8E, whose only declaration here is the non-static DDE member at user32.h:517 (U4). GetLastError/SetLastError are
    //correctly absent: the probe confirms both are MISSING from user32.dll, they are kernel32 exports.
    //Liveness (U15): three of the four names occur nowhere in src/examples/test outside this TU and the header.
    //FlashWindow has exactly one call site, WindowController::Flash (ui/gui/controllers/windowcontroller.cpp:717,
    //built at gui/CMakeLists.txt:27), and that method's only in-tree caller is WindowImpl::RequestAttention
    //(ui/gl/windowimpl.cpp:1835), which is in the winqor_gl target ui/CMakeLists.txt:4 comments out of the build - so
    //the wrapper is compiled and reachable but nothing built invokes it. The U19 scan over the four stems gives
    //1 LAYER / 0 BYPASS / 0 MEMBER: no site in the tree reaches these entry points through the SDK import.

    //The window's active state BEFORE the flash - the answer to a question about the caption, not a status word.
    //Return policy (U2, table row FlashWindow / errorhandling.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, and the table's verbatim docQuote is "The return value specifies the window's state before the call to the FlashWindow function. If the window caption was drawn as active before the call, the return value is nonzero. Otherwise, the return value is zero." - zero is the documented ANSWER for a window that was already inactive rather than a failure value, so the session-27 soundness test (a checked-failure site is only sound where the SDK's failure value is not also a legal success value) fails at that value before any check is considered, and a BoolCheck here would report a perfectly successful flash of an inactive window as a failure. BOOL + previous-state shape, 0 in-tree callers for the exact stem (D14 published surface) and the row is unchecked on the page's own wording rather than on that count; no token from this row and the answer comes back exactly as the OS wrote it.
    BOOL User32::FlashWindow(HWND hWnd, BOOL bInvert)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, FlashWindow);
        return Library::Call<BOOL, HWND, BOOL>(pFunc, hWnd, bInvert);
    }

    //The same previous-state BOOL as FlashWindow's; the one caller in the tree reads it as a success flag.
    //Return policy (U2, table row FlashWindowEx / errorhandling.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, and the table's verbatim docQuote is "The return value specifies the window's state before the call to the FlashWindowEx function. If the window caption was drawn as active before the call, the return value is nonzero. Otherwise, the return value is zero." - zero is the documented answer for a window that was already inactive, and the page names no failure value for the FLASHW_* requests at all, so the session-27 soundness test (a checked-failure site is only sound where the SDK's failure value is not also a legal success value) fails at that value before any check is considered. The 1 in-tree caller was re-read before the verdict was accepted (ui/gui/controllers/windowcontroller.cpp:729 - WindowController::Flash, built at gui/CMakeLists.txt:27, which writes User32::FlashWindowEx(&flashInfo) ? true : false): that reading is the U24 instance closed by D44 and not a failure signal this wrapper could act on, so the row keeps its unchecked verdict and the caller keeps its reading. No token from this row and the answer comes back exactly as the OS wrote it.
    BOOL User32::FlashWindowEx(::PFLASHWINFO pfwi)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, FlashWindowEx);
        return Library::Call<BOOL, ::PFLASHWINFO>(pFunc, pfwi);
    }

    BOOL User32::MessageBeep(UINT uType)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, MessageBeep);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, UINT >(pFunc, uType);
        return bResult;
    }

    //void both here and in the SDK (winuser.h:13558-13563), so Library::voidCall matches and there is no return to inspect; the row's verdict is void (page carries no Return value section) and this row adds neither a token nor a reason. 0 in-tree callers for the stem, and the call's own success is unobservable through this member either way (U7).
    void User32::SetLastErrorEx(DWORD dwErrCode, DWORD dwType)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetLastErrorEx);
        Library::voidCall< DWORD, DWORD >(pFunc, dwErrCode, dwType);
    }

}}}//qor::win::api
