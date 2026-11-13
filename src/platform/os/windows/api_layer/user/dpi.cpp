// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "../kernel/errorhandler.h"

//=== api_layer conventions ========================================= Reviewed 6 Nov 2026
//   (T8 step 6, tranche 6b: the DPI / awareness family - 21 wrappers, new TU)
//
//This TU is coverage added under D18 (docs/agent/DECISIONS.md): purpose.md states the library intends a full set of
//wrappers over the user32.dll exports, and registers/u18-coverage.md buckets the gap by family. The DPI / awareness
//bucket was counted at 21 and it reconciles with nothing shed: all 21 are in the user32.dll export capture AND declared
//in winuser.h 10.0.26100.0, and none of the 21 is a function-like macro or has an A/W sibling, so 21 in -> 21 wrapped,
//0 excluded. That is the cleanest tranche so far: 6a's Pointer bucket had to shed 9 prototype-less names plus 1
//declared-but-not-exported name.
//
//Signature rule (D18): the published signature IS the winuser.h prototype, parameter names included - 21/21 here copy it
//exactly. SDK declaration lines (each verified by tmp/_tmp_t8_6b_guards.ps1, which also dumps the guard stack open at
//that line): 5340, 5347, 5363, 5370 - the dialog DPI-change quartet; 7418, 13280 - the two DPI-parameterised twins of
//already-declared stems; 14462, 14468, 14474, 14480, 14486, 14492, 14499, 14511, 14517, 14523 - the awareness-context
//block; then 14539, 14550, 14560, 14566, 14571.
//
//Types: DPI_AWARENESS_CONTEXT is a DECLARE_HANDLE at shared/windef.h:256, DPI_AWARENESS an enum at :258, DPI_HOSTING_
//BEHAVIOR an enum at :271, and the SDK's five awareness levels are cast handles at :265-269 - NOT an enum, which is
//why AreDpiAwarenessContextsEqual and IsValidDpiAwarenessContext exist and are the only legal way to test a context.
//The two dialog flag enums open at winuser.h:5328 (DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS) and :5350
//(DIALOG_DPI_CHANGE_BEHAVIORS), each followed by DEFINE_ENUM_FLAG_OPERATORS (:5335, :5356), so a caller combining
//flags needs the SDK operators. Every one of those types reaches this TU through kernel32.h:12's #include <Windows.h> -
//neither dpi.cpp nor user32.h declares a type of its own for the family.
//
//Return conventions - stated here rather than coded (D15 / D18 pass-through), with six U2 landings this session: the
//table rows EnableNonClientDpiScaling / GetWindowDpiAwarenessContext / SetDialogControlDpiChangeBehavior /
//SetDialogDpiChangeBehavior / SetProcessDpiAwarenessContext / SystemParametersInfoForDpi / dpi.cpp read checked, so
//those six sites now carry CheckReturn and the other 15 stay pass-through for the reasons at each site. The row's own
//docQuote and the session-27 soundness test behind it are at the site, and the dated policy comments below carry the
//reason for each unchecked row.
//The dialog Get* members return the ENUM, not BOOL; the dialog Set* members take a
//mask/values pair and a bit outside mask is ignored by the OS, not reported; SetThreadDpiAwarenessContext and
//SetThreadDpiHostingBehavior return the PREVIOUS context / behavior, so a caller that changes one must keep what came
//back and restore it - the layer keeps no scope guard; SetProcessDpiAwarenessContext returns BOOL and is one-shot, so
//FALSE means "too late", not "bad argument"; GetThreadDpiAwarenessContext is documented never to return NULL;
//GetDpiFromDpiAwarenessContext returns 0 for an invalid context; GetSystemMetricsForDpi returns 0 on failure, which is
//also a legal metric value, so the return alone cannot decide success; SystemParametersInfoForDpi's pvParam is
//_Pre_maybenull_ _Post_valid_ with a type and size fixed by uiAction, which the layer does not and cannot validate.
//
//Guarding: 13 of the 21 prototypes have #if(WINVER >= 0x0605) as their innermost guard - 10 inside the block opening
//:14458 (the awareness-context group), plus SetProcessDpiAwarenessContext under its own :14535, plus
//GetSystemMetricsForDpi (:7414, inside #ifndef NOSYSMETRICS at :7242) and SystemParametersInfoForDpi (:13275, inside
//#ifndef NOSYSPARAMSINFO at :12433). Three - the hosting trio - have #if(WINVER >= 0x0606) at :14555.
//GetDpiAwarenessContextForProcess is the only backlog name whose innermost guard is NTDDI_VERSION rather than WINVER
//(:14545). The four dialog members have NO WINVER test at all, only #ifndef NOCTLMGR (:4912) plus
//WINAPI_PARTITION_DESKTOP (:5006). WINVER defaults to 0x0A00 and NTDDI_VERSION to WDK_NTDDI_VERSION = NTDDI_WIN11_GE
//(sdkddkver.h:166, :236-242) and nothing in this build sets either, so all 21 are live and the layer adds no #if - but
//a caller that pins NTDDI_VERSION below NTDDI_WIN10_19H1 loses the GetDpiAwarenessContextForProcess prototype while
//this header still declares it.
//
//Binding (D13): 21 qor_pp_useswinapi sites, all bare-stem, 21 qor_pp_fcontext, 21/21 Library::Call with explicit
//template arguments (0 voidCall - there is no VOID entry point in this family), 0 mismatched stems in
//tmp/_tmp_wrong_stem_scan.ps1. No name here is a macro and none has an A/W family, so no #undef is needed at
//user32.h:9-17; SystemParametersInfoForDpi is a separate name and is NOT rewritten by the SystemParametersInfo macro
//that forces the #undef at :11.
//
//Verified by the compiler, not by eye: tmp/_tmp_t8_6b_protos.cpp declares 21 function-pointer typedefs equal to the 21
//published signatures and initialises each one with &<SDK name>. Compiled at /W4 with no QOR define and no version macro
//of its own, exit 0 - which proves both halves at once: every declaration is live under the build's defaults (a
//compiled-out name would be undeclared) and every published signature matches the prototype (a mismatched function-
//pointer initialiser is C2440).
//
//Consumers: 0 LAYER sites, 1 live BYPASS - ui/gl/windowimpl.cpp:516 calls GetSystemMetricsForDpi through the SDK import
//while the line above it already routes through the layer (WindowController::GetDPI -> GetDpiForWindow) - and 1 BYPASS
//hit that is not a call at all: ui/gl/windowimpl.cpp:550 sits inside the block comment that opens at :530 and closes at
//:1301, so it is dead text. winqor_gl is not in the build (ui/CMakeLists.txt:4 "#add_subdirectory(gl)"), which is why
//neither site links. U19 is FROZEN at session 34, so this split is booked in this banner and not in the register.
//
//Two pre-flag figures corrected by this session's measurement: the preflags cited the dialog enums at winuser.h:5332 /
//:5355, which are the CLOSING lines - the typedefs open at :5328 / :5350 - and DPI_AWARENESS at windef.h:259, which is
//the first enumerator (the typedef is :258). The preflags also said the two ForDpi twins' stems "are wrapped": those
//stems are only DECLARED (user32.h:712, :714 after this TU's header insert) and defined by no TU, which is U4, so the
//"place the twins beside their stems" reading of U12 does not apply - all 21 went into one new "//DPI awareness
//functions" section after "//Pointer input functions", the way 6a placed its family. The 26-line insert moves every
//later self-citation in user32.h down by 26 (GetDpiForWindow :675 -> :701), which is the T7 citation-shift rule.
//windowimpl.cpp also has four bare GetDpiForWindow sites (:361, :1130, :1212, :1462), and every one of them is inside a
//block comment - 325-408, 530-1301, 530-1301, 1303-1511 - so there is no live bypass of that already-wrapped name.
//
//Gate: this TU standalone at /W4 exit 0, then the folder 40/40 exit 0 with no diagnostics, both re-run after this
//banner (dated by tmp/obj/dpi.obj being newer than the source). Because user32.h changed, the all-libraries run was
//captured before and after and read as a diff (tmp/_tmp_t8_6b_apialldiff.ps1): 107 -> 108 .cpp echoes, 165 warning
//lines both sides, the one pre-existing C1083 at opengl/wgl.cpp:31 both sides, and a set-difference of diagnostics of
//0 new / 0 gone - the only content difference between the two logs is the added "dpi.cpp" echo. cl /EP on this TU
//yields 21 binding literals, all with module literal "user32", each byte-equal to an export name, 0 duplicates.

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //DPI_AWARENESS_CONTEXT is a DECLARE_HANDLE (windef.h:256), not an enum - the SDK's own awareness levels are
    //cast handles (DPI_AWARENESS_CONTEXT_UNAWARE .. _UNAWARE_GDISCALED, windef.h:265-269). AreDpiAwarenessContextsEqual
    //exists because those handles cannot be compared with ==; the call is the only legal comparison.
    //Return policy (U2, table row AreDpiAwarenessContextsEqual / dpi.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "Returns TRUE if the values are equal, otherwise FALSE ." - the
    //FALSE is the answer to the caller's equality question about two opaque context handles, not a failure value, and
    //the page names no failure value at all, so the session-27 soundness test fails at that value; same shape as the
    //IsClipboardFormatAvailable row in clipboard.cpp. The comparison result comes back exactly as the SDK produced it.
    BOOL User32::AreDpiAwarenessContextsEqual(DPI_AWARENESS_CONTEXT dpiContextA, DPI_AWARENESS_CONTEXT dpiContextB)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, AreDpiAwarenessContextsEqual);
        return Library::Call< BOOL, DPI_AWARENESS_CONTEXT, DPI_AWARENESS_CONTEXT >(pFunc, dpiContextA, dpiContextB);
    }

    //EnableNonClientDpiScaling is only meaningful for a per-monitor-DPI-aware window and must be called before the
    //window is sized; the hwnd stays the caller's to create and destroy.
    //Return policy (U2, table row EnableNonClientDpiScaling / dpi.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "If the function succeeds, the return value is nonzero. If the
    //function fails, the return value is zero. To get extended error information, call GetLastError ." - zero is reserved
    //and this member answers nothing besides whether scaling was enabled for hwnd, so the session-27 soundness test
    //(a checked-failure site is only sound where the SDK's failure value is not also a legal success value) passes at
    //every value; BOOL + zero-means-failure takes the BoolCheck the library already has (returncheck.h:94, Test at :101
    //is value ? true : false) - no new check class. sdkRetType BOOL so the base is BOOL. 0 in-tree callers for the exact
    //stem (D14 published surface), so nothing built today reaches this site; G6 keeps the check live on every call, so
    //a failed enable now raises a continuable issue the Win32ErrorHandler above reports instead of travelling as a
    //silent success (status.md U7).
    BOOL User32::EnableNonClientDpiScaling(HWND hwnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EnableNonClientDpiScaling);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND >(pFunc, hwnd);
        return bResult;
    }

    //Returns the DPI_AWARENESS enum, i.e. which awareness level the opaque context maps to.
    //Return policy (U2, table row GetAwarenessFromDpiAwarenessContext / dpi.cpp, 12 Nov 2026): the row reads unchecked
    //with no checkClass, and the table's verbatim docQuote is "The DPI_AWARENESS . If the provided value is null or
    //invalid, this method will return DPI_AWARENESS_INVALID ." - the return is the awareness level the caller asked for,
    //and the only sentinel the page names is what an invalid *parameter* answers; DPI_AWARENESS_INVALID is also the
    //library's no-awareness value, so a legal answer collides with the tested value, and the page has no "if the
    //function fails" clause at all, so D51 step 2 reads no-check too.
    DPI_AWARENESS User32::GetAwarenessFromDpiAwarenessContext(DPI_AWARENESS_CONTEXT value)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetAwarenessFromDpiAwarenessContext);
        return Library::Call< DPI_AWARENESS, DPI_AWARENESS_CONTEXT >(pFunc, value);
    }

    //The dialog members take a mask/values pair and return the enum, not BOOL: values must be a subset of mask, and a
    //bit outside the mask is ignored by the OS rather than reported. The two return types are
    //DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS (winuser.h:5328) and DIALOG_DPI_CHANGE_BEHAVIORS (:5350), both
    //DEFINE_ENUM_FLAG_OPERATORS enums that reach this TU through kernel32.h:12's #include <Windows.h>.
    //Return policy (U2, table row GetDialogControlDpiChangeBehavior / dpi.cpp, 12 Nov 2026): the row reads unchecked with
    //no checkClass, and the table's verbatim docQuote is "The flags set on the given window. If passed an invalid handle,
    //this function will return zero, and set its last error to ERROR_INVALID_HANDLE ." - zero is the no-flags state of a
    //window, a legal answer, and the documented zero is what an invalid handle parameter answers rather than a failed
    //call; the distinction lives in GetLastError, which the layer never consults (0 GetLastError pass-through), so the
    //session-27 soundness test fails at that value.
    DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS User32::GetDialogControlDpiChangeBehavior(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetDialogControlDpiChangeBehavior);
        return Library::Call< DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS, HWND >(pFunc, hWnd);
    }

    //Return policy (U2, table row GetDialogDpiChangeBehavior / dpi.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "The flags set on the given dialog. If passed an invalid handle,
    //this function will return zero, and set its last error to ERROR_INVALID_HANDLE ." - that row's sentence with dialog
    //and window swapped, so zero is again the no-flags answer rather than a reserved failure value and the session-27
    //test fails at it.
    DIALOG_DPI_CHANGE_BEHAVIORS User32::GetDialogDpiChangeBehavior(HWND hDlg)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetDialogDpiChangeBehavior);
        return Library::Call< DIALOG_DPI_CHANGE_BEHAVIORS, HWND >(pFunc, hDlg);
    }

    //The only name in this TU whose SDK prototype is behind an NTDDI_VERSION test rather than WINVER
    //(#if (NTDDI_VERSION >= NTDDI_WIN10_19H1) at winuser.h:14545); live under the sdkddkver.h default, and the layer
    //adds no #if, exactly as it does not for the WINVER floors. hProcess is a handle the caller opens and closes.
    //Return policy (U2, table row GetDpiAwarenessContextForProcess / dpi.cpp, 12 Nov 2026): the row reads unchecked with
    //no checkClass, and the table's verbatim docQuote is "The DPI_AWARENESS_CONTEXT for the specified process." - that is
    //the whole Return value section, so the SDK reserves nothing for a failed call and D51 step 2 (an API that reserves
    //no return value for errors) reads no-check; keying on NULL would invent a failure value the page never states.
    DPI_AWARENESS_CONTEXT User32::GetDpiAwarenessContextForProcess(HANDLE hProcess)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetDpiAwarenessContextForProcess);
        return Library::Call< DPI_AWARENESS_CONTEXT, HANDLE >(pFunc, hProcess);
    }

    //Return policy (U2, table row GetDpiForSystem / dpi.cpp, 12 Nov 2026): the row reads unchecked with no checkClass,
    //and the table's verbatim docQuote is "The system DPI value." - the whole Return value section names no failure
    //value, so D51 step 2 reads no-check.
    UINT User32::GetDpiForSystem(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetDpiForSystem);
        return Library::Call< UINT >(pFunc);
    }

    //The DPI encoded in a context handle, or 0 for an invalid one.
    //Return policy (U2, table row GetDpiFromDpiAwarenessContext / dpi.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "The DPI value associated with the DPI_AWARENESS_CONTEXT handle."
    // - the whole Return value section names no failure value; the 0 case this banner notes at :40 is an invalid context,
    //i.e. a parameter rather than a returned answer, so the session-27 soundness test has no reserved value to pass at.
    UINT User32::GetDpiFromDpiAwarenessContext(DPI_AWARENESS_CONTEXT value)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetDpiFromDpiAwarenessContext);
        return Library::Call< UINT, DPI_AWARENESS_CONTEXT >(pFunc, value);
    }

    //Return policy (U2, table row GetSystemDpiForProcess / dpi.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "The process's system DPI value." - no failure value is documented,
    //so D51 step 2 reads no-check, the same shape as the GetDpiForSystem row.
    UINT User32::GetSystemDpiForProcess(HANDLE hProcess)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetSystemDpiForProcess);
        return Library::Call< UINT, HANDLE >(pFunc, hProcess);
    }

    //The DPI-parameterised twin of the wrapped GetSystemMetrics stem (user32.h:712 - declared, never defined here).
    //Returns 0 on failure, which is also a legal metric value, so the return alone cannot decide success; the SDK
    //sets last error. nIndex and dpi are the caller's to validate.
    //Return policy (U2, table row GetSystemMetricsForDpi / dpi.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is the generic status template "If the function succeeds, the return
    //value is nonzero. If the function fails, the return value is zero. To get extended error information, call
    //GetLastError ." - but the value this member answers is a metric and 0 is a legal metric, which is what this banner
    //states at :40, so keying a CheckNonZero on the documented zero would report a legal zero-size metric as a failure
    //and the session-27 soundness test fails at that value; same zero-is-legal shape as the CountClipboardFormats count
    //row. 0 in-tree callers reach the wrapper; the 1 live BYPASS at ui/gl/windowimpl.cpp:516 calls the SDK import
    //directly and is unaffected either way.
    int User32::GetSystemMetricsForDpi(int nIndex, UINT dpi)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetSystemMetricsForDpi);
        return Library::Call< int, int, UINT >(pFunc, nIndex, dpi);
    }

    //Get* returns the current thread context, which the SDK documents as never NULL; SetThreadDpiAwarenessContext
    //below returns the PREVIOUS context, so a caller that changes it must keep the returned handle and restore it -
    //the layer keeps no scope guard, that contract stays with the caller.
    //Return policy (U2, table row GetThreadDpiAwarenessContext / dpi.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "The current DPI_AWARENESS_CONTEXT for the thread." - that is the
    //whole Return value section, and the page's Remarks add that a thread with no earlier SetThreadDpiAwarenessContext
    //call answers the process default context, so every value here is a legal answer and D51 step 2 (an API that
    //reserves no return value for errors) reads no-check.
    DPI_AWARENESS_CONTEXT User32::GetThreadDpiAwarenessContext(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetThreadDpiAwarenessContext);
        return Library::Call< DPI_AWARENESS_CONTEXT >(pFunc);
    }

    //Return policy (U2, table row GetThreadDpiHostingBehavior / dpi.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "The DPI_HOSTING_BEHAVIOR of the current thread." - the whole
    //Return value section names no failure value, and the page's Remarks say a thread with no earlier call answers
    //DPI_HOSTING_BEHAVIOR_DEFAULT, a legal member of the enum, so D51 step 2 reads no-check.
    DPI_HOSTING_BEHAVIOR User32::GetThreadDpiHostingBehavior(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetThreadDpiHostingBehavior);
        return Library::Call< DPI_HOSTING_BEHAVIOR >(pFunc);
    }

    //Return policy (U2, table row GetWindowDpiAwarenessContext / dpi.cpp, 12 Nov 2026): the row reads checked with
    //checkClass CheckNonZeroHandle< DPI_AWARENESS_CONTEXT >, and the table's verbatim docQuote is "The DPI_AWARENESS_CONTEXT
    //for the provided window. If the window is not valid, the return value is NULL ." - a valid window always answers a
    //context handle (DPI_AWARENESS_CONTEXT is a DECLARE_HANDLE, and the preprocessed SDK header declares this member as
    //returning DPI_AWARENESS_CONTEXT), so NULL is never a legal success value at this stem and the session-27 soundness
    //test passes at that value; unlike the GetClipboardOwner / GetCursor / SetCapture rows, whose NULL names a legal
    //empty state, this NULL is the only value a call that could not do its job answers. The class base is the SDK's
    //declared DPI_AWARENESS_CONTEXT and CheckNonZeroHandle (returncheck.h:136, Test at :144 is (value == 0x00000000) ?
    //false : true) is the handle template the vocabulary already offers - no new check class. 0 in-tree callers for the
    //exact stem (D14 published surface); G6 keeps the check live on every call, so a NULL now raises a continuable issue
    //the Win32ErrorHandler above reports rather than travelling as a null context handle (status.md U7).
    DPI_AWARENESS_CONTEXT User32::GetWindowDpiAwarenessContext(HWND hwnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetWindowDpiAwarenessContext);
        CheckReturn< DPI_AWARENESS_CONTEXT, CheckNonZeroHandle< DPI_AWARENESS_CONTEXT > >::TType hResult = Library::Call< DPI_AWARENESS_CONTEXT, HWND >(pFunc, hwnd);
        return hResult;
    }

    //Return policy (U2, table row GetWindowDpiHostingBehavior / dpi.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "The DPI_HOSTING_BEHAVIOR of the specified window." - the whole
    //Return value section names no failure value, and the page's Remarks say a window's hosting behaviour is permanent
    //and cannot be changed after creation, so the member only ever answers a legal enum member and D51 step 2 reads
    //no-check.
    DPI_HOSTING_BEHAVIOR User32::GetWindowDpiHostingBehavior(HWND hwnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetWindowDpiHostingBehavior);
        return Library::Call< DPI_HOSTING_BEHAVIOR, HWND >(pFunc, hwnd);
    }

    //The validity test for the opaque handles - the SDK's own answer to "is this one of the sentinel contexts?".
    //Return policy (U2, table row IsValidDpiAwarenessContext / dpi.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "TRUE if the provided context is supported, otherwise FALSE ." -
    //this member answers the caller's question about a context it already holds, and the page's Remarks apply the FALSE
    //answer to an input NULL, so keying a check on FALSE would report a legal answer as a failure and the session-27
    //soundness test fails at that value; same shape as the AreDpiAwarenessContextsEqual row in this TU and the
    //IsClipboardFormatAvailable row in clipboard.cpp. The verdict comes back exactly as the SDK produced it.
    BOOL User32::IsValidDpiAwarenessContext(DPI_AWARENESS_CONTEXT value)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsValidDpiAwarenessContext);
        return Library::Call< BOOL, DPI_AWARENESS_CONTEXT >(pFunc, value);
    }

    //Return policy (U2, table row SetDialogControlDpiChangeBehavior / dpi.cpp, 12 Nov 2026): the row reads checked with
    //checkClass BoolCheck, and the table's verbatim docQuote is "This function returns TRUE if the operation was
    //successful, and FALSE otherwise. To get extended error information, call GetLastError . Possible errors are
    //ERROR_INVALID_HANDLE if passed an invalid HWND, and ERROR_ACCESS_DENIED if the windows belongs to another
    //process." - FALSE is reserved and the mask/values pair are parameters rather than the return, so this member
    //answers nothing besides whether the override took, and the session-27 soundness test passes at every value;
    //sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94, Test at :101 is value ? true : false) is the
    //class the library already has - no new check class. 0 in-tree callers for the exact stem (D14 published surface);
    //G6 keeps the check live on every call, so a failed override now raises a continuable issue the Win32ErrorHandler
    //above reports instead of travelling as a silent success (status.md U7).
    BOOL User32::SetDialogControlDpiChangeBehavior(HWND hWnd, DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS mask, DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS values)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetDialogControlDpiChangeBehavior);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS, DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS >(pFunc, hWnd, mask, values);
        return bResult;
    }

    //Return policy (U2, table row SetDialogDpiChangeBehavior / dpi.cpp, 12 Nov 2026): the row reads checked with
    //checkClass BoolCheck, and the table's verbatim docQuote is "This function returns TRUE if the operation was
    //successful, and FALSE otherwise. To get extended error information, call GetLastError . Possible errors are
    //ERROR_INVALID_HANDLE if passed an invalid dialog HWND, and ERROR_ACCESS_DENIED if the dialog belongs to another
    //process." - that row's sentence with dialog for control, so FALSE is reserved, the mask/values pair are parameters
    //rather than the return, and the session-27 soundness test passes at every value; this row takes the same BoolCheck
    //(returncheck.h:94) as its dialog pair. 0 in-tree callers for the exact stem (D14 published surface); G6 keeps the
    //check live on every call, so a failed override now raises a continuable issue the Win32ErrorHandler above reports
    //instead of travelling as a silent success (status.md U7).
    BOOL User32::SetDialogDpiChangeBehavior(HWND hDlg, DIALOG_DPI_CHANGE_BEHAVIORS mask, DIALOG_DPI_CHANGE_BEHAVIORS values)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetDialogDpiChangeBehavior);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, DIALOG_DPI_CHANGE_BEHAVIORS, DIALOG_DPI_CHANGE_BEHAVIORS >(pFunc, hDlg, mask, values);
        return bResult;
    }

    //One-shot: the SDK lets the process context be set once, before DPI awareness is otherwise fixed, and returns
    //FALSE (not the previous value, unlike the thread setter) once that window has passed.
    //Return policy (U2, table row SetProcessDpiAwarenessContext / dpi.cpp, 12 Nov 2026): the row reads checked with
    //checkClass BoolCheck, and the table's verbatim docQuote is "This function returns TRUE if the operation was
    //successful, and FALSE otherwise. To get extended error information, call GetLastError . Possible errors are
    //ERROR_INVALID_PARAMETER for an invalid input, and ERROR_ACCESS_DENIED if the default API awareness mode for the
    //process has already been set (via a previous API call or within the application manifest)." - FALSE is reserved,
    //the context being set is a parameter rather than the return, and the page calls a late call a failure ("Once API
    //awareness is set for an app, any future calls to this API will fail"), so the session-27 soundness test passes at
    //every value; sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94) is the class the library already
    //has - no new check class. 0 in-tree callers for the exact stem (D14 published surface); G6 keeps the check live on
    //every call, so a too-late set now raises a continuable issue the Win32ErrorHandler above reports rather than
    //travelling as a silent success (status.md U7).
    BOOL User32::SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT value)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetProcessDpiAwarenessContext);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, DPI_AWARENESS_CONTEXT >(pFunc, value);
        return bResult;
    }

    //Return policy (U2, table row SetThreadDpiAwarenessContext / dpi.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "The old DPI_AWARENESS_CONTEXT for the thread. If the dpiContext
    //is invalid, the thread will not be updated and the return value will be NULL . You can use this value to restore
    //the old DPI_AWARENESS_CONTEXT after overriding it with a predefined value." - the return is the state the caller
    //must keep and restore (D51's previous-state shape, the same one the ShowWindow / SetScrollPos / FlashWindow and
    //SetCursor / SetCapture rows carry), and the documented NULL is what an invalid *parameter* answers rather than a
    //failed call, so the session-27 soundness test fails at that value; the previous context comes back exactly as the
    //OS wrote it.
    DPI_AWARENESS_CONTEXT User32::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT dpiContext)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetThreadDpiAwarenessContext);
        return Library::Call< DPI_AWARENESS_CONTEXT, DPI_AWARENESS_CONTEXT >(pFunc, dpiContext);
    }

    //Returns the previous hosting behavior, as the thread awareness setter does. DPI_HOSTING_BEHAVIOR is the enum at
    //windef.h:271, and the SDK floor for all three hosting names is WINVER >= 0x0606.
    //Return policy (U2, table row SetThreadDpiHostingBehavior / dpi.cpp, 12 Nov 2026): the row reads unchecked with no
    //checkClass, and the table's verbatim docQuote is "The previous DPI_HOSTING_BEHAVIOR for the thread. If the hosting
    //behavior passed in is invalid, the thread will not be updated and the return value will be DPI_HOSTING_BEHAVIOR_INVALID
    //." - the return is the previous behaviour the caller restores (D51's previous-state shape again), and the sentinel
    //is an enumerator of the member's own return type (the preprocessed SDK header opens DPI_HOSTING_BEHAVIOR with
    //DPI_HOSTING_BEHAVIOR_INVALID = -1, DPI_HOSTING_BEHAVIOR_DEFAULT = 0, DPI_HOSTING_BEHAVIOR_MIXED = 1), so a legal
    //value of the return type collides with the tested value and the session-27 soundness test fails at it.
    DPI_HOSTING_BEHAVIOR User32::SetThreadDpiHostingBehavior(DPI_HOSTING_BEHAVIOR value)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetThreadDpiHostingBehavior);
        return Library::Call< DPI_HOSTING_BEHAVIOR, DPI_HOSTING_BEHAVIOR >(pFunc, value);
    }

    //The DPI-parameterised twin of the wrapped SystemParametersInfo stem (user32.h:714 - declared, never defined
    //here). pvParam is _Pre_maybenull_ _Post_valid_: its type and size are fixed by uiAction, which the layer does not
    //and cannot validate. fWinIni carries the SPI_SETWININI flags plus the send-change flag - a flag set passed as
    //UINT, exactly as the SDK declares it.
    //Return policy (U2, table row SystemParametersInfoForDpi / dpi.cpp, 12 Nov 2026): the row reads checked with
    //checkClass BoolCheck, and the table's verbatim docQuote is "If the function succeeds, the return value is nonzero.
    //If the function fails, the return value is zero. To get extended error information, call GetLastError ." - zero is
    //reserved there and the answer the member carries goes to the pvParam out parameter rather than to the return, so
    //the return says only whether the query ran and the session-27 soundness test passes at every value; the page's
    //Remarks name the ForDPI gap ("Other possible uiAction values do not provide ForDPI behavior, and therefore this
    //function returns 0 if called with them"), so that zero is a failure rather than a legal answer. sdkRetType BOOL so
    //is BOOL and BoolCheck (returncheck.h:94, Test at :101 is value ? true : false) is the class the library already
    //has - no new check class. 0 in-tree callers for the exact stem (D14 published surface); G6 keeps the check live on
    //every call, so a failed query now raises a continuable issue the Win32ErrorHandler above reports instead of
    //travelling as a silent success (status.md U7).
    BOOL User32::SystemParametersInfoForDpi(UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni, UINT dpi)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SystemParametersInfoForDpi);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, UINT, UINT, PVOID, UINT, UINT >(pFunc, uiAction, uiParam, pvParam, fWinIni, dpi);
        return bResult;
    }

}}}//qor::win::api
