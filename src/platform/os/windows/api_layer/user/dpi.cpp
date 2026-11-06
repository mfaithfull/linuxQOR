// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

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
//Return conventions, all kept as the SDK has them and stated here rather than coded (D15 / D18 pass-through - no
//CheckReturn appears in this TU): the dialog Get* members return the ENUM, not BOOL; the dialog Set* members take a
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
#include "../library.h"

namespace qor { namespace win { namespace api {

    //DPI_AWARENESS_CONTEXT is a DECLARE_HANDLE (windef.h:256), not an enum - the SDK's own awareness levels are
    //cast handles (DPI_AWARENESS_CONTEXT_UNAWARE .. _UNAWARE_GDISCALED, windef.h:265-269). AreDpiAwarenessContextsEqual
    //exists because those handles cannot be compared with ==; the call is the only legal comparison.
    BOOL User32::AreDpiAwarenessContextsEqual(DPI_AWARENESS_CONTEXT dpiContextA, DPI_AWARENESS_CONTEXT dpiContextB)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, AreDpiAwarenessContextsEqual);
        return Library::Call< BOOL, DPI_AWARENESS_CONTEXT, DPI_AWARENESS_CONTEXT >(pFunc, dpiContextA, dpiContextB);
    }

    //EnableNonClientDpiScaling is only meaningful for a per-monitor-DPI-aware window and must be called before the
    //window is sized; the hwnd stays the caller's to create and destroy.
    BOOL User32::EnableNonClientDpiScaling(HWND hwnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EnableNonClientDpiScaling);
        return Library::Call< BOOL, HWND >(pFunc, hwnd);
    }

    //Returns the DPI_AWARENESS enum, i.e. which awareness level the opaque context maps to.
    DPI_AWARENESS User32::GetAwarenessFromDpiAwarenessContext(DPI_AWARENESS_CONTEXT value)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetAwarenessFromDpiAwarenessContext);
        return Library::Call< DPI_AWARENESS, DPI_AWARENESS_CONTEXT >(pFunc, value);
    }

    //The dialog members take a mask/values pair and return the enum, not BOOL: values must be a subset of mask, and a
    //bit outside the mask is ignored by the OS rather than reported. The two return types are
    //DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS (winuser.h:5328) and DIALOG_DPI_CHANGE_BEHAVIORS (:5350), both
    //DEFINE_ENUM_FLAG_OPERATORS enums that reach this TU through kernel32.h:12's #include <Windows.h>.
    DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS User32::GetDialogControlDpiChangeBehavior(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDialogControlDpiChangeBehavior);
        return Library::Call< DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS, HWND >(pFunc, hWnd);
    }

    DIALOG_DPI_CHANGE_BEHAVIORS User32::GetDialogDpiChangeBehavior(HWND hDlg)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDialogDpiChangeBehavior);
        return Library::Call< DIALOG_DPI_CHANGE_BEHAVIORS, HWND >(pFunc, hDlg);
    }

    //The only name in this TU whose SDK prototype is behind an NTDDI_VERSION test rather than WINVER
    //(#if (NTDDI_VERSION >= NTDDI_WIN10_19H1) at winuser.h:14545); live under the sdkddkver.h default, and the layer
    //adds no #if, exactly as it does not for the WINVER floors. hProcess is a handle the caller opens and closes.
    DPI_AWARENESS_CONTEXT User32::GetDpiAwarenessContextForProcess(HANDLE hProcess)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDpiAwarenessContextForProcess);
        return Library::Call< DPI_AWARENESS_CONTEXT, HANDLE >(pFunc, hProcess);
    }

    UINT User32::GetDpiForSystem(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDpiForSystem);
        return Library::Call< UINT >(pFunc);
    }

    //The DPI encoded in a context handle, or 0 for an invalid one.
    UINT User32::GetDpiFromDpiAwarenessContext(DPI_AWARENESS_CONTEXT value)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetDpiFromDpiAwarenessContext);
        return Library::Call< UINT, DPI_AWARENESS_CONTEXT >(pFunc, value);
    }

    UINT User32::GetSystemDpiForProcess(HANDLE hProcess)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetSystemDpiForProcess);
        return Library::Call< UINT, HANDLE >(pFunc, hProcess);
    }

    //The DPI-parameterised twin of the wrapped GetSystemMetrics stem (user32.h:712 - declared, never defined here).
    //Returns 0 on failure, which is also a legal metric value, so the return alone cannot decide success; the SDK
    //sets last error. nIndex and dpi are the caller's to validate.
    int User32::GetSystemMetricsForDpi(int nIndex, UINT dpi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetSystemMetricsForDpi);
        return Library::Call< int, int, UINT >(pFunc, nIndex, dpi);
    }

    //Get* returns the current thread context, which the SDK documents as never NULL; SetThreadDpiAwarenessContext
    //below returns the PREVIOUS context, so a caller that changes it must keep the returned handle and restore it -
    //the layer keeps no scope guard, that contract stays with the caller.
    DPI_AWARENESS_CONTEXT User32::GetThreadDpiAwarenessContext(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetThreadDpiAwarenessContext);
        return Library::Call< DPI_AWARENESS_CONTEXT >(pFunc);
    }

    DPI_HOSTING_BEHAVIOR User32::GetThreadDpiHostingBehavior(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetThreadDpiHostingBehavior);
        return Library::Call< DPI_HOSTING_BEHAVIOR >(pFunc);
    }

    DPI_AWARENESS_CONTEXT User32::GetWindowDpiAwarenessContext(HWND hwnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetWindowDpiAwarenessContext);
        return Library::Call< DPI_AWARENESS_CONTEXT, HWND >(pFunc, hwnd);
    }

    DPI_HOSTING_BEHAVIOR User32::GetWindowDpiHostingBehavior(HWND hwnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetWindowDpiHostingBehavior);
        return Library::Call< DPI_HOSTING_BEHAVIOR, HWND >(pFunc, hwnd);
    }

    //The validity test for the opaque handles - the SDK's own answer to "is this one of the sentinel contexts?".
    BOOL User32::IsValidDpiAwarenessContext(DPI_AWARENESS_CONTEXT value)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsValidDpiAwarenessContext);
        return Library::Call< BOOL, DPI_AWARENESS_CONTEXT >(pFunc, value);
    }

    BOOL User32::SetDialogControlDpiChangeBehavior(HWND hWnd, DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS mask, DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS values)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetDialogControlDpiChangeBehavior);
        return Library::Call< BOOL, HWND, DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS, DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS >(pFunc, hWnd, mask, values);
    }

    BOOL User32::SetDialogDpiChangeBehavior(HWND hDlg, DIALOG_DPI_CHANGE_BEHAVIORS mask, DIALOG_DPI_CHANGE_BEHAVIORS values)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetDialogDpiChangeBehavior);
        return Library::Call< BOOL, HWND, DIALOG_DPI_CHANGE_BEHAVIORS, DIALOG_DPI_CHANGE_BEHAVIORS >(pFunc, hDlg, mask, values);
    }

    //One-shot: the SDK lets the process context be set once, before DPI awareness is otherwise fixed, and returns
    //FALSE (not the previous value, unlike the thread setter) once that window has passed.
    BOOL User32::SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT value)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetProcessDpiAwarenessContext);
        return Library::Call< BOOL, DPI_AWARENESS_CONTEXT >(pFunc, value);
    }

    DPI_AWARENESS_CONTEXT User32::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT dpiContext)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetThreadDpiAwarenessContext);
        return Library::Call< DPI_AWARENESS_CONTEXT, DPI_AWARENESS_CONTEXT >(pFunc, dpiContext);
    }

    //Returns the previous hosting behavior, as the thread awareness setter does. DPI_HOSTING_BEHAVIOR is the enum at
    //windef.h:271, and the SDK floor for all three hosting names is WINVER >= 0x0606.
    DPI_HOSTING_BEHAVIOR User32::SetThreadDpiHostingBehavior(DPI_HOSTING_BEHAVIOR value)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetThreadDpiHostingBehavior);
        return Library::Call< DPI_HOSTING_BEHAVIOR, DPI_HOSTING_BEHAVIOR >(pFunc, value);
    }

    //The DPI-parameterised twin of the wrapped SystemParametersInfo stem (user32.h:714 - declared, never defined
    //here). pvParam is _Pre_maybenull_ _Post_valid_: its type and size are fixed by uiAction, which the layer does not
    //and cannot validate. fWinIni carries the SPI_SETWININI flags plus the send-change flag - a flag set passed as
    //UINT, exactly as the SDK declares it.
    BOOL User32::SystemParametersInfoForDpi(UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni, UINT dpi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SystemParametersInfoForDpi);
        return Library::Call< BOOL, UINT, UINT, PVOID, UINT, UINT >(pFunc, uiAction, uiParam, pvParam, fWinIni, dpi);
    }

}}}//qor::win::api
