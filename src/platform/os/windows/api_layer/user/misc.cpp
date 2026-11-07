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
//   (T8 step 6, tranche 6f: the Misc bucket - 24 names / 23 wrappers, new TU)
//
//This TU is coverage added under D18 (docs/agent/DECISIONS.md): purpose.md states the library intends a full set of
//wrappers over the user32.dll exports, and registers/u18-coverage.md buckets the gap by family. The Misc bucket is what
//is left of the declared-and-unwrapped gap once the five family tranches are drained (6a Pointer 32, 6b DPI / awareness
//21, 6c Touch / gesture 14, 6d Display config 7, 6e Shutdown / power 3). It is derived, not typed: tmp/_tmp_t8_6f_pre.ps1
//filters the gap list tmp/_tmp_u18_gap_sdk.txt (written by tmp/_tmp_u18_coverage4.ps1) for everything outside the legacy
//hook and legacy word / class-long families, and that yields 24 names. All 24 are exported from user32.dll AND declared
//in an SDK header, with 0 function-like macros and 0 rows already in user32.h - but 23 members, because ChangeMenuA/W is
//the only A/W pair left in the bucket (ChangeMenu has no bare export: the pre-scan measures ChangeMenu=0 ChangeMenuA=1
//ChangeMenuW=1) and the library's T-suffix convention (D13, model windowclass.cpp:79 GetClassLongPtr / qor_pp_useswinapiAW
//at :79) collapses the pair to one ChangeMenuT member bound through qor_pp_useswinapiAW. configuration.h:13 sets
//qor_pp_unicode 1, so this build binds the W form (ordinal 2B) and the A form (2A) stays unresolved at runtime - the
//same shape every other AW stem in this folder already has, and the reason U17's A/W question is one wrapper, not dozens.
//
//Signature rule (D18): the published signature IS the winuser.h prototype, parameter names included - 23/23 copy it
//exactly. SDK declaration lines (winuser.h, um/10.0.26100.0): 7798 CalculatePopupWindowPosition, 7519 ChangeMenuA,
//7528 ChangeMenuW, 15665 ChangeWindowMessageFilterEx, 6764 EvaluateProximityToPolygon, 6756 EvaluateProximityToRect,
//16057 GetAutoRotationState, 15993 GetCIMSSM, 5490 GetClipboardMetadata, 15987 GetCurrentInputMessageSource,
//3671 GetUnpredictedMessagePos, 4788 GetWindowDisplayAffinity, 6800 GetWindowFeedbackSetting, 14529 InheritWindowMonitor,
//16094 IsImmersiveProcess, 16403 IsWindowArranged, 16382 RegisterForTooltipDismissNotification,
//16359 SetAdditionalForegroundBoostProcesses, 7106 SetCoalescableTimer, 13541 SetDebugErrorLevel, 16100
//SetProcessRestrictionExemption, 10874 SetThreadCursorCreationScaling, 4795 SetWindowDisplayAffinity, 6810
//SetWindowFeedbackSetting.
//
//Guards, forward pass over the declaring header (tmp/_tmp_t8_6f_pre2.ps1): every name sits inside the desktop partition;
//the innermost WINVER blocks are 0x0601, 0x0602, 0x0604 (IsWindowArranged) and 0x0607 (RegisterForTooltipDismissNotification,
//SetAdditionalForegroundBoostProcesses). GetUnpredictedMessagePos sits behind #if(_WIN32_WINNT >= 0x0602) at winuser.h:3671
//- the 6e-close pre-flags called it WINNT-guarded, and that is wrong: the forward pass at the declaration line itself
//reports winnt_guard_nearby=0, and winuser.h:3673 closes the block with the _WIN32_WINNT comment. SetThreadCursorCreationScaling
//sits inside #if (NTDDI_VERSION >= NTDDI_WIN10_CO) at winuser.h:10867-:10876, and that block IS live here: sdkddkver.h:242
//sets NTDDI_VERSION to 0x0A000010, which is NTDDI_WIN11_GE (sdkddkver.h:164), while NTDDI_WIN10_CO is 0x0A00000B
//(sdkddkver.h:159) - so 0x0A000010 >= 0x0A00000B. The /EP probe (tmp/_tmp_t8_6f_ver.cpp) measures WINVER 0x0A00 and
//_WIN32_WINNT 0x0A00 for this build, and 0 files under src, cmake, examples or test pin any of them. Liveness was
//settled before the constants were read by tmp/_tmp_t8_6f_protos.cpp: 24 decltype(&name) rows plus 24 spelled
//signatures, /W4, no QOR define and no version override, exit 0 - a guarded-out name would be C2065 and a wrong
//signature C2440. The layer adds no #if of its own.
//
//Return conventions: 1 VOID (SetDebugErrorLevel -> Library::voidCall) and 22 value-returning, 21 of them BOOL success
//flags. Two are not flags: SetCoalescableTimer returns UINT_PTR timer-id where 0 is a failure the caller must test, and
//SetThreadCursorCreationScaling returns the scaling that took effect. GetUnpredictedMessagePos is the TU's only zero-argument
//wrapper - the SDK writes DWORD WINAPI GetUnpredictedMessagePos(VOID) and the layer matches desktop.cpp:107
//GetProcessWindowStation's Library::Call< T >(pFunc) shape (declared user32.h:410); its 0 is a legal answer, so it is a
//value and there is nothing for U2 to check. No name in the bucket carries _Check_return_ or a documented GetLastError
//convention, so this TU adds 0 Win32ErrorHandler and 0 CheckReturn sites - U1 and U2 stay open on this bucket.


namespace qor { namespace win { namespace api {

    //BOOL; the answer is the out parameter popupWindowPosition, and excludeRect is _In_opt_ - a legal NULL the layer
    //forwards unchecked (U1/U2). flags is the TPM_XXX group, passed as UINT exactly as the SDK declares it.
    BOOL User32::CalculatePopupWindowPosition(const ::POINT *anchorPoint, const ::SIZE *windowSize, UINT flags, ::RECT *excludeRect, ::RECT *popupWindowPosition)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CalculatePopupWindowPosition);
        return Library::Call< BOOL, const ::POINT *, const ::SIZE *, UINT, ::RECT *, ::RECT * >(pFunc, anchorPoint, windowSize, flags, excludeRect, popupWindowPosition);
    }

    //One member for the A/W pair, per D13 and this library's T-suffix convention (the model is windowclass.cpp:76
    //GetClassLongPtrT / :79 qor_pp_useswinapiAW): the macro appends W or A to the stem according to qor_pp_unicode,
    //so the published parameter is the wide form and this build binds ChangeMenuW.
    BOOL User32::ChangeMenuT(HMENU hMenu, UINT cmd, LPCWSTR lpszNewItem, UINT cmdInsert, UINT flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, ChangeMenu);
        return Library::Call< BOOL, HMENU, UINT, LPCWSTR, UINT, UINT >(pFunc, hMenu, cmd, lpszNewItem, cmdInsert, flags);
    }

    //BOOL as a real success flag; pChangeFilterStruct is _Inout_opt_ and reports what changed, so a caller that passes
    //NULL learns nothing about the filter that was already in place.
    BOOL User32::ChangeWindowMessageFilterEx(HWND hwnd, UINT message, DWORD action, PCHANGEFILTERSTRUCT pChangeFilterStruct)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, ChangeWindowMessageFilterEx);
        return Library::Call< BOOL, HWND, UINT, DWORD, PCHANGEFILTERSTRUCT >(pFunc, hwnd, message, action, pChangeFilterStruct);
    }

    //BOOL, and the answer is pProximityEval; controlPolygon is _In_reads_(numVertices), so numVertices and the array
    //length are the caller's contract - the layer cannot see a short array.
    BOOL User32::EvaluateProximityToPolygon(UINT32 numVertices, const ::POINT *controlPolygon, const TOUCH_HIT_TESTING_INPUT *pHitTestingInput, TOUCH_HIT_TESTING_PROXIMITY_EVALUATION *pProximityEval)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EvaluateProximityToPolygon);
        return Library::Call< BOOL, UINT32, const ::POINT *, const TOUCH_HIT_TESTING_INPUT *, TOUCH_HIT_TESTING_PROXIMITY_EVALUATION * >(pFunc, numVertices, controlPolygon, pHitTestingInput, pProximityEval);
    }

    //The RECT form of the same evaluation; controlBoundingBox is a plain const RECT*, not an LPRECT.
    BOOL User32::EvaluateProximityToRect(const ::RECT *controlBoundingBox, const TOUCH_HIT_TESTING_INPUT *pHitTestingInput, TOUCH_HIT_TESTING_PROXIMITY_EVALUATION *pProximityEval)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EvaluateProximityToRect);
        return Library::Call< BOOL, const ::RECT *, const TOUCH_HIT_TESTING_INPUT *, TOUCH_HIT_TESTING_PROXIMITY_EVALUATION * >(pFunc, controlBoundingBox, pHitTestingInput, pProximityEval);
    }

    //PAR_STATE is itself a pointer typedef, which is why the SDK writes the out parameter with no star.
    BOOL User32::GetAutoRotationState(PAR_STATE pState)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetAutoRotationState);
        return Library::Call< BOOL, PAR_STATE >(pFunc, pState);
    }

    BOOL User32::GetCIMSSM(INPUT_MESSAGE_SOURCE *inputMessageSource)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetCIMSSM);
        return Library::Call< BOOL, INPUT_MESSAGE_SOURCE * >(pFunc, inputMessageSource);
    }

    //metadata is _Inout_: the format argument selects which metadata is asked for, and a FALSE return leaves it untouched.
    BOOL User32::GetClipboardMetadata(UINT format, PGETCLIPBMETADATA metadata)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetClipboardMetadata);
        return Library::Call< BOOL, UINT, PGETCLIPBMETADATA >(pFunc, format, metadata);
    }

    BOOL User32::GetCurrentInputMessageSource(INPUT_MESSAGE_SOURCE *inputMessageSource)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetCurrentInputMessageSource);
        return Library::Call< BOOL, INPUT_MESSAGE_SOURCE * >(pFunc, inputMessageSource);
    }

    //Zero-argument (the TU's only one) - matches desktop.cpp:107 GetProcessWindowStation's Library::Call< T >(pFunc)
    //shape; the banner covers why its DWORD is a value and not a success flag.
    DWORD User32::GetUnpredictedMessagePos(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetUnpredictedMessagePos);
        return Library::Call< DWORD >(pFunc);
    }

    //BOOL, with the affinity leaving through pdwAffinity rather than as the return.
    BOOL User32::GetWindowDisplayAffinity(HWND hWnd, DWORD* pdwAffinity)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetWindowDisplayAffinity);
        return Library::Call< BOOL, HWND, DWORD * >(pFunc, hWnd, pdwAffinity);
    }

    //pSize is _Inout_: it carries the caller's buffer capacity in and the used byte count out, and config is written
    //only up to what it reports - GetWindowFeedbackSetting's size argument is the same UINT32 the setter takes as a
    //plain _In_ UINT32, so the two halves of the pair spell the same slot differently, as the SDK has them.
    BOOL User32::GetWindowFeedbackSetting(HWND hwnd, FEEDBACK_TYPE feedback, DWORD dwFlags, UINT32* pSize, VOID* config)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetWindowFeedbackSetting);
        return Library::Call< BOOL, HWND, FEEDBACK_TYPE, DWORD, UINT32 *, VOID * >(pFunc, hwnd, feedback, dwFlags, pSize, config);
    }

    //hwndInherit is _In_opt_: NULL means "inherit the process's monitor", not "no monitor".
    BOOL User32::InheritWindowMonitor(HWND hwnd, HWND hwndInherit)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, InheritWindowMonitor);
        return Library::Call< BOOL, HWND, HWND >(pFunc, hwnd, hwndInherit);
    }

    //BOOL predicate over a process or thread handle; no out parameter, so FALSE is the only answer it can give.
    BOOL User32::IsImmersiveProcess(HANDLE hProcess)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsImmersiveProcess);
        return Library::Call< BOOL, HANDLE >(pFunc, hProcess);
    }

    //BOOL predicate over a window handle.
    BOOL User32::IsWindowArranged(HWND hwnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsWindowArranged);
        return Library::Call< BOOL, HWND >(pFunc, hwnd);
    }

    //tdFlags is the TOOLTIP_DISMISS_FLAGS enum group, not a raw DWORD.
    BOOL User32::RegisterForTooltipDismissNotification(HWND hWnd, TOOLTIP_DISMISS_FLAGS tdFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, RegisterForTooltipDismissNotification);
        return Library::Call< BOOL, HWND, TOOLTIP_DISMISS_FLAGS >(pFunc, hWnd, tdFlags);
    }

    //processHandleArray is _In_reads_(processHandleCount) - the count and the array length are the caller's contract.
    BOOL User32::SetAdditionalForegroundBoostProcesses(HWND topLevelWindow, DWORD processHandleCount, HANDLE *processHandleArray)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetAdditionalForegroundBoostProcesses);
        return Library::Call< BOOL, HWND, DWORD, HANDLE * >(pFunc, topLevelWindow, processHandleCount, processHandleArray);
    }

    //UINT_PTR - the timer id, and 0 is a failure a caller has to test; uToleranceDelay is the coalescing window.
    //lpTimerFunc is the TIMERPROC callback, the same slot shape as SetTimer above.
    UINT_PTR User32::SetCoalescableTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc, ULONG uToleranceDelay)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetCoalescableTimer);
        return Library::Call< UINT_PTR, HWND, UINT_PTR, UINT, TIMERPROC, ULONG >(pFunc, hWnd, nIDEvent, uElapse, lpTimerFunc, uToleranceDelay);
    }

    //VOID in the SDK, so voidCall and nothing to test - the level is set, and the answer is not returned.
    void User32::SetDebugErrorLevel(DWORD dwLevel)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetDebugErrorLevel);
        Library::voidCall< DWORD >(pFunc, dwLevel);
    }

    //BOOL; the exemption is process-wide and one-shot, so FALSE means it was refused, not that the argument was bad.
    BOOL User32::SetProcessRestrictionExemption(BOOL fEnableExemption)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetProcessRestrictionExemption);
        return Library::Call< BOOL, BOOL >(pFunc, fEnableExemption);
    }

    //UINT in, UINT out - the return is the scaling that took effect, not a success flag.
    UINT User32::SetThreadCursorCreationScaling(UINT cursorDpi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetThreadCursorCreationScaling);
        return Library::Call< UINT, UINT >(pFunc, cursorDpi);
    }

    //BOOL, and dwAffinity is one of the WINDOW_AFFINITY_* values.
    BOOL User32::SetWindowDisplayAffinity(HWND hWnd, DWORD dwAffinity)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetWindowDisplayAffinity);
        return Library::Call< BOOL, HWND, DWORD >(pFunc, hWnd, dwAffinity);
    }

    //The setter's size is a plain _In_ UINT32 and configuration is _In_reads_bytes_opt_(size). The SDK writes that
    //parameter CONST VOID*; the layer spells it const VOID*, the same denoted type, so this TU adds nothing to the
    //17 CONST sites U25 still carries.
    BOOL User32::SetWindowFeedbackSetting(HWND hwnd, FEEDBACK_TYPE feedback, DWORD dwFlags, UINT32 size, const VOID* configuration)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetWindowFeedbackSetting);
        return Library::Call< BOOL, HWND, FEEDBACK_TYPE, DWORD, UINT32, const VOID * >(pFunc, hwnd, feedback, dwFlags, size, configuration);
    }

}}}//qor::win::api
