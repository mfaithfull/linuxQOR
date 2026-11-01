// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //One wrapper, one member: user32.h:636 SendIMEMessageEx. That declaration sits in an unlabelled four-declaration
    //block, user32.h:635-639, between //Window-Class functions (:619) and //Shell functions (:641) - the header has no
    //"IME functions" section comment to match, and the block is U21-shaped: this TU owns 1 of its 4 members,
    //DisableProcessWindowsGhosting (:635) and WINNLSEnableIME (:637) are declared non-static and defined in no TU (U4),
    //and GetDpiForWindow (:639) belongs to window.cpp.
    //Binding: qor_pp_useswinapiAW(user32, SendIMEMessageEx) is D13-conformant and load-bearing. cl /EP emits the single
    //literal "SendIMEMessageExW" (tmp/_tmp_ime_pp.txt:453241); the live probe says bare SendIMEMessageEx MISSING while
    //SendIMEMessageExA (ordinal 313) and SendIMEMessageExW (314) are RESOLVED, so a bare qor_pp_useswinapi here would
    //cache a NULL pFunc and throw on every call (G5). The member is one of U17's eleven A/W wrappers that carry no T
    //suffix - untouched under the session-11 defer policy.
    //No SDK prototype reaches this TU. The declarations are um/Ime.h:35-36, behind #ifndef _IME_ (:16) and the Desktop
    //partition (:22) with no version guard (U16: 0 of 1), and Ime.h has no includer anywhere in SDK 10.0.26100 - so the
    //SDK's own SendIMEMessageEx -> SendIMEMessageExW macro is not in scope here either, unlike every other A/W wrapper
    //in this folder (G1). um/WinNls32.h is opt-in the same way and declares WINNLSEnableIME (:88),
    //WINNLSGetEnableStatus (:89), WINNLSGetIMEHotkey (:87) and the IMPGetIME/IMPQueryIME/IMPSetIME A/W pairs (:65-85).
    //Contract: the SDK documents lParam as a PIMESTRUCT (um/Ime.h:46-55), a type named nowhere in src - 0 occurrences in
    //this TU's preprocessed output and 0 repo-wide - so a caller has to hand-build a mirror struct whose layout nothing
    //checks (the U26 mirror class, the one that produced the ICONINFOEX overrun). The LRESULT comes back raw: no
    //Win32ErrorHandler and no CheckReturn per the U1/U2 policy, so a swallowed fault and a legitimate 0 are
    //indistinguishable (U7).
    //Coverage: thirteen IME-shaped entry points are exported - CliImmSetHotKey 4B, IMPGetIMEA/W 203/204,
    //IMPQueryIMEA/W 205/206, IMPSetIMEA/W 207/208, SendIMEMessageExA/W 313/314, User32InitializeImmEntryTable 3D6,
    //WINNLSEnableIME 3E6, WINNLSGetEnableStatus 3E7, WINNLSGetIMEHotkey 3E8 - and user32.h declares two of them. Each
    //IMP* family resolves only as A/W (bare IMPGetIME/IMPQueryIME/IMPSetIME are MISSING), so they would need AW
    //bindings if they were ever wrapped. The IMM API proper is a different case: Windows.h:250 includes <imm.h>, so 64
    //Imm* prototypes (imm32.dll, not user32.dll) sit in every TU's chain and none is wrapped anywhere in the tree.
    //Liveness (U15): 1 of 1 unreachable. The sole caller is WindowController::SendIMEMessageEx
    //(ui/gui/controllers/windowcontroller.cpp:773-775, published at windowcontroller.h:282), which has no call site in
    //src/examples/test. The gui IME path is dead on its own account too: IMEHandler's slot m_IME (interactive.h:57) is
    //read at interactive.cpp:41 and written nowhere, and IMEHandler calls no wrapper. U19: 1 LAYER / 1 INDIRECT /
    //0 BYPASS / 0 MEMBER.

    LRESULT User32::SendIMEMessageEx(HWND hwnd, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SendIMEMessageEx);
        return Library::Call< LRESULT, HWND, LPARAM >(pFunc, hwnd, lParam);
    }

}}}//qor::win::api
