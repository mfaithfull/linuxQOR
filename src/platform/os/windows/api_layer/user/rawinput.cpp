// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//=== api_layer conventions ========================================= Added 9 Nov 2026
//   (T9 step 9b: the "//Raw Input functions" section declared 7 members non-static and no TU defined any of them - new TU)
//
//T9 (registers/tasks.md, D26) is the tranche queue for U4's remaining declared-but-undefined surface, and this TU lands
//step 9b's two halves together the way 9a's strings.cpp, T1's security.cpp, T3's powermanagement.cpp, T4's help.cpp,
//T5's ime.cpp and T6's listbox.cpp did: it defines all 7 members of user32.h's "//Raw Input functions" (user32.h:543-549)
//and user32.h gains `static` on all 7 declarations. U4's link gap (a caller of any of the 7 failed at LNK2019) and U18's
//non-static shape close together. Coverage stays 759 of 1,018 - all 7 were already declared, so the instrument counted
//them; this closes a link gap, never a coverage gap (the arithmetic D24 recorded for T6). The section is 7/7 declared,
//7/7 defined and 7/7 bound, and it contributes no raw gap: registers/u18-coverage.md names no Raw Input name as excluded.
//
//The AW spelling is load-bearing, not stylistic: um\WinUser.h:15280-:15284 is "#ifdef UNICODE / #define GetRawInputDeviceInfo
//GetRawInputDeviceInfoW / #else / #define GetRawInputDeviceInfo GetRawInputDeviceInfoA / #endif", and user32.h:16 #undef's
//that stem before the class is declared, so the member speaks a name the SDK macro no longer rewrites and user32.dll never
//exported (G2). A bare qor_pp_useswinapi would stringize that un-exported stem, cache a NULL pFunc for the process
//lifetime and throw on every call (G5). The other 6 stems are neither #undef'd at user32.h:13-:21 nor macro switches, so
//the bare form is the D13 spelling for them and qor_pp_useswinapiAW (which pastes Stem##W at kernel32.h:77) would bind
//GetRawInputBuffer to the non-existent export GetRawInputBufferW. cl /EP on this TU resolves the 7 bindings to the
//literals "DefRawInputProc" "GetRawInputBuffer" "GetRawInputData" "GetRawInputDeviceInfoW" "GetRawInputDeviceList"
//"GetRegisteredRawInputDevices" "RegisterRawInputDevices" against the module literal L"user32".
//
//SDK availability, read at the declaration line in um\WinUser.h 10.0.26100.0: GetRawInputData :15206, GetRawInputDeviceInfoA
//:15267, GetRawInputDeviceInfoW :15275, GetRawInputBuffer :15293, RegisterRawInputDevices :15349, GetRegisteredRawInputDevices
//:15357, GetRawInputDeviceList :15371, DefRawInputProc :15379. All 8 sit at depth 3 - #ifndef _WINUSER_ (:13), #if(_WIN32_WINNT
//>= 0x0501) (:14935) and a Desktop Family partition (#if at :15201 for GetRawInputData, :15224 for the DeviceInfo pair and
//GetRawInputBuffer, :15344 for the Register/Registered/List/DefRawInputProc run). None of them is inside the #ifndef NOUSER
//region, which ends at :12263, so no shrink macro can drop these prototypes; only the unpinned WINAPI_FAMILY partition can,
//which leaves D21's pin (kernel32.h:22-30, _WIN32_WINNT 0x0A00) as the live condition - user32.h adds no guard of its own.
//
//Signatures - D18's rule, the published signature is the prototype of the export this member binds, parameter names
//included. All 7 declarations already copy it exactly (types, parameter names and order), so the header edit is `static`
//only: no type and no parameter name moves, so no mangled name moves and no caller follows. The SDK's SAL annotations are
//carried as the SDK states them and none of this layer enforces them: 0 of the 8 prototype blocks carry _Check_return_.
//
//Binding - 6 x qor_pp_useswinapi(user32, <literal>) and 1 x qor_pp_useswinapiAW(user32, GetRawInputDeviceInfo). The
//loaded user32.dll table has one row for each of the 6 bare literals and none for the stem GetRawInputDeviceInfo: that
//stem's exports are the pair GetRawInputDeviceInfoA 1B7 / GetRawInputDeviceInfoW 1B8. The other 6 literals: DefRawInputProc
//A5, GetRawInputBuffer 1B5, GetRawInputData 1B6, GetRawInputDeviceList 1B9, GetRegisteredRawInputDevices 1BC and
//RegisterRawInputDevices 2F2, so no binding in this TU can cache a NULL pFunc except the one the AW form fixes.
//
//Return conventions (U1/U2) - 0 Win32ErrorHandler and 0 CheckReturn in this TU, and 0 of the 8 SDK prototypes asks for
//one, so a discarded failure is silent on both sides at once. The mix is 6 numeric (5 UINT counts and 1 LRESULT) plus 1
//BOOL: the UINTs are counts of records, bytes or devices and 0 is a legal answer for each, and the BOOL is the one member
//whose FALSE is unambiguous, so the session-27 soundness test ("only sound where the failure value is not also a legal
//success value") bars a CheckNonZero at all 6 numerics. Every value comes back as the OS wrote it, which is what leaves a
//swallowed SEH fault and a legitimate 0 indistinguishable to the caller (U7).
//
//U17: GetRawInputDeviceInfo is one of the eleven A/W members declared without the folder's T suffix. It is not renamed
//here - the session-11 defer policy keeps the eleven as they are until one folder-wide pass, and listbox.cpp's DlgDirList
//and DlgDirSelectEx plus ime.cpp's SendIMEMessageEx are the identical case.
//
//Liveness (U15/U19): 0 call sites of any of the 7 names go through the layer - all 7 are among the 18 names _tmp_decl_no_def_repo_scan.ps1
//finds never mentioned as User32:: anywhere in src/examples/test. U19 reads 0 LAYER / 0 INDIRECT / 1 BYPASS / 0 MEMBER for the
//7 stems: one live call reaches the SDK import thunk instead of this layer, at ui/gl/windowimpl.cpp:291 in
//WindowImpl::DisableRawMouseMotionWin32. The identical call at :280 is dead - it sits inside the block comment opened at
//:275 and closed at :284 - so a naive grep reads 2 sites where the instrumented scan reads 1.
//
//Record: the 7 declarations needed only `static`, so this TU is the whole of step 9b. The section is 7/7 declared, 7/7
//defined and 7/7 bound; the folder moves 48 -> 49 sources, 582 -> 589 static, 52 -> 45 non-static and 581 -> 588 bindings
//over the close (_tmp_static_census.ps1).
//
//Gate: rawinput.cpp standalone at /W4 exit 0, then the folder 49/49 at /W4 exit 0, cl /EP emitting the 7 literals above,
//and - because user32.h moved - the all-libraries run as a diff against a pre-edit run (116 cpp echoes, 166 diagnostic
//lines, 1 C1083 at opengl/wgl.cpp:31, exit 2).

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //The callback the OS calls at the end of a WM_INPUT message. paRawInput is not one record but an ARRAY of pointers into
    //the raw input block chain - the SDK declares PRAWINPUT* (RAWINPUT is um\WinUser.h:15173-:15180 and NEXTRAWINPUTBLOCK
    //:15191 is the macro that walks it), nInput says how many entries the array holds and cbSizeHeader the RAWINPUTHEADER
    //size the caller assumed for each block. Nothing here walks, sizes or inspects the chain, and the LRESULT is handed back
    //as the OS wrote it - no handler and no GetLastError pass-through, so whatever the OS reported is the caller's to read.
    LRESULT User32::DefRawInputProc(PRAWINPUT* paRawInput, INT nInput, UINT cbSizeHeader)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DefRawInputProc);
        return Library::Call< LRESULT, PRAWINPUT *, INT, UINT >(pFunc, paRawInput, nInput, cbSizeHeader);
    }

    //The bulk read over the whole chain. The capacity is not a separate argument - it is the value the caller left in pcbSize
    //(_Out_writes_bytes_opt_(*pcbSize)) and the same PUINT carries the bytes copied back out (_Inout_), so the caller owns
    //both the buffer and that word and the layer forwards them as written. The UINT is a count and 0 is a legal answer.
    UINT User32::GetRawInputBuffer(PRAWINPUT pData, PUINT pcbSize, UINT cbSizeHeader)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetRawInputBuffer);
        return Library::Call< UINT, PRAWINPUT, PUINT, UINT >(pFunc, pData, pcbSize, cbSizeHeader);
    }

    //One record from the WM_INPUT message in flight: hRawInput is that message's wParam (HRAWINPUT is DECLARE_HANDLE at
    //um\WinUser.h:14944 and is only meaningful while the message is being processed), and uiCommand picks the shape from the
    //SDK's own pair RID_INPUT 0x10000003 / RID_HEADER 0x10000005 (winuser.h:15197/:15198). cbSizeHeader is the header size the
    //caller assumes, pData the destination and pcbSize capacity-in/bytes-out again. A count, so 0 is a legal answer.
    UINT User32::GetRawInputData(HRAWINPUT hRawInput, UINT uiCommand, LPVOID pData, PUINT pcbSize, UINT cbSizeHeader)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetRawInputData);
        return Library::Call< UINT, HRAWINPUT, UINT, LPVOID, PUINT, UINT >(pFunc, hRawInput, uiCommand, pData, pcbSize, cbSizeHeader);
    }

    //The one member here whose binding literal is not its own name - see the AW note in the banner above. hDevice is _In_opt_,
    //so NULL names no device, and uiCommand selects which of the RID_DEVICE_INFO shapes (:15254-:15262, cbSize + dwType + a
    //union of the mouse/keyboard/HID variants) is read into pData; pcbSize is capacity-in/bytes-out. The UINT is the count and
    //comes back unchecked.
    UINT User32::GetRawInputDeviceInfo(HANDLE hDevice, UINT uiCommand, LPVOID pData, PUINT pcbSize)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetRawInputDeviceInfo);
        return Library::Call< UINT, HANDLE, UINT, LPVOID, PUINT >(pFunc, hDevice, uiCommand, pData, pcbSize);
    }

    //Every raw input device the system has: pRawInputDeviceList is the caller's array of RAWINPUTDEVICELIST (:15366) and cbSize
    //the element size, so those two have to agree and the layer neither sizes the array nor checks that they do. puiNumDevices
    //is _Inout_ and carries the count out; the returned count is handed over as written.
    UINT User32::GetRawInputDeviceList(PRAWINPUTDEVICELIST pRawInputDeviceList, PUINT puiNumDevices, UINT cbSize)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetRawInputDeviceList);
        return Library::Call< UINT, PRAWINPUTDEVICELIST, PUINT, UINT >(pFunc, pRawInputDeviceList, puiNumDevices, cbSize);
    }

    //The devices the caller registered below: the same shape as the list above over PRAWINPUTDEVICE (:15306), so the array
    //element size and cbSize are the caller's to agree on and puiNumDevices is the count out.
    UINT User32::GetRegisteredRawInputDevices(PRAWINPUTDEVICE pRawInputDevices, PUINT puiNumDevices, UINT cbSize)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetRegisteredRawInputDevices);
        return Library::Call< UINT, PRAWINPUTDEVICE, PUINT, UINT >(pFunc, pRawInputDevices, puiNumDevices, cbSize);
    }

    //The only BOOL here, and a real success flag, so its FALSE is unambiguous - though the reason for it is not, and this layer
    //carries no GetLastError pass-through that would give the caller one. pRawInputDevices is an array of PCRAWINPUTDEVICE,
    //which the SDK typedefs CONST RAWINPUTDEVICE* at :15308 so the const is already in the type and no U23-style const move is
    //owed here; uiNumDevices is the element count and cbSize the element size, and _In_reads_(uiNumDevices) is the SDK's own
    //statement that the two agree. RAWINPUTDEVICE (:15301-:15306) is usUsagePage/usUsage/dwFlags/hwndTarget, and the SDK notes
    //a NULL hwndTarget "follows keyboard focus" - the layer forwards the array the caller built, unchanged.
    BOOL User32::RegisterRawInputDevices(PCRAWINPUTDEVICE pRawInputDevices, UINT uiNumDevices, UINT cbSize)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, RegisterRawInputDevices);
        return Library::Call< BOOL, PCRAWINPUTDEVICE, UINT, UINT >(pFunc, pRawInputDevices, uiNumDevices, cbSize);
    }

}}}   //qor::win::api

