// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //T5 closed here: this TU now owns the whole IME/WINNLS block. Before this pass the unlabelled run of declarations
    //between //Window-Class functions and //Shell functions in user32.h carried four members and this TU defined only
    //SendIMEMessageEx, while DisableProcessWindowsGhosting and WINNLSEnableIME were declared non-static and defined in
    //no TU (the U4 shape -> LNK2019 at any caller) and the SDK's sibling WINNLS entry points WINNLSGetIMEHotkey and
    //WINNLSGetEnableStatus were declared nowhere. This pass makes the pair static, wraps them, and adds the two WINNLS
    //siblings as static declarations plus definitions. The block is now this TU's five members, in header order:
    //DisableProcessWindowsGhosting, SendIMEMessageEx, WINNLSGetIMEHotkey, WINNLSEnableIME, WINNLSGetEnableStatus -
    //GetDpiForWindow stays in window.cpp as it did. WINNLSEnableIME keeps the SDK's own "obsolete" warning as a one-line
    //comment above its declaration and remains on the published interface.
    //Binding: qor_pp_useswinapiAW(user32, SendIMEMessageEx) is D13-conformant and load-bearing. cl /EP emits the single
    //literal "SendIMEMessageExW" (tmp/_tmp_ime_pp.txt:453241); the live probe says bare SendIMEMessageEx MISSING while
    //SendIMEMessageExA (ordinal 313) and SendIMEMessageExW (314) are RESOLVED, so a bare qor_pp_useswinapi here would
    //cache a NULL pFunc and throw on every call (G5). The member is one of U17's eleven A/W wrappers that carry no T
    //suffix - untouched under the session-11 defer policy.
    //The four members added by this pass are all bare single exports, so the bare qor_pp_useswinapi is the D13 spelling
    //for each: DisableProcessWindowsGhosting (ordinal BB), WINNLSEnableIME (3E6, its A/W spellings MISSING), WINNLSGetEnableStatus
    //(3E7) and WINNLSGetIMEHotkey (3E8) all RESOLVED under their bare stem in the probe (tmp/_tmp_name_resolve_s19b.txt).
    //None of the four stems is #undef'd at user32.h:13-21, and no SDK macro is in scope for any of them - WinNls32.h and
    //Ime.h are opt-in and absent from this layer's include chain, so the WINNLS/IME Stem->StemW macros the folder-wide
    //D13 rule warns about never expand here, which is why the explicit AW form is needed only for SendIMEMessageEx.
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
    //WINNLSEnableIME 3E6, WINNLSGetEnableStatus 3E7, WINNLSGetIMEHotkey 3E8 - and user32.h now declares four of them
    //(SendIMEMessageEx plus the WINNLS trio), all four wrapped here. Each
    //IMP* family resolves only as A/W (bare IMPGetIME/IMPQueryIME/IMPSetIME are MISSING), so they would need AW
    //bindings if they were ever wrapped. The IMM API proper is a different case: Windows.h:250 includes <imm.h>, so 64
    //Imm* prototypes (imm32.dll, not user32.dll) sit in every TU's chain and none is wrapped anywhere in the tree.
    //Liveness (U15): 1 of 1 unreachable. The sole caller is WindowController::SendIMEMessageEx
    //(ui/gui/controllers/windowcontroller.cpp:773-775, published at windowcontroller.h:282), which has no call site in
    //src/examples/test. The gui IME path is dead on its own account too: IMEHandler's slot m_IME (interactive.h:57) is
    //read at interactive.cpp:41 and written nowhere, and IMEHandler calls no wrapper. U19: 1 LAYER / 1 INDIRECT /
    //0 BYPASS / 0 MEMBER.

    //DisableProcessWindowsGhosting takes nothing and returns nothing (fire-and-forget, no failure signal - the SDK
    //prototype is void(void)), so Library::voidCall matches the SDK and this adds no U18 instance.
    void User32::DisableProcessWindowsGhosting(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DisableProcessWindowsGhosting);
        Library::voidCall<>(pFunc);
    }

    LRESULT User32::SendIMEMessageEx(HWND hwnd, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SendIMEMessageEx);
        return Library::Call< LRESULT, HWND, LPARAM >(pFunc, hwnd, lParam);
    }

    //WINNLSGetIMEHotkey returns UINT (WinNls32.h:87: UINT WINAPI WINNLSGetIMEHotkey(HWND)) - a hotkey code, not a
    //success flag, so it comes back raw with no CheckReturn, matching the U1/U2 policy for this block.
    UINT User32::WINNLSGetIMEHotkey(HWND hwnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, WINNLSGetIMEHotkey);
        return Library::Call<UINT, HWND>(pFunc, hwnd);
    }

    //WINNLSEnableIME returns BOOL (WinNls32.h:88: BOOL WINAPI WINNLSEnableIME(HWND, BOOL)) and the SDK marks it obsolete;
    //it stays on the interface (see the one-line obsolete comment at its declaration) and returns raw - no CheckReturn,
    //so a swallowed fault and a legitimate FALSE are indistinguishable (U7), the same convention as SendIMEMessageEx.
    BOOL User32::WINNLSEnableIME(HWND hwnd, BOOL bFlag)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, WINNLSEnableIME);
        return Library::Call<BOOL, HWND, BOOL>(pFunc, hwnd, bFlag);
    }

    //WINNLSGetEnableStatus returns BOOL (WinNls32.h:89: BOOL WINAPI WINNLSGetEnableStatus(HWND)) - the IME enable state
    //for the window, returned raw per the same U1/U2 policy.
    BOOL User32::WINNLSGetEnableStatus(HWND hwnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, WINNLSGetEnableStatus);
        return Library::Call<BOOL, HWND>(pFunc, hwnd);
    }

}}}//qor::win::api
