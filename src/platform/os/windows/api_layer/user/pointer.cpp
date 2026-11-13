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
#include "../kernel/errorhandler.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //Return policy (U2, table row CreateSyntheticPointerDevice / pointer.cpp, 12 Nov 2026): the row reads checked with
    //checkClass CheckNonZeroHandle< HSYNTHETICPOINTERDEVICE > - docQuote "If the function succeeds, the return value is a
    //handle to the pointer injection device. Otherwise, it returns null. To retrieve extended error information, call the
    //GetLastError function." so NULL is reserved for a failed call and the caller hands the device handle straight to
    //InjectSyntheticPointerInput, so it is never legitimately NULL from this call; the row's sdkRetType column is blank,
    //so the class base is the published HSYNTHETICPOINTERDEVICE and CheckNonZeroHandle (returncheck.h:137, template<
    //typename THandle > : public AutoErrorCheck< THandle >) covers it - no new check class. AUDIT: the table counts 0
    //in-tree callers for the exact stem, so the check is change-safe and no call site needs reading.
    HSYNTHETICPOINTERDEVICE User32::CreateSyntheticPointerDevice(POINTER_INPUT_TYPE pointerType, ULONG maxCount, POINTER_FEEDBACK_MODE mode)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CreateSyntheticPointerDevice);
        CheckReturn< HSYNTHETICPOINTERDEVICE, CheckNonZeroHandle< HSYNTHETICPOINTERDEVICE > >::TType hResult = Library::Call< HSYNTHETICPOINTERDEVICE, POINTER_INPUT_TYPE, ULONG, POINTER_FEEDBACK_MODE >(pFunc, pointerType, maxCount, mode);
        return hResult;
    }

    void User32::DestroySyntheticPointerDevice(HSYNTHETICPOINTERDEVICE device)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DestroySyntheticPointerDevice);
        Library::voidCall< HSYNTHETICPOINTERDEVICE >(pFunc, device);
    }

    //Return policy (U2, table row EnableMouseInPointer / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - docQuote "If the function succeeds, the return value is non-zero. If the function fails, the return value is
    //zero. To get extended error information, call GetLastError ." so zero is reserved for a failed call and the flag
    //being set is a parameter, so the BOOL carries no answer of its own; deliberately unlike the IsMouseInPointerEnabled
    //row, whose zero reports the flag the caller set. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new
    //check class. AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::EnableMouseInPointer(BOOL fEnable)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EnableMouseInPointer);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, BOOL >(pFunc, fEnable);
        return hResult;
    }

    //Return policy (U2, table row GetPointerCursorId / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - docQuote "If the function succeeds, the return value is non-zero. If the function fails, the return value is
    //zero. To get extended error information, call GetLastError ." so zero is reserved for a failed call and the cursor
    //id the call reports is an out-parameter, so the BOOL carries no answer and a caller that gets zero back has a
    //broken call rather than a cursor id of 0. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new
    //check class. AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::GetPointerCursorId(UINT32 pointerId, UINT32* cursorId)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerCursorId);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32, UINT32* >(pFunc, pointerId, cursorId);
        return hResult;
    }

    //Return policy (U2, table row GetPointerDevice / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - docQuote "If this function succeeds, it returns TRUE. Otherwise, it returns FALSE. To retrieve extended
    //error information, call the GetLastError function." so FALSE is reserved for a failed call and the device the call
    //reports is the POINTER_DEVICE_INFO out-parameter, so the BOOL carries no answer. sdkRetType BOOL so BoolCheck
    //(returncheck.h:94) covers it - no new check class. AUDIT: the table counts 0 in-tree callers, so the check is
    //change-safe.
    BOOL User32::GetPointerDevice(HANDLE device, POINTER_DEVICE_INFO* pointerDevice)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerDevice);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, HANDLE, POINTER_DEVICE_INFO* >(pFunc, device, pointerDevice);
        return hResult;
    }

    //Return policy (U2, table row GetPointerDeviceCursors / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - docQuote "TRUE if the function succeeds; otherwise, FALSE. If the function fails, call the GetLastError
    //function for more information." so FALSE is reserved for a failed call and the cursor array plus its count are
    //out-parameters, so an empty cursor list is an out-parameter answer while FALSE is a broken call. sdkRetType BOOL so
    //BoolCheck (returncheck.h:94) covers it - no new check class. AUDIT: the table counts 0 in-tree callers, so the check
    //is change-safe.
    BOOL User32::GetPointerDeviceCursors(HANDLE device, UINT32* cursorCount, POINTER_DEVICE_CURSOR_INFO* deviceCursors)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerDeviceCursors);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, HANDLE, UINT32*, POINTER_DEVICE_CURSOR_INFO* >(pFunc, device, cursorCount, deviceCursors);
        return hResult;
    }

    //Return policy (U2, table row GetPointerDeviceProperties / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - docQuote "TRUE if the function succeeds; otherwise, FALSE. If the function fails, call the GetLastError
    //function for more information." word for word with the GetPointerDeviceCursors row, and here the property array plus
    //its count are out-parameters, so a device with no properties answers through them while FALSE is a failed call. sdkRetType
    //BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. AUDIT: the table counts 0 in-tree callers, so the
    //check is change-safe.
    BOOL User32::GetPointerDeviceProperties(HANDLE device, UINT32* propertyCount, POINTER_DEVICE_PROPERTY* pointerProperties)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerDeviceProperties);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, HANDLE, UINT32*, POINTER_DEVICE_PROPERTY* >(pFunc, device, propertyCount, pointerProperties);
        return hResult;
    }

    //Return policy (U2, table row GetPointerDeviceRects / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - docQuote "TRUE if the function succeeds; otherwise, FALSE. If the function fails, call the GetLastError
    //function for more information.", the same status line as the GetPointerDeviceCursors and GetPointerDeviceProperties
    //rows, and here both rectangles are out-parameters, so the BOOL carries no answer. sdkRetType BOOL so BoolCheck
    //(returncheck.h:94) covers it - no new check class. AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::GetPointerDeviceRects(HANDLE device, RECT* pointerDeviceRect, RECT* displayRect)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerDeviceRects);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, HANDLE, RECT*, RECT* >(pFunc, device, pointerDeviceRect, displayRect);
        return hResult;
    }

    //Return policy (U2, table row GetPointerDevices / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - docQuote "If this function succeeds, it returns TRUE. Otherwise, it returns FALSE. To retrieve extended
    //error information, call the GetLastError function." word for word with the GetPointerDevice row, and here the device
    //array and its count are out-parameters, so a machine with no pointer devices answers through them (a zero count)
    //while FALSE is a failed call. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. AUDIT:
    //the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::GetPointerDevices(UINT32* deviceCount, POINTER_DEVICE_INFO* pointerDevices)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerDevices);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32*, POINTER_DEVICE_INFO* >(pFunc, deviceCount, pointerDevices);
        return hResult;
    }

    //Return policy (U2, table row GetPointerFrameInfo / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - docQuote "If the function succeeds, the return value is non-zero. If the function fails, the return value is
    //zero. To get extended error information, call GetLastError ." so zero is reserved for a failed call and the frame's
    //pointer count is an out-parameter, so a frame with no pointers answers through it while a zero return is a broken
    //call. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. AUDIT: the table counts 0
    //in-tree callers, so the check is change-safe.
    BOOL User32::GetPointerFrameInfo(UINT32 pointerId, UINT32* pointerCount, POINTER_INFO* pointerInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerFrameInfo);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32, UINT32*, POINTER_INFO* >(pFunc, pointerId, pointerCount, pointerInfo);
        return hResult;
    }

    //Return policy (U2, table row GetPointerFrameInfoHistory / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - docQuote "If the function succeeds, the return value is non-zero. If the function fails, the return value is
    //zero. To get extended error information, call GetLastError ." the same status line the GetPointerFrameInfo row takes,
    //so zero is reserved for a failed call while the entries and pointer counts are out-parameters - a history of no
    //entries answers through them, not through a zero return. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it
    //- no new check class. AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::GetPointerFrameInfoHistory(UINT32 pointerId, UINT32* entriesCount, UINT32* pointerCount, POINTER_INFO* pointerInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerFrameInfoHistory);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32, UINT32*, UINT32*, POINTER_INFO* >(pFunc, pointerId, entriesCount, pointerCount, pointerInfo);
        return hResult;
    }

    //Return policy (U2, table row GetPointerFramePenInfo / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - docQuote "If the function succeeds, the return value is non-zero. If the function fails, the return value is
    //zero. To get extended error information, call GetLastError ." so zero is reserved for a failed call and the frame's
    //pen count and POINTER_PEN_INFO array are out-parameters, so a frame with no pen sample answers through them. sdkRetType
    //BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. AUDIT: the table counts 0 in-tree callers, so the
    //check is change-safe.
    BOOL User32::GetPointerFramePenInfo(UINT32 pointerId, UINT32* pointerCount, POINTER_PEN_INFO* penInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerFramePenInfo);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32, UINT32*, POINTER_PEN_INFO* >(pFunc, pointerId, pointerCount, penInfo);
        return hResult;
    }

    //Return policy (U2, table row GetPointerFramePenInfoHistory / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the GetPointerFramePenInfo row's status wording word for word, with the entries count added as a third
    //out-parameter, so zero is reserved for a failed call and an empty pen history answers through the counts. sdkRetType
    //BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. AUDIT: the table counts 0 in-tree callers, so the
    //check is change-safe.
    BOOL User32::GetPointerFramePenInfoHistory(UINT32 pointerId, UINT32* entriesCount, UINT32* pointerCount, POINTER_PEN_INFO* penInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerFramePenInfoHistory);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32, UINT32*, UINT32*, POINTER_PEN_INFO* >(pFunc, pointerId, entriesCount, pointerCount, penInfo);
        return hResult;
    }

    //Return policy (U2, table row GetPointerFrameTouchInfo / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the GetPointerFramePenInfo row's status wording word for word, so zero is reserved for a failed call and the
    //touch count and POINTER_TOUCH_INFO array are out-parameters, a frame with no touch sample answering through them. sdkRetType
    //BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. AUDIT: the table counts 0 in-tree callers, so the
    //check is change-safe.
    BOOL User32::GetPointerFrameTouchInfo(UINT32 pointerId, UINT32* pointerCount, POINTER_TOUCH_INFO* touchInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerFrameTouchInfo);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32, UINT32*, POINTER_TOUCH_INFO* >(pFunc, pointerId, pointerCount, touchInfo);
        return hResult;
    }

    //Return policy (U2, table row GetPointerFrameTouchInfoHistory / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the GetPointerFramePenInfoHistory row's wording word for word with the entries count as the third out-parameter,
    //so zero is reserved for a failed call and an empty touch history answers through the counts. sdkRetType BOOL so BoolCheck
    //(returncheck.h:94) covers it - no new check class. AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::GetPointerFrameTouchInfoHistory(UINT32 pointerId, UINT32* entriesCount, UINT32* pointerCount, POINTER_TOUCH_INFO* touchInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerFrameTouchInfoHistory);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32, UINT32*, UINT32*, POINTER_TOUCH_INFO* >(pFunc, pointerId, entriesCount, pointerCount, touchInfo);
        return hResult;
    }

    //Return policy (U2, table row GetPointerInfo / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the GetPointerFramePenInfo row's status wording, so zero is reserved for a failed call and the POINTER_INFO
    //the caller passes is an out-parameter holding the sample. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it
    //- no new check class. AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::GetPointerInfo(UINT32 pointerId, POINTER_INFO* pointerInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerInfo);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32, POINTER_INFO* >(pFunc, pointerId, pointerInfo);
        return hResult;
    }

    //Return policy (U2, table row GetPointerInfoHistory / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the GetPointerInfo row's status wording with the entries count added as an out-parameter, so zero is reserved
    //for a failed call and an empty history answers through that count. sdkRetType BOOL so BoolCheck (returncheck.h:94)
    //covers it - no new check class. AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::GetPointerInfoHistory(UINT32 pointerId, UINT32* entriesCount, POINTER_INFO* pointerInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerInfoHistory);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32, UINT32*, POINTER_INFO* >(pFunc, pointerId, entriesCount, pointerInfo);
        return hResult;
    }

    //Return policy (U2, table row GetPointerInputTransform / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the GetPointerInfo row's status wording, so zero is reserved for a failed call and the INPUT_TRANSFORM the
    //caller passes is an out-parameter holding the transform. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it
    //- no new check class. AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::GetPointerInputTransform(UINT32 pointerId, UINT32 historyCount, INPUT_TRANSFORM* inputTransform)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerInputTransform);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32, UINT32, INPUT_TRANSFORM* >(pFunc, pointerId, historyCount, inputTransform);
        return hResult;
    }

    //Return policy (U2, table row GetPointerPenInfo / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the GetPointerInfo row's status wording, so zero is reserved for a failed call and the POINTER_PEN_INFO
    //the caller passes is an out-parameter holding the sample. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it
    //- no new check class. AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::GetPointerPenInfo(UINT32 pointerId, POINTER_PEN_INFO* penInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerPenInfo);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32, POINTER_PEN_INFO* >(pFunc, pointerId, penInfo);
        return hResult;
    }

    //Return policy (U2, table row GetPointerPenInfoHistory / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the GetPointerInfoHistory row's wording word for word, so zero is reserved for a failed call and an empty
    //pen history answers through the entries count. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new
    //check class. AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::GetPointerPenInfoHistory(UINT32 pointerId, UINT32* entriesCount, POINTER_PEN_INFO* penInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerPenInfoHistory);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32, UINT32*, POINTER_PEN_INFO* >(pFunc, pointerId, entriesCount, penInfo);
        return hResult;
    }

    //Return policy (U2, table row GetPointerTouchInfo / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the GetPointerInfo row's status wording, so zero is reserved for a failed call and the POINTER_TOUCH_INFO
    //the caller passes is an out-parameter holding the sample. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it
    //- no new check class. AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::GetPointerTouchInfo(UINT32 pointerId, POINTER_TOUCH_INFO* touchInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerTouchInfo);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32, POINTER_TOUCH_INFO* >(pFunc, pointerId, touchInfo);
        return hResult;
    }

    //Return policy (U2, table row GetPointerTouchInfoHistory / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the GetPointerInfoHistory row's wording word for word, so zero is reserved for a failed call and an empty
    //touch history answers through the entries count. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new
    //check class. AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::GetPointerTouchInfoHistory(UINT32 pointerId, UINT32* entriesCount, POINTER_TOUCH_INFO* touchInfo)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerTouchInfoHistory);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32, UINT32*, POINTER_TOUCH_INFO* >(pFunc, pointerId, entriesCount, touchInfo);
        return hResult;
    }

    //Return policy (U2, table row GetPointerType / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the GetPointerInfo row's status wording, so zero is reserved for a failed call and the POINTER_INPUT_TYPE
    //the caller passes is an out-parameter holding the type. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it
    //- no new check class. AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::GetPointerType(UINT32 pointerId, POINTER_INPUT_TYPE* pointerType)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetPointerType);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32, POINTER_INPUT_TYPE* >(pFunc, pointerId, pointerType);
        return hResult;
    }

    //Return policy (U2, table row GetRawPointerDeviceData / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the GetPointerInfo row's status wording, so zero is reserved for a failed call and the property names and
    //value arrays the caller passes are out-parameters holding the samples. sdkRetType BOOL so BoolCheck (returncheck.h:94)
    //covers it - no new check class. AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::GetRawPointerDeviceData(UINT32 pointerId, UINT32 historyCount, UINT32 propertiesCount, POINTER_DEVICE_PROPERTY* pProperties, LONG* pValues)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetRawPointerDeviceData);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32, UINT32, UINT32, POINTER_DEVICE_PROPERTY*, LONG* >(pFunc, pointerId, historyCount, propertiesCount, pProperties, pValues);
        return hResult;
    }

    //Return policy (U2, table row InjectSyntheticPointerInput / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the GetPointerInfo row's status wording, so zero is reserved for a failed call and the POINTER_TYPE_INFO
    //array the caller passes is the input being injected. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it
    //- no new check class. AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::InjectSyntheticPointerInput(HSYNTHETICPOINTERDEVICE device, const POINTER_TYPE_INFO* pointerInfo, UINT32 count)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, InjectSyntheticPointerInput);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, HSYNTHETICPOINTERDEVICE, const POINTER_TYPE_INFO*, UINT32 >(pFunc, device, pointerInfo, count);
        return hResult;
    }

    //Return policy (U2, table row IsMouseInPointerEnabled / pointer.cpp, 12 Nov 2026): the row reads unchecked with no checkClass.
    //A question, not a flag - "If EnableMouseInPointer is set, the return value is nonzero. If EnableMouseInPointer is not
    //set, the return value is zero." - the zero reports the state of the flag the caller is asking about, so nothing is
    //reserved and keying BoolCheck would report a legal disabled state as a failure; deliberately unlike the EnableMouseInPointer
    //row, which is the Set* half and is checked. sdkRetType BOOL; 0 in-tree callers, so the reading is change-safe.
    BOOL User32::IsMouseInPointerEnabled(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsMouseInPointerEnabled);
        return Library::Call< BOOL >(pFunc);
    }

    //Return policy (U2, table row RegisterPointerDeviceNotifications / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the GetPointerInfo row's status wording, so zero is reserved for a failed call and the window handle and
    //the notify-range flag are inputs. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new check class.
    //AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::RegisterPointerDeviceNotifications(HWND window, BOOL notifyRange)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, RegisterPointerDeviceNotifications);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, HWND, BOOL >(pFunc, window, notifyRange);
        return hResult;
    }

    //Return policy (U2, table row RegisterPointerInputTarget / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the GetPointerInfo row's status wording, so zero is reserved for a failed call and the window handle and
    //POINTER_INPUT_TYPE are inputs. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new check class.
    //AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::RegisterPointerInputTarget(HWND hwnd, POINTER_INPUT_TYPE pointerType)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, RegisterPointerInputTarget);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, HWND, POINTER_INPUT_TYPE >(pFunc, hwnd, pointerType);
        return hResult;
    }

    //Return policy (U2, table row RegisterPointerInputTargetEx / pointer.cpp, 12 Nov 2026): the row reads unchecked with no checkClass.
    //The page documents no return value at all - the Return value section is the two words "Not supported." - so the page
    //names no value for success and no value for failure, and D51 step 2 (an API that reserves no return value for errors)
    //reads no-check; sdkRetType BOOL is the header's declaration only. 0 in-tree callers, so the reading is change-safe.
    BOOL User32::RegisterPointerInputTargetEx(HWND hwnd, POINTER_INPUT_TYPE pointerType, BOOL fObserve)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, RegisterPointerInputTargetEx);
        return Library::Call< BOOL, HWND, POINTER_INPUT_TYPE, BOOL >(pFunc, hwnd, pointerType, fObserve);
    }

    //Return policy (U2, table row SkipPointerFrameMessages / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - the GetPointerInfo row's status wording, so zero is reserved for a failed call and the pointer id is the
    //input being skipped. sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it - no new check class. AUDIT: the table
    //counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::SkipPointerFrameMessages(UINT32 pointerId)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SkipPointerFrameMessages);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, UINT32 >(pFunc, pointerId);
        return hResult;
    }

    //Return policy (U2, table row UnregisterPointerInputTarget / pointer.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck - plain status BOOL for a deregistration action, the GetPointerFrameInfo row's wording word for word, so the
    //window handle is a parameter and zero is reserved for a failed call. sdkRetType BOOL so BoolCheck (returncheck.h:94)
    //covers it - no new check class. AUDIT: the table counts 0 in-tree callers, so the check is change-safe.
    BOOL User32::UnregisterPointerInputTarget(HWND hwnd, POINTER_INPUT_TYPE pointerType)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, UnregisterPointerInputTarget);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, HWND, POINTER_INPUT_TYPE >(pFunc, hwnd, pointerType);
        return hResult;
    }

    //Return policy (U2, table row UnregisterPointerInputTargetEx / pointer.cpp, 12 Nov 2026): the row reads unchecked with no checkClass.
    //The RegisterPointerInputTargetEx row's two-word Return value section "Not supported." - the page names no value for
    //success or failure, so D51 step 2 reads no-check and no class is landed; sdkRetType BOOL is the header's declaration
    //only. 0 in-tree callers, so the reading is change-safe.
    BOOL User32::UnregisterPointerInputTargetEx(HWND hwnd, POINTER_INPUT_TYPE pointerType)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, UnregisterPointerInputTargetEx);
        return Library::Call< BOOL, HWND, POINTER_INPUT_TYPE >(pFunc, hwnd, pointerType);
    }

}}}//qor::win::api
