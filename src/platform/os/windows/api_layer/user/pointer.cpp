// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//=== api_layer conventions ========================================= Reviewed 6 Nov 2026
//   (T8 step 6, tranche 6a: the Pointer input family - 32 wrappers, new TU)
//
//This TU is coverage added under D18 (docs/agent/DECISIONS.md): purpose.md states the library intends a full set of
//wrappers over the user32.dll exports, and registers/u18-coverage.md buckets the gap by family. The Pointer bucket was
//counted at 32 and that count is now reconciled name-for-name: the export table has 41 *Pointer* names, 9 of them have
//no prototype in winuser.h 10.0.26100.0, and 41 - 9 = 32. The 32 declared-and-exported names are wrapped here; the 9
//undocumented ones are excluded in writing by the 5 Nov scope rule ("the exports that winuser.h declares"):
//GetExtendedPointerDeviceProperty, GetPointerDeviceInputSpace, GetPointerDeviceOrientation,
//GetPointerFrameArrivalTimes, GetPointerFrameTimes, InitializePointerDeviceInjection,
//InitializePointerDeviceInjectionEx, InjectPointerInput, SetPointerDeviceInputSpace. A tenth name moves the other way
//and is excluded too: EnableMouseInPointerForThread IS declared (winuser.h:6715, inside
//#if WDK_NTDDI_VERSION >= NTDDI_WIN10_RS3) but it is ABSENT from the user32.dll export table - the only
//EnableMouseInPointer* export is ordinal EA EnableMouseInPointer - so wrapping it would cache a permanently null
//GetProcAddress result. That corrects an interim note from the measurement pass, which read the header declaration as
//availability.
//
//Signature rule (D18): the published signature IS the winuser.h prototype, parameter names included - 32/32 here copy it
//exactly. SDK lines: 6528, 6535, 6542, 6549, 6557, 6565, 6574, 6581, 6589, 6597, 6606, 6613, 6621, 6629, 6640, 6646,
//6653, 6660, 6668, 6677, 6685, 6693, 6702, 6708, 15458, 15465, 15472, 15480, 15487, 15495, 15503, 6854. Every type those
//prototypes use (POINTER_INFO, POINTER_TOUCH_INFO, POINTER_PEN_INFO, POINTER_TYPE_INFO, POINTER_DEVICE_INFO,
//POINTER_DEVICE_PROPERTY, POINTER_DEVICE_CURSOR_INFO, POINTER_INPUT_TYPE, POINTER_FEEDBACK_MODE,
//HSYNTHETICPOINTERDEVICE, INPUT_TRANSFORM) arrives through kernel32.h:12's #include <Windows.h> - neither this TU nor
//user32.h declares a type of its own for the family. POINTER_INPUT_TYPE is typedef DWORD (:6312), not an enum type, and
//is spelled as the SDK spells it.
//
//Guarding: 31 of the 32 prototypes sit inside #if(WINVER >= 0x0602) (opens :6294, and :15390 for the GetPointerDevice*
//group); GetPointerInputTransform sits inside #if(WINVER >= 0x0603) (:6823). WINVER defaults to 0x0A00 and nothing in
//this build sets it, so all 32 are live and the layer adds no #if.
//
//Binding (D13): 32 qor_pp_useswinapi sites, all bare-stem. None of the 32 has an A/W family and none is a function-like
//macro (every "#define <Name>" in winuser.h was checked: 0 hits), so the bare stem is the name GetProcAddress is given,
//and no #undef is needed at user32.h:9-17 - unlike CopyCursor (cursor.cpp), where the macro is what makes the spelling
//lie. Idiom counts: 32 qor_pp_fcontext, 32 definition heads qualified User32::, 31 Library::Call + 1
//Library::voidCall, 32/32 with explicit template arguments (U14(b) clean). The voidCall is
//DestroySyntheticPointerDevice, whose SDK prototype really returns VOID (:6693). CreateSyntheticPointerDevice returns
//HSYNTHETICPOINTERDEVICE (DECLARE_HANDLE at :6673) with NULL as the failure value, and the GetPointer*Info /
//GetPointerFrame* members take in/out count parameters (deviceCount, entriesCount, pointerCount) whose buffer contract
//stays the caller's. Under the D15/D18 pass-through policy nothing here checks a return: the semantics are stated in
//this block, not coded, and no CheckReturn is introduced.
//
//Consumers: none in-tree (a new surface - src, examples and test had no pointer-input code before this TU), so under
//D14 there is nothing to record and no U19 LAYER/BYPASS split to book; the point of the tranche is that the layer can
//now be called at all. user32.h gained the matching 32 declarations in a new "//Pointer input functions" section after
//"//Mouse functions" (U12's section question resolved by placement, as step 5 did it), and CMakeLists.txt gained
//pointer.cpp, making the folder 39 sources.
//
//Gate: this TU standalone at /W4 exit 0, then the folder 39/39 exit 0 with no diagnostics, both re-run after this
//banner (dated by tmp/obj/pointer.obj being newer than the source). Because user32.h changed, the 106-source
//all-libraries gate was run before and after and read as a diff (pre-fix baseline: 106 echoes, 165 warning lines, 1
//pre-existing C1083 at opengl/wgl.cpp:31). cl /EP on this TU yields 32 binding literals, each byte-equal to an export
//name.

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    HSYNTHETICPOINTERDEVICE User32::CreateSyntheticPointerDevice(POINTER_INPUT_TYPE pointerType, ULONG maxCount, POINTER_FEEDBACK_MODE mode)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CreateSyntheticPointerDevice);
        return Library::Call< HSYNTHETICPOINTERDEVICE, POINTER_INPUT_TYPE, ULONG, POINTER_FEEDBACK_MODE >(pFunc, pointerType, maxCount, mode);
    }

    void User32::DestroySyntheticPointerDevice(HSYNTHETICPOINTERDEVICE device)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DestroySyntheticPointerDevice);
        Library::voidCall< HSYNTHETICPOINTERDEVICE >(pFunc, device);
    }

    BOOL User32::EnableMouseInPointer(BOOL fEnable)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EnableMouseInPointer);
        return Library::Call< BOOL, BOOL >(pFunc, fEnable);
    }

    BOOL User32::GetPointerCursorId(UINT32 pointerId, UINT32* cursorId)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerCursorId);
        return Library::Call< BOOL, UINT32, UINT32* >(pFunc, pointerId, cursorId);
    }

    BOOL User32::GetPointerDevice(HANDLE device, POINTER_DEVICE_INFO* pointerDevice)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerDevice);
        return Library::Call< BOOL, HANDLE, POINTER_DEVICE_INFO* >(pFunc, device, pointerDevice);
    }

    BOOL User32::GetPointerDeviceCursors(HANDLE device, UINT32* cursorCount, POINTER_DEVICE_CURSOR_INFO* deviceCursors)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerDeviceCursors);
        return Library::Call< BOOL, HANDLE, UINT32*, POINTER_DEVICE_CURSOR_INFO* >(pFunc, device, cursorCount, deviceCursors);
    }

    BOOL User32::GetPointerDeviceProperties(HANDLE device, UINT32* propertyCount, POINTER_DEVICE_PROPERTY* pointerProperties)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerDeviceProperties);
        return Library::Call< BOOL, HANDLE, UINT32*, POINTER_DEVICE_PROPERTY* >(pFunc, device, propertyCount, pointerProperties);
    }

    BOOL User32::GetPointerDeviceRects(HANDLE device, RECT* pointerDeviceRect, RECT* displayRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerDeviceRects);
        return Library::Call< BOOL, HANDLE, RECT*, RECT* >(pFunc, device, pointerDeviceRect, displayRect);
    }

    BOOL User32::GetPointerDevices(UINT32* deviceCount, POINTER_DEVICE_INFO* pointerDevices)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerDevices);
        return Library::Call< BOOL, UINT32*, POINTER_DEVICE_INFO* >(pFunc, deviceCount, pointerDevices);
    }

    BOOL User32::GetPointerFrameInfo(UINT32 pointerId, UINT32* pointerCount, POINTER_INFO* pointerInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerFrameInfo);
        return Library::Call< BOOL, UINT32, UINT32*, POINTER_INFO* >(pFunc, pointerId, pointerCount, pointerInfo);
    }

    BOOL User32::GetPointerFrameInfoHistory(UINT32 pointerId, UINT32* entriesCount, UINT32* pointerCount, POINTER_INFO* pointerInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerFrameInfoHistory);
        return Library::Call< BOOL, UINT32, UINT32*, UINT32*, POINTER_INFO* >(pFunc, pointerId, entriesCount, pointerCount, pointerInfo);
    }

    BOOL User32::GetPointerFramePenInfo(UINT32 pointerId, UINT32* pointerCount, POINTER_PEN_INFO* penInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerFramePenInfo);
        return Library::Call< BOOL, UINT32, UINT32*, POINTER_PEN_INFO* >(pFunc, pointerId, pointerCount, penInfo);
    }

    BOOL User32::GetPointerFramePenInfoHistory(UINT32 pointerId, UINT32* entriesCount, UINT32* pointerCount, POINTER_PEN_INFO* penInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerFramePenInfoHistory);
        return Library::Call< BOOL, UINT32, UINT32*, UINT32*, POINTER_PEN_INFO* >(pFunc, pointerId, entriesCount, pointerCount, penInfo);
    }

    BOOL User32::GetPointerFrameTouchInfo(UINT32 pointerId, UINT32* pointerCount, POINTER_TOUCH_INFO* touchInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerFrameTouchInfo);
        return Library::Call< BOOL, UINT32, UINT32*, POINTER_TOUCH_INFO* >(pFunc, pointerId, pointerCount, touchInfo);
    }

    BOOL User32::GetPointerFrameTouchInfoHistory(UINT32 pointerId, UINT32* entriesCount, UINT32* pointerCount, POINTER_TOUCH_INFO* touchInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerFrameTouchInfoHistory);
        return Library::Call< BOOL, UINT32, UINT32*, UINT32*, POINTER_TOUCH_INFO* >(pFunc, pointerId, entriesCount, pointerCount, touchInfo);
    }

    BOOL User32::GetPointerInfo(UINT32 pointerId, POINTER_INFO* pointerInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerInfo);
        return Library::Call< BOOL, UINT32, POINTER_INFO* >(pFunc, pointerId, pointerInfo);
    }

    BOOL User32::GetPointerInfoHistory(UINT32 pointerId, UINT32* entriesCount, POINTER_INFO* pointerInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerInfoHistory);
        return Library::Call< BOOL, UINT32, UINT32*, POINTER_INFO* >(pFunc, pointerId, entriesCount, pointerInfo);
    }

    BOOL User32::GetPointerInputTransform(UINT32 pointerId, UINT32 historyCount, INPUT_TRANSFORM* inputTransform)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerInputTransform);
        return Library::Call< BOOL, UINT32, UINT32, INPUT_TRANSFORM* >(pFunc, pointerId, historyCount, inputTransform);
    }

    BOOL User32::GetPointerPenInfo(UINT32 pointerId, POINTER_PEN_INFO* penInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerPenInfo);
        return Library::Call< BOOL, UINT32, POINTER_PEN_INFO* >(pFunc, pointerId, penInfo);
    }

    BOOL User32::GetPointerPenInfoHistory(UINT32 pointerId, UINT32* entriesCount, POINTER_PEN_INFO* penInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerPenInfoHistory);
        return Library::Call< BOOL, UINT32, UINT32*, POINTER_PEN_INFO* >(pFunc, pointerId, entriesCount, penInfo);
    }

    BOOL User32::GetPointerTouchInfo(UINT32 pointerId, POINTER_TOUCH_INFO* touchInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerTouchInfo);
        return Library::Call< BOOL, UINT32, POINTER_TOUCH_INFO* >(pFunc, pointerId, touchInfo);
    }

    BOOL User32::GetPointerTouchInfoHistory(UINT32 pointerId, UINT32* entriesCount, POINTER_TOUCH_INFO* touchInfo)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerTouchInfoHistory);
        return Library::Call< BOOL, UINT32, UINT32*, POINTER_TOUCH_INFO* >(pFunc, pointerId, entriesCount, touchInfo);
    }

    BOOL User32::GetPointerType(UINT32 pointerId, POINTER_INPUT_TYPE* pointerType)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetPointerType);
        return Library::Call< BOOL, UINT32, POINTER_INPUT_TYPE* >(pFunc, pointerId, pointerType);
    }

    BOOL User32::GetRawPointerDeviceData(UINT32 pointerId, UINT32 historyCount, UINT32 propertiesCount, POINTER_DEVICE_PROPERTY* pProperties, LONG* pValues)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetRawPointerDeviceData);
        return Library::Call< BOOL, UINT32, UINT32, UINT32, POINTER_DEVICE_PROPERTY*, LONG* >(pFunc, pointerId, historyCount, propertiesCount, pProperties, pValues);
    }

    BOOL User32::InjectSyntheticPointerInput(HSYNTHETICPOINTERDEVICE device, const POINTER_TYPE_INFO* pointerInfo, UINT32 count)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, InjectSyntheticPointerInput);
        return Library::Call< BOOL, HSYNTHETICPOINTERDEVICE, const POINTER_TYPE_INFO*, UINT32 >(pFunc, device, pointerInfo, count);
    }

    BOOL User32::IsMouseInPointerEnabled(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsMouseInPointerEnabled);
        return Library::Call< BOOL >(pFunc);
    }

    BOOL User32::RegisterPointerDeviceNotifications(HWND window, BOOL notifyRange)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, RegisterPointerDeviceNotifications);
        return Library::Call< BOOL, HWND, BOOL >(pFunc, window, notifyRange);
    }

    BOOL User32::RegisterPointerInputTarget(HWND hwnd, POINTER_INPUT_TYPE pointerType)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, RegisterPointerInputTarget);
        return Library::Call< BOOL, HWND, POINTER_INPUT_TYPE >(pFunc, hwnd, pointerType);
    }

    BOOL User32::RegisterPointerInputTargetEx(HWND hwnd, POINTER_INPUT_TYPE pointerType, BOOL fObserve)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, RegisterPointerInputTargetEx);
        return Library::Call< BOOL, HWND, POINTER_INPUT_TYPE, BOOL >(pFunc, hwnd, pointerType, fObserve);
    }

    BOOL User32::SkipPointerFrameMessages(UINT32 pointerId)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SkipPointerFrameMessages);
        return Library::Call< BOOL, UINT32 >(pFunc, pointerId);
    }

    BOOL User32::UnregisterPointerInputTarget(HWND hwnd, POINTER_INPUT_TYPE pointerType)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UnregisterPointerInputTarget);
        return Library::Call< BOOL, HWND, POINTER_INPUT_TYPE >(pFunc, hwnd, pointerType);
    }

    BOOL User32::UnregisterPointerInputTargetEx(HWND hwnd, POINTER_INPUT_TYPE pointerType)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UnregisterPointerInputTargetEx);
        return Library::Call< BOOL, HWND, POINTER_INPUT_TYPE >(pFunc, hwnd, pointerType);
    }

}}}//qor::win::api
