// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "../kernel/errorhandler.h"

//=== api_layer conventions ========================================= Reviewed 6 Nov 2026
//   (T8 step 6, tranche 6c: the Touch / gesture family - 14 wrappers, new TU)
//
//This TU is coverage added under D18 (docs/agent/DECISIONS.md): purpose.md states the library intends a full set of
//wrappers over the user32.dll exports, and registers/u18-coverage.md buckets the gap by family. The Touch / gesture
//bucket measures 14 and it reconciles with nothing shed: 19 export names match the bucket filter, of which 4 were
//already wrapped in pointer.cpp with the 6a Pointer tranche (GetPointerTouchInfo :6574, GetPointerTouchInfoHistory
//:6581, GetPointerFrameTouchInfo :6589, GetPointerFrameTouchInfoHistory :6597 - they are POINTER_TOUCH_INFO members,
//so a name-based filter finds them a second time), 1 is exported with no prototype in winuser.h at all
//(MITSynthesizeTouchInput, 0 hits - out under the D18 documented-set rule) and the remaining 14 are here. All 14 are
//exported AND declared, and the pre-scan (tmp/_tmp_t8_6c_pre.ps1 over the 62-name gap list written by
//tmp/_tmp_u18_coverage4.ps1) measures 0 function-like macros, 0 A/W siblings and 0 already declared in user32.h for
//every one of them: 14 in -> 14 wrapped, 0 excluded, nothing shed.
//
//Signature rule (D18): the published signature IS the winuser.h prototype, parameter names included - 14/14 here copy
//it exactly. SDK declaration lines: 6243, 6252 (the WM_TOUCH handle pair), 6272, 6279, 6285 (window registration),
//6485, 6492 (injection), 6726, 6773 (hit testing), 15770, 15783, 15801 (gesture retrieval and handle close) and
//15869, 15884 (gesture configuration). Export ordinals in the folder's capture: 51, 52, 15C, 15D, 15E, 1D4, 214, 21B,
//245, 2A5, 2F9, 2FA, 33F, 3CE.
//
//Two pre-flag prototype claims were wrong, and the published text was copied from winuser.h itself rather than from
//the digest: (a) the digest truncates GetTouchInputInfo (:6243), SetGestureConfig (:15869) and GetGestureConfig
//(:15884) - those three prototypes are multi-line and their inline parameter comments contain ';', which is exactly
//where the digest's end-of-prototype scan stops; and (b) the pre-flags gave GetGestureConfig's count parameter as
//_Inout_ PUINT, whereas the SDK has _In_ PUINT pcIDs (:15888) and puts the in/out on the buffer instead
//(_Inout_updates_(*pcIDs) PGESTURECONFIG, :15890) - so the published parameter type is PUINT.
//
//Liveness and signatures were COMPILED, not eyeballed: tmp/_tmp_t8_6c_protos.cpp declares 14 function-pointer typedefs
//equal to the 14 published signatures and initialises each one with &<SDK name>. At /W4 with no QOR define and no
//version macro of its own it compiles clean (tmp/_tmp_t8_6c_protos.log, exit 0) - a declaration compiled out by a
//guard would be undeclared and a mismatched signature would be C2440, so that one run settles both questions.
//
//Types: every one of them reaches this TU through kernel32.h:12's #include <Windows.h>, and neither touch.cpp nor
//user32.h declares a type of its own for the family. HTOUCHINPUT is a DECLARE_HANDLE at winuser.h:6193, TOUCHINPUT
//:6195-6206, PCTOUCHINPUT :6207; HGESTUREINFO a DECLARE_HANDLE at :15691, GESTUREINFO :15726-15736, GESTURECONFIG
//:15812-15816; TOUCH_HIT_TESTING_PROXIMITY_EVALUATION :6730-6734 and TOUCH_HIT_TESTING_INPUT :6740-6747; and
//InjectTouchInput reuses POINTER_TOUCH_INFO (:6392), the type pointer.cpp already wraps accessors for.
//
//The handle contract is the one thing a caller can get wrong here, so it is stated, not coded: HTOUCHINPUT and
//HGESTUREINFO are created by the OS and not by this library - a window receives them in the lParam of WM_TOUCH and
//WM_GESTURE - and CloseTouchInputHandle / CloseGestureInfoHandle close exactly those. winuser.h:15788-15796 is
//explicit that closing the handle is the application's responsibility once it is done with it (skipping the close
//leaks process memory), but that a handle passed on to DefWindowProc or forwarded through PostMessage / SendMessage
//has transferred and must NOT be closed. The layer therefore keeps no scope guard on either handle, exactly as
//dpi.cpp keeps none for the SetThreadDpiAwarenessContext / SetThreadDpiHostingBehavior previous-value pair.
//
//Return conventions, kept as the SDK has them and stated here rather than coded (D15 / D18 pass-through - 0
//CheckReturn in this TU): 13 of the 14 return BOOL as a success flag whose buffer or handle side effect the layer
//neither sees nor validates, and the 14th, PackTouchHitTestingProximityEvaluation, returns LRESULT (:6770-6772) - the
//packed value a WM_TOUCHHITTESTING handler returns, not a success flag, and the one a CheckReturn-shaped reflex would
//get wrong. GetTouchInputInfo's cInputs / pInputs / cbSize is the caller's buffer contract, cbSize being
//sizeof(TOUCHINPUT); the same contract governs SetGestureConfig (cIDs with a pGestureConfig array) and
//GetGestureConfig, whose *pcIDs carries the buffer size in and through which the SDK does not undertake to return a
//count, despite the buffer being _Inout_updates_(*pcIDs). GESTURECONFIGMAXCOUNT (256, :15860) caps both gesture-config
//calls. IsTouchWindow's pulFlags is _Out_opt_, so nullptr is a legal argument. RegisterTouchWindow's ulFlags is the
//TWF_FINETOUCH / TWF_WANTPALM pair (:6263-6264) and RegisterTouchHitTestingWindow's value is one of
//TOUCH_HIT_TESTING_DEFAULT / _CLIENT / _NONE (:6719-6721). InitializeTouchInjection is per-process, must precede the
//first InjectTouchInput and requires the injected-input privilege, which nothing here grants.
//
//Guarding: 5 names have #if(WINVER >= 0x0601) as their innermost guard inside #ifndef NOUSER (:6243, :6252, :6272,
//:6279, :6285), 4 have #if(WINVER >= 0x0602) inside NOUSER (:6485, :6492, :6726, :6773), and the 5 gesture names have
//#if(WINVER >= 0x0601) with NO NOUSER test at all. All 14 additionally sit inside
//#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP). WINVER defaults to 0x0A00 (sdkddkver.h) and nothing in this
//build sets it, so all 14 are live and the layer adds no #if - but a caller that pins WINVER below 0x0602 loses the
//injection / hit-testing quartet from the SDK while this header still declares it.
//
//Binding (D13): 14 qor_pp_useswinapi sites, all bare-stem, 14 qor_pp_fcontext, 14/14 Library::Call with explicit
//template arguments, 0 voidCall (there is no VOID entry point in this family), 0 Win32ErrorHandler (U1 stands
//folder-wide) and 0 mismatched stems in tmp/_tmp_wrong_stem_scan.ps1. No name here is a macro and none has an A/W
//family, so user32.h:9-17 gains no new #undef - RegisterTouchWindow and IsTouchWindow have no W twin, unlike
//ChangeMenu in the Misc bucket.
//
//Consumers: 0 LAYER and 0 BYPASS - 0 raw hits for all 14 names over the 3,228 .h/.hpp/.cpp/.cxx of src, examples and
//test (tmp/_tmp_t8_6c_consumers.ps1), so there is no U19 split to book here; U19 is FROZEN at session 34 in any case.
//
//Placement (U12): a new "//Touch and gesture functions" section in user32.h after "//DPI awareness functions" (which
//opens :603 and ends :627), the 14 declarations in the same order as the 14 definitions below. No stem in this family
//is already declared, so nothing pulls a declaration beside a wrapped sibling - the section question resolves by
//placement, as it did for 6a (:569) and 6b (:603). The 19-line header insert moves every later self-citation in
//user32.h down by 19 (the T7 citation-shift rule).
//
//Gate: this TU standalone at /W4 exit 0, then the folder 41/41 exit 0 with no diagnostics, both re-run after this
//banner and dated by tmp/obj/touch.obj being newer than the source. cl /EP on this TU yields 14 binding literals, all
//with module literal "user32", each byte-equal to an export name, 0 duplicates (digest tmp/_tmp_t8_6c_ppclass.txt over
//the capture tmp/_tmp_t8_6c_pp_touch.txt, which is never opened). Because user32.h changed, the all-libraries run was
//captured before and after and read as a DIFF rather than as an exit code: 108 -> 109 .cpp echoes, 165 warning lines
//both sides, the one pre-existing fatal C1083 at opengl/wgl.cpp:31 both sides, exit 2 both sides, and a diagnostics
//set-difference of 0 new / 0 gone - the only content difference between the two logs is the added "touch.cpp" echo.
//
//Folder after this tranche: 41 sources / 594 members / 497 bindings, counted comment-stripped
//(tmp/_tmp_t8_6c_counts.ps1) because TU banners spell out the binding macros in prose. Coverage re-measure
//(tmp/_tmp_u18_coverage4.ps1 + tmp/_tmp_u18_buckets.ps1): 717 of the 1,018 export names covered, 48
//declared-and-unwrapped raw, 40 net backlog (Display config 7, Shutdown/power 3, Misc 24, held-open legacy 6) and the
//buckets instrument prints "0: Touch / gesture family []", i.e. the family is drained with nothing left unaccounted.

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {
    //Closes the HGESTUREINFO a window received in the lParam of WM_GESTURE (winuser.h:15788-15796): once this window
    //is done with the handle the close is its responsibility and skipping it leaks process memory, but a handle that
    //was passed to DefWindowProc or forwarded with PostMessage / SendMessage has transferred and must NOT be closed.
    //The layer holds no guard for it - the message loop, not the wrapper, owns that decision.
    //Return policy (U2, table row CloseGestureInfoHandle / touch.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended error information,
    //use the GetLastError function." - the handle being closed is the only argument and the GESTUREINFO payload never travels through this return,
    //so the BOOL carries no answer and zero is reserved; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. The table reports 0 in-tree
    //callers for this row, so no in-tree site reads the old FALSE return today; the check is live (G6) on every call from anywhere.
    BOOL User32::CloseGestureInfoHandle(HGESTUREINFO hGestureInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CloseGestureInfoHandle);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HGESTUREINFO >(pFunc, hGestureInfo);
        return bResult;
    }

    //The WM_TOUCH counterpart of the close above: hTouchInput comes from WM_TOUCH lParam and nothing in this library
    //creates it. FALSE rather than a raised Win32Error is the SDK's answer for an invalid handle (U1, no
    //Win32ErrorHandler at these call sites).
    //Return policy (U2, table row CloseTouchInputHandle / touch.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended error information,
    //use the GetLastError function." - the HTOUCHINPUT being closed is the only argument and the payload the window reads comes from WM_TOUCH lParam,
    //not from this return, so the BOOL carries no answer and zero is reserved; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. The table
    //reports 0 in-tree callers for this row, so no in-tree site reads the old FALSE return today; the check is live (G6) on every call from anywhere.
    BOOL User32::CloseTouchInputHandle(HTOUCHINPUT hTouchInput)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CloseTouchInputHandle);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HTOUCHINPUT >(pFunc, hTouchInput);
        return bResult;
    }

    //dwReserved must be 0 and dwFlags is the GCF_* set (GCF_INCLUDE_ANCESTORS, :15878). The SDK declares the count
    //parameter _In_ PUINT pcIDs (:15888) - it reads the buffer size from *pcIDs and puts the in/out on the buffer
    //itself (_Inout_updates_(*pcIDs), :15890) - so the published parameter stays PUINT and the buffer contract,
    //including the GESTURECONFIGMAXCOUNT (256) cap, remains the caller's. cbSize is sizeof(GESTURECONFIG).
    //Return policy (U2, table row GetGestureConfig / touch.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended error information,
    //use the GetLastError function." - the configuration the call reports lands in the PGESTURECONFIG buffer the caller sizes and in *pcIDs, so this
    //BOOL carries no answer of its own and zero is reserved for a failed call; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. The table
    //reports 0 in-tree callers for this row, so no in-tree site reads the old FALSE return today; the check is live (G6) on every call from anywhere.
    BOOL User32::GetGestureConfig(HWND hwnd, DWORD dwReserved, DWORD dwFlags, PUINT pcIDs, PGESTURECONFIG pGestureConfig, UINT cbSize)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetGestureConfig);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, DWORD, DWORD, PUINT, PGESTURECONFIG, UINT >(pFunc, hwnd, dwReserved, dwFlags, pcIDs, pGestureConfig, cbSize);
        return bResult;
    }

    //cbExtraArgs is the cbExtraArgs field of the GESTUREINFO filled by GetGestureInfo, and pExtraArgs must be a
    //buffer of exactly that many bytes - the layer does not know the size and cannot check it.
    //Return policy (U2, table row GetGestureExtraArgs / touch.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended error information,
    //use the GetLastError function." - the extra arguments the call reports land in the pExtraArgs buffer the caller sized from the GESTUREINFO's cbExtraArgs,
    //so this BOOL carries no answer and zero is reserved; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. The table
    //reports 0 in-tree callers for this row, so no in-tree site reads the old FALSE return today; the check is live (G6) on every call from anywhere.
    BOOL User32::GetGestureExtraArgs(HGESTUREINFO hGestureInfo, UINT cbExtraArgs, PBYTE pExtraArgs)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetGestureExtraArgs);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HGESTUREINFO, UINT, PBYTE >(pFunc, hGestureInfo, cbExtraArgs, pExtraArgs);
        return bResult;
    }

    //Turns the WM_GESTURE lParam handle into a GESTUREINFO. FALSE means the handle was not valid, and the handle is
    //still the caller's to close (or to let travel with the message) - see CloseGestureInfoHandle above.
    //Return policy (U2, table row GetGestureInfo / touch.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended error information,
    //use the GetLastError function." - the GESTUREINFO the call reads out of the handle lands in the pGestureInfo the caller supplies, so this BOOL
    //carries no answer and zero is reserved for a failed call (an invalid handle); sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. The table
    //reports 0 in-tree callers for this row, so no in-tree site reads the old FALSE return today; the check is live (G6) on every call from anywhere.
    BOOL User32::GetGestureInfo(HGESTUREINFO hGestureInfo, PGESTUREINFO pGestureInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetGestureInfo);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HGESTUREINFO, PGESTUREINFO >(pFunc, hGestureInfo, pGestureInfo);
        return bResult;
    }

    //The WM_TOUCH payload. cInputs is the count the window is expecting (normally from the message's wParam) and
    //pInputs must be an array of cInputs TOUCHINPUT structures with cbSize == sizeof(TOUCHINPUT); the SDK writes the
    //array, the caller sizes it, and dwMask / TOUCHINPUTMASKF_* decide which fields are meaningful afterwards.
    //Return policy (U2, table row GetTouchInputInfo / touch.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended error information,
    //use the GetLastError function." - the TOUCHINPUT array the call writes goes into the pInputs buffer the caller sized with cInputs and cbSize,
    //so this BOOL carries no answer and zero is reserved; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. The table
    //reports 0 in-tree callers for this row, so no in-tree site reads the old FALSE return today; the check is live (G6) on every call from anywhere.
    BOOL User32::GetTouchInputInfo(HTOUCHINPUT hTouchInput, UINT cInputs, PTOUCHINPUT pInputs, int cbSize)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetTouchInputInfo);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HTOUCHINPUT, UINT, PTOUCHINPUT, int >(pFunc, hTouchInput, cInputs, pInputs, cbSize);
        return bResult;
    }

    //Injection is per-process and is expected once, before the first InjectTouchInput, with maxCount the largest
    //contact array the caller will ever inject. It needs the injected-input privilege, which this layer does not and
    //cannot grant - FALSE is the documented answer for both "already initialised" and "not permitted".
    //Return policy (U2, table row InitializeTouchInjection / touch.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "If the function succeeds, the return value is TRUE. If the function fails, the return value is FALSE. To get extended error information,
    //call GetLastError ." (this page words the pair TRUE / FALSE rather than the nonzero / zero form the other rows here carry) - the call's only
    //subject is its parameters, so the BOOL carries no answer, and both conditions the comment above names (already initialised, and not permitted
    //without the injected-input privilege) are reasons the page routes to GetLastError rather than answers the caller reads back; sdkRetType BOOL
    //so BoolCheck (returncheck.h:94) covers it. The table reports 0 in-tree callers for this row, so no in-tree site reads the old FALSE return today;
    //the check is live (G6) on every call from anywhere.
    BOOL User32::InitializeTouchInjection(UINT32 maxCount, DWORD dwMode)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, InitializeTouchInjection);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, UINT32, DWORD >(pFunc, maxCount, dwMode);
        return bResult;
    }

    //The array element type is POINTER_TOUCH_INFO, the same structure pointer.cpp wraps the GetPointerTouchInfo*
    //accessors for - this is where the 6a and 6c families meet, and it is why the Touch-named pointer members were
    //not wrapped twice. count is the caller's, and the SDK reads (does not write) the array.
    //Return policy (U2, table row InjectTouchInput / touch.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "If the function succeeds, the return value is non-zero. If the function fails, the return value is zero. To get extended error information,
    //call GetLastError ." (the page hyphenates non-zero here) - the contacts the call injects are the const array the caller passes, which the SDK
    //only reads, so the BOOL carries no answer and zero is reserved for a failed injection; sdkRetType BOOL so BoolCheck (returncheck.h:94)
    //covers it. The table reports 0 in-tree callers for this row, so no in-tree site reads the old FALSE return today; the check is live (G6) on every call.
    BOOL User32::InjectTouchInput(UINT32 count, const POINTER_TOUCH_INFO *contacts)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, InjectTouchInput);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, UINT32, const POINTER_TOUCH_INFO * >(pFunc, count, contacts);
        return bResult;
    }


    //pulFlags is _Out_opt_, so passing nullptr is legal and asks only the yes/no question. The value written is the
    //TWF_* flag set the window was registered with.
    BOOL User32::IsTouchWindow(HWND hwnd, PULONG pulFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsTouchWindow);
        return Library::Call< BOOL, HWND, PULONG >(pFunc, hwnd, pulFlags);
    }

    //The only non-BOOL return in the family: LRESULT, because the value is what a WM_TOUCHHITTESTING handler returns
    //to the hit-tester, not a success flag - no CheckReturn-shaped reading of it is valid. Its inputs are normally
    //produced by EvaluateProximityToRect / EvaluateProximityToPolygon (winuser.h:6756, :6764), which are still
    //unwrapped and sit in the Misc bucket (tranche 6f), so this wrapper is only useful with those written by hand.
    LRESULT User32::PackTouchHitTestingProximityEvaluation(const TOUCH_HIT_TESTING_INPUT *pHitTestingInput, const TOUCH_HIT_TESTING_PROXIMITY_EVALUATION *pProximityEval)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, PackTouchHitTestingProximityEvaluation);
        return Library::Call< LRESULT, const TOUCH_HIT_TESTING_INPUT *, const TOUCH_HIT_TESTING_PROXIMITY_EVALUATION * >(pFunc, pHitTestingInput, pProximityEval);
    }

    //value is one of TOUCH_HIT_TESTING_DEFAULT / TOUCH_HIT_TESTING_CLIENT / TOUCH_HIT_TESTING_NONE (:6719-6721) and
    //decides whether the OS or this window hit-tests touch; it is a ULONG, not a flag set, so combining values is not
    //meaningful. WINVER >= 0x0602 name.
    //Return policy (U2, table row RegisterTouchHitTestingWindow / touch.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "If this function succeeds, it returns TRUE. Otherwise, it returns FALSE. To retrieve extended error information, call the GetLastError function."
    // (this page words the pair TRUE / FALSE rather than the nonzero / zero form the other rows here carry) - the window and its hit-test value are
    //arguments and the effective TOUCH_HIT_TESTING_* choice is read back from the window, not through this return, so the BOOL carries no answer and
    //FALSE is reserved for a failed call; the page has no Remarks section and documents no legal FALSE. sdkRetType BOOL so BoolCheck (returncheck.h:94)
    //covers it. The table reports 0 in-tree callers for this row, so no in-tree site reads the old FALSE return today; the check is live (G6) on every call.
    BOOL User32::RegisterTouchHitTestingWindow(HWND hwnd, ULONG value)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, RegisterTouchHitTestingWindow);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, ULONG >(pFunc, hwnd, value);
        return bResult;
    }

    //opts the window into WM_TOUCH; ulFlags is the TWF_FINETOUCH / TWF_WANTPALM pair (:6263-6264). Registering does
    //not make the window pointer-enabled - the 6a RegisterPointerInputTarget pair is a separate contract, and the
    //hwnd is the caller's to create and destroy, as it is for every wrapper in this library.
    //Return policy (U2, table row RegisterTouchWindow / touch.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended error
    //information, use the GetLastError function." - the hwnd and its TWF_* flags are arguments, so the BOOL carries no answer and zero is reserved;
    //the Remarks make the call mandatory on every touch window ("RegisterTouchWindow must be called on every window that will be used for touch
    //input"), so a FALSE is a registration that did not happen rather than an answer the caller reads back. sdkRetType BOOL so BoolCheck
    //(returncheck.h:94) covers it. The table reports 0 in-tree callers for this row, so no in-tree site reads the old FALSE return today; the check
    //is live (G6) on every call from anywhere.
    BOOL User32::RegisterTouchWindow(HWND hwnd, ULONG ulFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, RegisterTouchWindow);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, ULONG >(pFunc, hwnd, ulFlags);
        return bResult;
    }

    //dwReserved must be 0. cIDs is the length of the pGestureConfig array (cap GESTURECONFIGMAXCOUNT, :15860) and
    //cbSize sizeof(GESTURECONFIG); dwWant / dwBlock take the GCF_* flags per dwID, and anything neither wanted nor
    //blocked falls back to the parent window's preferences or the system defaults (:15808-15810).
    //Return policy (U2, table row SetGestureConfig / touch.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended error
    //information, use the GetLastError function." - the gesture configuration is read in from the pGestureConfig array the caller supplies and
    //never travels through this return, so the BOOL carries no answer and zero is reserved; the Remarks only time the call ("call SetGestureConfig
    //at window creation time ... in response to WM_GESTURENOTIFY messages"), they name no legal zero. sdkRetType BOOL so BoolCheck
    //(returncheck.h:94) covers it. The table reports 0 in-tree callers for this row, so no in-tree site reads the old FALSE return today; the check
    //is live (G6) on every call from anywhere.
    BOOL User32::SetGestureConfig(HWND hwnd, DWORD dwReserved, UINT cIDs, PGESTURECONFIG pGestureConfig, UINT cbSize)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetGestureConfig);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, DWORD, UINT, PGESTURECONFIG, UINT >(pFunc, hwnd, dwReserved, cIDs, pGestureConfig, cbSize);
        return bResult;
    }

    //Reverses RegisterTouchWindow for the same hwnd; the WM_TOUCH handle contract is unaffected by it.
    //Return policy (U2, table row UnregisterTouchWindow / touch.cpp, 12 Nov 2026): checked with checkClass BoolCheck; the page's verbatim
    //docQuote is "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended error
    //information, use the GetLastError function." - the hwnd is an argument, so the BOOL carries no answer and zero is reserved. The Remarks make
    //clear that an unregistered window is NOT reported as a FALSE ("The UnregisterTouchWindow function succeeds even if the specified window was
    //not previously registered as being touch-capable."), so only a bad hwnd fails and no legal outcome is mistaken for a failure. sdkRetType BOOL
    //so BoolCheck (returncheck.h:94) covers it. The table reports 0 in-tree callers for this row, so no in-tree site reads the old FALSE return today;
    //the check is live (G6) on every call from anywhere.
    BOOL User32::UnregisterTouchWindow(HWND hwnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, UnregisterTouchWindow);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND >(pFunc, hwnd);
        return bResult;
    }

}}}//qor::win::api

