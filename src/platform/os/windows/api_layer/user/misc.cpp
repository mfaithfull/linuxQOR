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
    //Return policy (U2, table row CalculatePopupWindowPosition / misc.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "Type: BOOL If the function succeeds, it returns TRUE ; otherwise, it returns
    //FALSE . To get extended error information, call GetLastError ." - FALSE is reserved for a failed call and the popup rectangle
    //itself is an out-parameter, so the BOOL carries no answer and the session-27 soundness test (a checked-failure site is only
    //sound where the SDK's failure value is not also a legal success value) passes at every value; sdkRetType BOOL so the base is
    //BOOL and BoolCheck (returncheck.h:94, Test at :101 is value ? true : false) is already in the library - no new check class.
    //0 in-tree callers for the exact stem (D14 published surface), so nothing built today reaches this site; G6 keeps the check
    //live on every call, so a failed call now raises a continuable issue the Win32ErrorHandler above reports rather than
    //travelling as a silent FALSE.
    BOOL User32::CalculatePopupWindowPosition(const ::POINT *anchorPoint, const ::SIZE *windowSize, UINT flags, ::RECT *excludeRect, ::RECT *popupWindowPosition)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CalculatePopupWindowPosition);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, const ::POINT *, const ::SIZE *, UINT, ::RECT *, ::RECT * >(pFunc, anchorPoint, windowSize, flags, excludeRect, popupWindowPosition);
        return bResult;
    }

    //One member for the A/W pair, per D13 and this library's T-suffix convention (the model is windowclass.cpp:76
    //GetClassLongPtrT / :79 qor_pp_useswinapiAW): the macro appends W or A to the stem according to qor_pp_unicode,
    //so the published parameter is the wide form and this build binds ChangeMenuW.
    //Return policy (U2, table row ChangeMenu / misc.cpp, 12 Nov 2026): the row reads unchecked with no checkClass and carries no
    //docQuote - the Learn page has no Return value section at all (probe status no-page, tag no-return-value-section), so the page
    //documents no failure value and D51 step 2 (an API that reserves no return value for errors) reads no-check; the row's AW binding
    //has no SDK prototype (sdkRetType NO-PROTOTYPE), so the row's only type is the published BOOL, and the page is the obsolete
    //Menu-style call whose answer lives in the parameters the caller passes. No token is landed; G6 makes any landed check live on
    //every call, so booking this row checked would change behaviour with no documented failure value to key on.
    BOOL User32::ChangeMenuT(HMENU hMenu, UINT cmd, LPCWSTR lpszNewItem, UINT cmdInsert, UINT flags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, ChangeMenu);
        return Library::Call< BOOL, HMENU, UINT, LPCWSTR, UINT, UINT >(pFunc, hMenu, cmd, lpszNewItem, cmdInsert, flags);
    }

    //BOOL as a real success flag; pChangeFilterStruct is _Inout_opt_ and reports what changed, so a caller that passes
    //NULL learns nothing about the filter that was already in place.
    //Return policy (U2, table row ChangeWindowMessageFilterEx / misc.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is the CalculatePopupWindowPosition row's status line word for word - "If the
    //function succeeds, it returns TRUE ; otherwise, it returns FALSE . To get extended error information, call GetLastError ." -
    //so FALSE is reserved for a failed call and the filter being changed is a parameter, the reported filter state leaving through
    //the _Inout_opt_ struct rather than the return; sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94) is
    //already in the library - no new check class. 0 in-tree callers for the exact stem (D14 published surface), so nothing built
    //today reaches this site; G6 keeps the check live on every call, so a failed call now raises a continuable issue the
    //Win32ErrorHandler above reports rather than travelling as a silent FALSE.
    BOOL User32::ChangeWindowMessageFilterEx(HWND hwnd, UINT message, DWORD action, PCHANGEFILTERSTRUCT pChangeFilterStruct)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, ChangeWindowMessageFilterEx);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, UINT, DWORD, PCHANGEFILTERSTRUCT >(pFunc, hwnd, message, action, pChangeFilterStruct);
        return bResult;
    }

    //BOOL, and the answer is pProximityEval; controlPolygon is _In_reads_(numVertices), so numVertices and the array
    //length are the caller's contract - the layer cannot see a short array.
    //Return policy (U2, table row EvaluateProximityToPolygon / misc.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote "If this function succeeds, it returns TRUE. Otherwise, it returns FALSE. To
    //retrieve extended error information, call the GetLastError function." is the RegisterTouchHitTestingWindow row's status line
    //word for word, which that leaf books checked; the page's own first line puts the answer elsewhere - "Returns the score of a
    //polygon as the probable touch target (compared to all other polygons that intersect the touch contact area) and an adjusted
    //touch point within the polygon" - and that score and adjusted point land in the TOUCH_HIT_TESTING_PROXIMITY_EVALUATION out
    //parameter, so a far-away polygon answers through the out-parameter while FALSE is a failed call; sdkRetType BOOL so the base
    //is BOOL and BoolCheck (returncheck.h:94) is already in the library - no new check class. 0 in-tree callers for the exact stem
    //(D14 published surface), so nothing built today reaches this site; G6 keeps the check live on every call, so a failed call now
    //raises a continuable issue the Win32ErrorHandler above reports rather than travelling as a silent FALSE.
    BOOL User32::EvaluateProximityToPolygon(UINT32 numVertices, const ::POINT *controlPolygon, const TOUCH_HIT_TESTING_INPUT *pHitTestingInput, TOUCH_HIT_TESTING_PROXIMITY_EVALUATION *pProximityEval)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EvaluateProximityToPolygon);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, UINT32, const ::POINT *, const TOUCH_HIT_TESTING_INPUT *, TOUCH_HIT_TESTING_PROXIMITY_EVALUATION * >(pFunc, numVertices, controlPolygon, pHitTestingInput, pProximityEval);
        return bResult;
    }

    //The RECT form of the same evaluation; controlBoundingBox is a plain const RECT*, not an LPRECT.
    //Return policy (U2, table row EvaluateProximityToRect / misc.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is the EvaluateProximityToPolygon row's status line word for word, so the same
    //reading applies - the rectangle is the input and the score plus adjusted point land in the same
    //TOUCH_HIT_TESTING_PROXIMITY_EVALUATION out parameter, so FALSE is reserved for a failed call and never for a far-away
    //rectangle; sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94) is already in the library - no new check
    //class. 0 in-tree callers for the exact stem (D14 published surface), so nothing built today reaches this site; G6 keeps the
    //check live on every call, so a failed call now raises a continuable issue the Win32ErrorHandler above reports rather than
    //travelling as a silent FALSE.
    BOOL User32::EvaluateProximityToRect(const ::RECT *controlBoundingBox, const TOUCH_HIT_TESTING_INPUT *pHitTestingInput, TOUCH_HIT_TESTING_PROXIMITY_EVALUATION *pProximityEval)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EvaluateProximityToRect);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, const ::RECT *, const TOUCH_HIT_TESTING_INPUT *, TOUCH_HIT_TESTING_PROXIMITY_EVALUATION * >(pFunc, controlBoundingBox, pHitTestingInput, pProximityEval);
        return bResult;
    }

    //PAR_STATE is itself a pointer typedef, which is why the SDK writes the out parameter with no star.
    //Return policy (U2, table row GetAutoRotationState / misc.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote "TRUE if the method succeeds, otherwise FALSE. See GetDisplayAutoRotationPreferences
    //for an example of using this function." is the whole Return value section - the same line the SetDisplayAutoRotationPreferences
    //row carries, where the answer is the GetDisplayAutoRotationPreferences out parameter - so FALSE is reserved for a failed call
    //and the rotation state itself leaves through the PAR_STATE out parameter; sdkRetType BOOL so the base is BOOL and BoolCheck
    //(returncheck.h:94) is already in the library - no new check class. 0 in-tree callers for the exact stem (D14 published
    //surface), so nothing built today reaches this site; G6 keeps the check live on every call, so a failed call now raises a
    //continuable issue the Win32ErrorHandler above reports rather than travelling as a silent FALSE.
    BOOL User32::GetAutoRotationState(PAR_STATE pState)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetAutoRotationState);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, PAR_STATE >(pFunc, pState);
        return bResult;
    }

    //Return policy (U2, table row GetCIMSSM / misc.cpp, 12 Nov 2026): the row reads unchecked with no checkClass. Its page does
    //reserve a failure value - docQuote "If this function succeeds, it returns TRUE. Otherwise, it returns ERROR_INVALID_PARAMETER.
    //This function fails when: The input parameter is invalid. GetCurrentInputMessageSource returns a value other than IMDT_UNAVAILABLE
    //for the device type." - but that failure value is an HRESULT the row cannot carry: the row's sdkRetType and pubRetType are both
    //BOOL, and no class in returncheck.h compares a BOOL row against an HRESULT sentinel (BoolCheck and CheckNonZero both read the
    //nonzero HRESULT as success), so no class fits this row and booking it checked here would land a check that cannot see the documented
    //failure; the fitting class belongs to the CheckClassVocabulary leaf rather than to this batch. The page also deprecates the call
    //("GetCIMSSM may be altered or unavailable in the future. Instead, use GetCurrentInputMessageSource"), whose row is checked. No
    //token is landed.
    BOOL User32::GetCIMSSM(INPUT_MESSAGE_SOURCE *inputMessageSource)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetCIMSSM);
        return Library::Call< BOOL, INPUT_MESSAGE_SOURCE * >(pFunc, inputMessageSource);
    }

    //metadata is _Inout_: the format argument selects which metadata is asked for, and a FALSE return leaves it untouched.
    //Return policy (U2, table row GetClipboardMetadata / misc.cpp, 12 Nov 2026): the row reads unchecked with no checkClass and carries
    //no docQuote - the page has no Return value section (probe status no-page, tag no-return-value-section), so nothing is reserved for
    //failure and D51 step 2 reads no-check; the metadata the call reports is the _Inout_ parameter, so keying the published BOOL on zero
    //would report the metadata state as a call failure. No token is landed.
    BOOL User32::GetClipboardMetadata(UINT format, PGETCLIPBMETADATA metadata)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetClipboardMetadata);
        return Library::Call< BOOL, UINT, PGETCLIPBMETADATA >(pFunc, format, metadata);
    }

    //Return policy (U2, table row GetCurrentInputMessageSource / misc.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote "If this function succeeds, it returns TRUE. Otherwise, it returns FALSE. To
    //retrieve extended error information, call the GetLastError function." is the EvaluateProximityToPolygon row's line for line;
    //the Parameters section puts the answer in the INPUT_MESSAGE_SOURCE the caller passes ("deviceType in INPUT_MESSAGE_SOURCE is
    //set to IMDT_UNAVAILABLE when SendMessage is used to inject input"), so an unavailable source is an out-parameter answer while
    //FALSE is a failed call; sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94) is already in the library - no
    //new check class. 0 in-tree callers for the exact stem (D14 published surface), so nothing built today reaches this site; G6
    //keeps the check live on every call, so a failed call now raises a continuable issue the Win32ErrorHandler above reports rather
    //than travelling as a silent FALSE.
    BOOL User32::GetCurrentInputMessageSource(INPUT_MESSAGE_SOURCE *inputMessageSource)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetCurrentInputMessageSource);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, INPUT_MESSAGE_SOURCE * >(pFunc, inputMessageSource);
        return bResult;
    }

    //Zero-argument (the TU's only one) - matches desktop.cpp:107 GetProcessWindowStation's Library::Call< T >(pFunc)
    //shape; the banner covers why its DWORD is a value and not a success flag.
    //Return policy (U2, table row GetUnpredictedMessagePos / misc.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - a
    //value answer with no documented failure value: docQuote "The screen location of the pointer input." is the whole Return value
    //section, so D51 step 2 reads no-check; the Remarks add only "By default, touch prediction is activated." and a screen position is
    //a packed value any of whose words may legitimately be 0. No token is landed.
    DWORD User32::GetUnpredictedMessagePos(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetUnpredictedMessagePos);
        return Library::Call< DWORD >(pFunc);
    }

    //BOOL, with the affinity leaving through pdwAffinity rather than as the return.
    //Return policy (U2, table row GetWindowDisplayAffinity / misc.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote "Type: BOOL This function succeeds only when the window is layered and Desktop
    //Windows Manager is composing the desktop. If this function succeeds, it returns TRUE ; otherwise, it returns FALSE . To get
    //extended error information, call GetLastError ." so FALSE is reserved for a failed call and the affinity itself leaves through
    //the pdwAffinity out parameter; the Remarks (the window content-protection feature note shared with the SetWindowDisplayAffinity
    //row) name no legal FALSE answer; sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94) is already in the library
    //- no new check class. 0 in-tree callers for the exact stem (D14 published surface), so nothing built today reaches this site;
    //G6 keeps the check live on every call, so a failed call now raises a continuable issue the Win32ErrorHandler above reports rather
    //than travelling as a silent FALSE.
    BOOL User32::GetWindowDisplayAffinity(HWND hWnd, DWORD* pdwAffinity)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetWindowDisplayAffinity);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, DWORD * >(pFunc, hWnd, pdwAffinity);
        return bResult;
    }

    //pSize is _Inout_: it carries the caller's buffer capacity in and the used byte count out, and config is written
    //only up to what it reports - GetWindowFeedbackSetting's size argument is the same UINT32 the setter takes as a
    //plain _In_ UINT32, so the two halves of the pair spell the same slot differently, as the SDK has them.
    //Return policy (U2, table row GetWindowFeedbackSetting / misc.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - a
    //question, not a flag: docQuote "Returns TRUE if the specified feedback setting is configured on the specified window. Otherwise,
    //it returns FALSE (and config won't be modified)." - the FALSE is the answer to the caller's question about a window's feedback
    //configuration, a window that does not set that FEEDBACK_TYPE being a legal state, so nothing is reserved and keying BoolCheck
    //would report a legal answer as a failure; same shape as the IsTouchWindow and AreDpiAwarenessContextsEqual rows the table books
    //unchecked, and the configuration itself is the [in, out] parameter. No token is landed.
    BOOL User32::GetWindowFeedbackSetting(HWND hwnd, FEEDBACK_TYPE feedback, DWORD dwFlags, UINT32* pSize, VOID* config)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetWindowFeedbackSetting);
        return Library::Call< BOOL, HWND, FEEDBACK_TYPE, DWORD, UINT32 *, VOID * >(pFunc, hwnd, feedback, dwFlags, pSize, config);
    }

    //hwndInherit is _In_opt_: NULL means "inherit the process's monitor", not "no monitor".
    //Return policy (U2, table row InheritWindowMonitor / misc.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote "True if the call is successful." is the whole Return value section and the page
    //has no Remarks; the monitor handle the window inherits leaves through the parameters rather than the return, so the BOOL carries
    //no answer and the session-27 soundness test passes at every value; the _In_opt_ NULL spelling an inherited monitor is an input,
    //not a return; sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94) is already in the library - no new check
    //class. 0 in-tree callers for the exact stem (D14 published surface), so nothing built today reaches this site; G6 keeps the
    //check live on every call, so a failed call now raises a continuable issue the Win32ErrorHandler above reports rather than
    //travelling as a silent FALSE.
    BOOL User32::InheritWindowMonitor(HWND hwnd, HWND hwndInherit)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, InheritWindowMonitor);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, HWND >(pFunc, hwnd, hwndInherit);
        return bResult;
    }

    //BOOL predicate over a process or thread handle; no out parameter, so FALSE is the only answer it can give.
    //Return policy (U2, table row IsImmersiveProcess / misc.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - the BOOL
    //is the answer the call's own name asks: the article's first line is "Determines whether the process belongs to a Windows Store
    //app" and the only parameter is the process handle, so a process that is not part of a Windows Store app is a legal state answering
    //zero; the page carries only the boilerplate docQuote "If the function succeeds, the return value is nonzero. If the function fails,
    //the return value is zero. To get extended error information, call GetLastError ." and has no Remarks section and names no failure
    //condition, so the boilerplate reserves nothing separable from the answer; same shape as the IsTouchWindow, IsWow64Message and
    //IsValidDpiAwarenessContext rows the table books unchecked, and keying BoolCheck here would report a legal non-Store process as a
    //failure. No token is landed - deliberately unlike the SetProcessRestrictionExemption row below, which shares this boilerplate but
    //is a Set* whose subject is its parameter and so does get the token.
    BOOL User32::IsImmersiveProcess(HANDLE hProcess)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsImmersiveProcess);
        return Library::Call< BOOL, HANDLE >(pFunc, hProcess);
    }

    //BOOL predicate over a window handle.
    //Return policy (U2, table row IsWindowArranged / misc.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - answer-shaped:
    //docQuote "A nonzero value if the window is arranged; otherwise, zero." is the whole Return value section, and zero answers the
    //caller's question about the window's arrangement (the page's own example asks it, combining IsIconic, IsZoomed and IsWindowArranged
    //in one condition and returning false when any of them holds), so nothing is reserved; the Remarks also state the call has no header
    //or library file ("At this time, this function does not have an associated header file or library file. Your application can call
    //LoadLibrary with the DLL name ( User32.dll ) to obtain a module handle."), and the page is flagged prerelease. No token is landed.
    BOOL User32::IsWindowArranged(HWND hwnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsWindowArranged);
        return Library::Call< BOOL, HWND >(pFunc, hwnd);
    }

    //tdFlags is the TOOLTIP_DISMISS_FLAGS enum group, not a raw DWORD.
    //Return policy (U2, table row RegisterForTooltipDismissNotification / misc.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the Return value line's "(See Remarks.)" pointer is load-bearing: docQuote "TRUE if the window was successfully
    //registered or unregistered; otherwise, FALSE . (See Remarks.)" and the Remarks put FALSE at legal registration-state answers -
    //"If an app or framework calls RegisterForTooltipDismissNotification with TDF_UNREGISTER after the window has been automatically
    //unregistered, the function returns FALSE . There is no impact on future registrations.", "the function returns TRUE if the window
    //was successfully registered or FALSE if the window was already registered. The window is treated as registered either way", and
    //"or FALSE if the windows was not currently registered. The window is treated as unregistered either way" - so FALSE reports a state
    //that is already where the caller wants it, a routine outcome, and keying BoolCheck would report a legal answer as a failure. No
    //token is landed.
    BOOL User32::RegisterForTooltipDismissNotification(HWND hWnd, TOOLTIP_DISMISS_FLAGS tdFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, RegisterForTooltipDismissNotification);
        return Library::Call< BOOL, HWND, TOOLTIP_DISMISS_FLAGS >(pFunc, hWnd, tdFlags);
    }

    //processHandleArray is _In_reads_(processHandleCount) - the count and the array length are the caller's contract.
    //Return policy (U2, table row SetAdditionalForegroundBoostProcesses / misc.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote "Returns TRUE if the call succeeds in boosting the application, FALSE otherwise.
    //SetAdditionalForegroundBoostProcesses sets the last error code, so the application can call GetLastError() to obtain extended
    //information if the call failed (for example, ERROR_INVALID_PARAMETER, ERROR_NOT_ENOUGH_MEMORY, or ERROR_ACCESS_DENIED)." - the
    //extended-error route is named inside the Return value section itself; the process-handle array and the top-level HWND are
    //parameters, so the BOOL carries no answer, and the long Remarks name no legal FALSE; sdkRetType BOOL so the base is BOOL and
    //BoolCheck (returncheck.h:94) is already in the library - no new check class. 0 in-tree callers for the exact stem (D14 published
    //surface), so nothing built today reaches this site; G6 keeps the check live on every call, so a failed call now raises a
    //continuable issue the Win32ErrorHandler above reports rather than travelling as a silent FALSE.
    BOOL User32::SetAdditionalForegroundBoostProcesses(HWND topLevelWindow, DWORD processHandleCount, HANDLE *processHandleArray)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetAdditionalForegroundBoostProcesses);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, DWORD, HANDLE * >(pFunc, topLevelWindow, processHandleCount, processHandleArray);
        return bResult;
    }

    //UINT_PTR - the timer id, and 0 is a failure a caller has to test; uToleranceDelay is the coalescing window.
    //lpTimerFunc is the TIMERPROC callback, the same slot shape as SetTimer above.
    //Return policy (U2, table row SetCoalescableTimer / misc.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZero< UINT_PTR >, and the table's verbatim docQuote "Type: UINT_PTR If the function succeeds and the hWnd parameter is
    //NULL , the return value is an integer identifying the new timer. An application can pass this value to the KillTimer function to
    //destroy the timer. If the function succeeds and the hWnd parameter is not NULL , then the return value is a nonzero integer. An
    //application can pass the value of the nIDEvent parameter to the KillTimer function to destroy the timer. If the function fails to
    //create a timer, the return value is zero. To get extended error information, call GetLastError ." so
    //zero is reserved and a timer identifier, which the caller hands straight to KillTimer, is never legitimately zero; the row's
    //sdkRetType column is blank and its pubRetType is UINT_PTR, so the class base is the published UINT_PTR and the template
    //CheckNonZero (returncheck.h:187, template< typename TNumeric > : public AutoErrorCheck< TNumeric >) covers it - no new check
    //class. 0 in-tree callers for the exact stem (D14 published surface), so nothing built today reaches this site; G6 keeps the
    //check live on every call, so a failed call now raises a continuable issue the Win32ErrorHandler above reports rather than
    //travelling as a silent 0 the caller would have to notice.
    UINT_PTR User32::SetCoalescableTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc, ULONG uToleranceDelay)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetCoalescableTimer);
        CheckReturn< UINT_PTR, CheckNonZero< UINT_PTR > >::TType uResult = Library::Call< UINT_PTR, HWND, UINT_PTR, UINT, TIMERPROC, ULONG >(pFunc, hWnd, nIDEvent, uElapse, lpTimerFunc, uToleranceDelay);
        return uResult;
    }

    //VOID in the SDK, so voidCall and nothing to test - the level is set, and the answer is not returned.
    void User32::SetDebugErrorLevel(DWORD dwLevel)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetDebugErrorLevel);
        Library::voidCall< DWORD >(pFunc, dwLevel);
    }

    //BOOL; the exemption is process-wide and one-shot, so FALSE means it was refused, not that the argument was bad.
    //Return policy (U2, table row SetProcessRestrictionExemption / misc.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote "If the function succeeds, the return value is nonzero. If the function fails, the
    //return value is zero. To get extended error information, call GetLastError ." is word-for-word the IsImmersiveProcess row's
    //boilerplate - but this call is a Set* whose only subject is its parameter, so the BOOL carries no answer of its own and zero is
    //reserved; deliberately unlike that row, which is unchecked because its BOOL is the answer to the question the name asks; sdkRetType
    //BOOL so the base is BOOL and BoolCheck (returncheck.h:94) is already in the library - no new check class. 0 in-tree callers for
    //the exact stem (D14 published surface), so nothing built today reaches this site; G6 keeps the check live on every call, so a
    //refused exemption now raises a continuable issue the Win32ErrorHandler above reports rather than travelling as a silent FALSE.
    BOOL User32::SetProcessRestrictionExemption(BOOL fEnableExemption)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetProcessRestrictionExemption);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, BOOL >(pFunc, fEnableExemption);
        return bResult;
    }

    //UINT in, UINT out - the return is the scaling that took effect, not a success flag.
    //Return policy (U2, table row SetThreadCursorCreationScaling / misc.cpp, 12 Nov 2026): the row reads unchecked with no checkClass - a
    //value answer with no documented failure value: docQuote "The previous value set for the thread before calling this API." is the
    //whole Return value section, so D51 step 2 reads no-check; the previous scaling for a thread that never called this API is 0, a
    //legal answer, so keying CheckNonZero< UINT > here would report a legal first call as a failure; sdkRetType UINT. No token is landed.
    UINT User32::SetThreadCursorCreationScaling(UINT cursorDpi)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetThreadCursorCreationScaling);
        return Library::Call< UINT, UINT >(pFunc, cursorDpi);
    }

    //BOOL, and dwAffinity is one of the WINDOW_AFFINITY_* values.
    //Return policy (U2, table row SetWindowDisplayAffinity / misc.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote "Type: BOOL If the function succeeds, it returns TRUE ; otherwise, it returns FALSE
    //when, for example, the function call is made on a non top-level window. To get extended error information, call GetLastError ."
    //so FALSE is reserved and the affinity being set is a parameter; the Remarks (shared with the GetWindowDisplayAffinity row) add
    //only the content-protection background and "Setting the display affinity to WDA_EXCLUDEFROMCAPTURE on previous version of
    //Windows will behave as if WDA_MONITOR is applied.", which names no legal FALSE; sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94) is already in the library - no
    //new check class. 0 in-tree callers for the exact stem (D14 published surface), so nothing built today reaches this site; G6 keeps
    //the check live on every call, so a call made on a non top-level window now raises a continuable issue the Win32ErrorHandler above
    //reports rather than travelling as a silent FALSE.
    BOOL User32::SetWindowDisplayAffinity(HWND hWnd, DWORD dwAffinity)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetWindowDisplayAffinity);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, DWORD >(pFunc, hWnd, dwAffinity);
        return bResult;
    }

    //The setter's size is a plain _In_ UINT32 and configuration is _In_reads_bytes_opt_(size). The SDK writes that
    //parameter CONST VOID*; the layer spells it const VOID*, the same denoted type, so this TU adds nothing to the
    //17 CONST sites U25 still carries.
    //Return policy (U2, table row SetWindowFeedbackSetting / misc.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote "Returns TRUE if successful; otherwise, returns FALSE." is the whole Return value
    //section and the page has no Remarks - the same status line the AddClipboardFormatListener and RemoveClipboardFormatListener rows
    //take BoolCheck for; the feedback setting being configured is a parameter, so the BOOL carries no answer - deliberately unlike the
    //GetWindowFeedbackSetting row, whose FALSE is the answer to the caller's question; sdkRetType BOOL so the base is BOOL and BoolCheck
    //(returncheck.h:94) is already in the library - no new check class. 0 in-tree callers for the exact stem (D14 published surface), so
    //nothing built today reaches this site; G6 keeps the check live on every call, so a failed setting now raises a continuable issue
    //the Win32ErrorHandler above reports rather than travelling as a silent FALSE.
    BOOL User32::SetWindowFeedbackSetting(HWND hwnd, FEEDBACK_TYPE feedback, DWORD dwFlags, UINT32 size, const VOID* configuration)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetWindowFeedbackSetting);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, FEEDBACK_TYPE, DWORD, UINT32, const VOID * >(pFunc, hwnd, feedback, dwFlags, size, configuration);
        return bResult;
    }

}}}//qor::win::api
